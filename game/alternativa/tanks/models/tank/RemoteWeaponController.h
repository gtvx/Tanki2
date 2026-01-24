#ifndef REMOTEWEAPONCONTROLLER_H
#define REMOTEWEAPONCONTROLLER_H

#include "IWeaponController.h"
class Weapon;

class RemoteWeaponController : public IWeaponController
{
	Weapon *weapon;

public:
	RemoteWeaponController(Weapon*);

	void activateWeapon() override;
	void deactivateWeapon() override;
	void lockWeapon(int, bool) override;
	void unlockWeapon(int) override;
};

#endif // REMOTEWEAPONCONTROLLER_H
