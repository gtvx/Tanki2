#include "ChassisController.h"
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "ChassisControlListener.h"


ChassisController::ChassisController(Tank *tank, ChassisControlListener *listener)
{
	this->tank = tank;
	this->listener = listener;
	this->controlState = 0;
	this->turnSpeedNumber = 0;
	this->appliedControlState = 0;
}


void ChassisController::lock(int status)
{
	bool b = this->isNotLocked();
	this->lockMask.setBits(status);
	if (b && !this->isNotLocked())
	{
		this->applyControlState(0, 0);
	}
}


void ChassisController::unlock(int status)
{
	bool b = this->isNotLocked();
	this->lockMask.clearBits(status);

	if (this->isNotLocked() && !b)
	{
		this->applyControlState(this->controlState, this->turnSpeedNumber);
	}
}


void ChassisController::setControlState(int controlState, int turnSpeedNumber)
{
	this->controlState = controlState;
	this->turnSpeedNumber = turnSpeedNumber;
	if (this->isNotLocked())
	{
		this->applyControlState(controlState, turnSpeedNumber);
	}
}


bool ChassisController::isNotLocked()
{
	return this->lockMask.isEmpty();
}


void ChassisController::applyControlState(int controlState, int turnSpeedNumber)
{
	int _loc3_ = this->appliedControlState;
	this->appliedControlState = controlState;
	int _loc4_ = MathUtils::getBitValue(controlState, BIT_FORWARD) - MathUtils::getBitValue(controlState, BIT_BACK);
	double _loc5_ = MathUtils::getBitValue(controlState, BIT_LEFT) - MathUtils::getBitValue(controlState, BIT_RIGHT);
	bool _loc6_ = MathUtils::getBitValue(controlState, BIT_REVERSE_TURN) == 1;
	this->tank->setMovementParams(_loc4_, _loc5_, turnSpeedNumber, _loc6_);
	if (_loc3_ != this->appliedControlState)
	{
		this->onAppliedControlStateChanged(this->appliedControlState);
	}
}


void ChassisController::onAppliedControlStateChanged(int param1)
{
	this->listener->onChassisControlChanged(param1, false);
}


int ChassisController::getControlState()
{
   return this->isNotLocked() ? this->controlState : 0;
}
