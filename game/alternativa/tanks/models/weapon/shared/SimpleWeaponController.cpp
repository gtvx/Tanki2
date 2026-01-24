#include "SimpleWeaponController.h"
#include "game/alternativa/tanks/services/battleinput/BattleInputService.h"
#include "game/alternativa/tanks/battle/objects/tank/LocalWeapon.h"


extern BattleInputService *battleInputService;


SimpleWeaponController::SimpleWeaponController()
{
	this->weapon = nullptr;
	this->triggerPulled = false;
	this->wasTriggerPulled = false;
}


void SimpleWeaponController::init()
{
	battleInputService->addGameActionListener(this);
}


void SimpleWeaponController::destroy()
{
	battleInputService->removeGameActionListener(this);
}


void SimpleWeaponController::setWeapon(LocalWeapon *localWeapon)
{
	this->weapon = localWeapon;
}


bool SimpleWeaponController::wasActive()
{
	return this->triggerPulled || this->wasTriggerPulled;
}


bool SimpleWeaponController::isTriggerPulled()
{
	return this->triggerPulled;
}


void SimpleWeaponController::discardStoredAction()
{
	this->wasTriggerPulled = false;
}


void SimpleWeaponController::onGameAction(GameActionEnum e, bool b)
{
	if (e == GameActionEnum::SHOT)
	{
		if (b)
		{
			this->pullTrigger();
		}
		else
		{
			this->releaseTrigger();
		}
	}
}


void SimpleWeaponController::pullTrigger()
{
	if (!this->triggerPulled)
	{
		this->triggerPulled = true;
		this->wasTriggerPulled = true;
		if (this->weapon != nullptr)
		{
			this->weapon->pullTrigger();
		}
	}
}


void SimpleWeaponController::releaseTrigger()
{
	if (this->triggerPulled)
	{
		this->triggerPulled = false;
		if (this->weapon != nullptr)
		{
			this->weapon->releaseTrigger();
		}
	}
}
