#ifndef REMOTESMOKYWEAPON_H
#define REMOTESMOKYWEAPON_H

#include "game/alternativa/tanks/battle/objects/tank/Weapon.h"

class WeaponForces;
class ISmokyEffects;
class WeaponPlatform;

class RemoteSmokyWeapon : public Weapon
{
	WeaponForces *weaponForces;
	ISmokyEffects *effects;
	WeaponPlatform *weaponPlatform;

public:
	RemoteSmokyWeapon(WeaponForces *weaponForces, ISmokyEffects *effects);

	void init(WeaponPlatform*) override;
	void destroy() override;
	void activate() override;
	void deactivate() override;
	void enable() override;
	void disable(bool) override;
	void stun() override;
	void calm(int) override;
	void reset() override;
	double getStatus() override;
	ItemProperty *getResistanceProperty() override;
	void updateRecoilForce(double) override;
	void fullyRecharge() override;
	void weaponReloadTimeChanged(int, int) override;
	bool check(Tank*) override;
	SimpleWeaponController* getController() override;
	DistanceWeakening* getWeakening() override;
};

#endif // REMOTESMOKYWEAPON_H
