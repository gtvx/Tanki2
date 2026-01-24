#ifndef IWEAPONCONTROLLER_H
#define IWEAPONCONTROLLER_H

class IWeaponController
{
public:
	virtual void activateWeapon() = 0;
	virtual void deactivateWeapon() = 0;
	virtual void lockWeapon(int, bool) = 0;
	virtual void unlockWeapon(int) = 0;
};

#endif // IWEAPONCONTROLLER_H
