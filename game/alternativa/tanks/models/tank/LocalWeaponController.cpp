#include "LocalWeaponController.h"
#include "game/alternativa/tanks/battle/objects/tank/TankControlLockBits.h"
#include "game/alternativa/tanks/utils/BitMask.h"
#include "game/alternativa/tanks/battle/objects/tank/Weapon.h"
#include <QDebug>


LocalWeaponController::LocalWeaponController(Weapon *weapon)
{
	this->weapon = weapon;
	this->lockMask = new BitMask();
	this->lockMask->setBits(TankControlLockBits::DEAD);
}


void LocalWeaponController::activateWeapon()
{
	this->weapon->activate();
}


void LocalWeaponController::deactivateWeapon()
{
	this->weapon->deactivate();
}


void LocalWeaponController::lockWeapon(int param1, bool param2)
{
	bool _loc3_ = this->lockMask->isEmpty();
	this->lockMask->setBits(param1);
	if (_loc3_)
	{
		this->weapon->disable(param2);
	}
}


void LocalWeaponController::unlockWeapon(int param1)
{
	bool _loc2_ = this->lockMask->isEmpty();
	this->lockMask->clearBits(param1);
	if (!_loc2_ && this->lockMask->isEmpty())
	{
		this->weapon->enable();
	}
}


ItemProperty* LocalWeaponController::getResistanceProperty()
{
	qDebug("used LocalWeaponController::getResistanceProperty");
	return nullptr;
	//return this->weapon->getResistanceProperty();
}
