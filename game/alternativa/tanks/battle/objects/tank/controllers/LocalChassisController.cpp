#include "LocalChassisController.h"
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "game/alternativa/tanks/battle/objects/tank/tankchassis/TrackedChassis.h"
#include "game/alternativa/tanks/services/battleinput/BattleInputServiceImpl.h"

extern BattleInputService *battleInputService;
static const int REVERSE_BACK_TURN_MASK = 1 << LocalChassisController::BIT_REVERSE_TURN;


LocalChassisController::LocalChassisController(Tank *tank, ChassisControlListener *listener) :
	ChassisController(tank, listener)
{
	this->isEnabled = false;
	this->reverseBackTurnMask = 0;
}


void LocalChassisController::enable()
{
	if (!this->isEnabled)
	{
		this->isEnabled = true;
		battleInputService->addGameActionListener(this);
	}
}


void LocalChassisController::onGameAction(GameActionEnum param1, bool param2)
{
	int state = controlState;

	switch(param1)
	{
		case GameActionEnum::CHASSIS_FORWARD_MOVEMENT:
			state = MathUtils::changeBitValue(controlState, BIT_FORWARD, param2);
			break;
		case GameActionEnum::CHASSIS_BACKWARD_MOVEMENT:
			state = MathUtils::changeBitValue(controlState, BIT_BACK, param2);
			break;
		case GameActionEnum::CHASSIS_LEFT_MOVEMENT:
			state = MathUtils::changeBitValue(controlState, BIT_LEFT, param2);
			break;
		case GameActionEnum::CHASSIS_RIGHT_MOVEMENT:
			state = MathUtils::changeBitValue(controlState, BIT_RIGHT, param2);
			break;
		default:
			break;
	}

	if (state != controlState)
	{
		int s = state | this->reverseBackTurnMask;
		setControlState(s, TrackedChassis::TURN_SPEED_COUNT);
	}
}


void LocalChassisController::setReversedBackTurn(bool param1)
{
	if (param1)
	{
		this->reverseBackTurnMask = REVERSE_BACK_TURN_MASK;
	}
	else
	{
		this->reverseBackTurnMask = 0;
		this->controlState = this->controlState & ~REVERSE_BACK_TURN_MASK;
	}
}


void LocalChassisController::disable()
{
	if (this->isEnabled)
	{
		this->isEnabled = false;
		battleInputService->removeGameActionListener(this);
		setControlState(0, TrackedChassis::TURN_SPEED_COUNT);
	}
}
