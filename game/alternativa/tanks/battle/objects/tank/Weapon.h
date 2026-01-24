#ifndef WEAPON_H
#define WEAPON_H

class WeaponPlatform;
class ItemProperty;
class Tank;
class SimpleWeaponController;
class DistanceWeakening;

class Weapon
{
public:
	virtual void init(WeaponPlatform*) = 0;
	virtual void destroy() = 0;
	virtual void activate() = 0;
	virtual void deactivate() = 0;
	virtual void enable() = 0;
	virtual void disable(bool) = 0;
	virtual void stun() = 0;
	virtual void calm(int) = 0;
	virtual void reset() = 0;
	virtual double getStatus() = 0;
	virtual ItemProperty* getResistanceProperty() = 0;
	virtual void updateRecoilForce(double) = 0;
	virtual void fullyRecharge() = 0;
	virtual void weaponReloadTimeChanged(int, int) = 0;
	virtual bool check(Tank*) = 0;
	virtual SimpleWeaponController* getController() = 0;
	virtual DistanceWeakening* getWeakening() = 0;
};

#endif // WEAPON_H
