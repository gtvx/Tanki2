#ifndef SMOKYWEAPON_H
#define SMOKYWEAPON_H

#include "game/alternativa/tanks/battle/objects/tank/Weapon.h"
#include "game/alternativa/tanks/battle/LogicUnit.h"
#include "game/alternativa/tanks/utils/EncryptedInt.h"

class WeaponObject;
class WeaponForces;
class TargetingSystem;
class DistanceWeakening;
class ISmokyEffects;
class SmokyCallback;
class SimpleWeaponController;
class AllGlobalGunParams;
class HitInfo;


class SmokyWeapon : public Weapon, public LogicUnit
{
	 EncryptedInt *nextTime;
	 WeaponForces *weaponForces;
	 TargetingSystem *targetingSystem;
	 DistanceWeakening *weakening;
	 ISmokyEffects *effects;
	 SmokyCallback *callback;
	 SimpleWeaponController *controller;
	 WeaponObject *weaponObject;
	 bool stunned;
	 bool enabled;
	 WeaponPlatform *weaponPlatform;
	 double stunEnergy;
	 int bullet_count_max;

public:
	SmokyWeapon(WeaponObject*,
				WeaponForces*,
				TargetingSystem*,
				DistanceWeakening*,
				ISmokyEffects*,
				SmokyCallback*,
				SimpleWeaponController*);

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
	void runLogic(int, int) override;
	bool check(Tank *tank) override;
	SimpleWeaponController* getController() override;
	DistanceWeakening *getWeakening() override;

private:
	void shoot(int);
	bool getTarget(AllGlobalGunParams *allGlobalGunParams, HitInfo *hitInfo);
};

#endif // SMOKYWEAPON_H
