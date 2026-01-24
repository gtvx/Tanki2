#include "RemoteWeaponController.h"
#include "game/alternativa/tanks/battle/objects/tank/Weapon.h"


RemoteWeaponController::RemoteWeaponController(Weapon *weapon)
{
	this->weapon = weapon;
}


void RemoteWeaponController::activateWeapon()
{
	this->weapon->activate();
}


void RemoteWeaponController::deactivateWeapon()
{
	this->weapon->deactivate();
}


void RemoteWeaponController::lockWeapon(int, bool)
{
	this->weapon->disable(false);
}


void RemoteWeaponController::unlockWeapon(int)
{
	this->weapon->enable();
}
