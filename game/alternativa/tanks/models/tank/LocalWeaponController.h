#ifndef LOCALWEAPONCONTROLLER_H
#define LOCALWEAPONCONTROLLER_H

#include "IWeaponController.h"

class Weapon;
class BitMask;
class ItemProperty;

class LocalWeaponController : public IWeaponController
{
	Weapon *weapon;
	BitMask *lockMask;

public:
	LocalWeaponController(Weapon *weapon);
	void activateWeapon();
	void deactivateWeapon();
	void lockWeapon(int param1, bool param2);
	void unlockWeapon(int param1);
	ItemProperty *getResistanceProperty();
};

#endif // LOCALWEAPONCONTROLLER_H
