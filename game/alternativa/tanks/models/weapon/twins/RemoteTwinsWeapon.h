#ifndef REMOTETWINSWEAPON_H
#define REMOTETWINSWEAPON_H

#include "game/alternativa/tanks/battle/objects/tank/Weapon.h"

class TwinsAmmunition;
class TwinsEffects;
class WeaponPlatform;
class IGameObject;
class TwinsCC;

class RemoteTwinsWeapon : public Weapon
{
	double recoilForce;
	TwinsAmmunition *ammunition;
	TwinsEffects *effects;
	WeaponPlatform *weaponPlatform;

public:
	RemoteTwinsWeapon(IGameObject *object, TwinsCC *cc);

	void init(WeaponPlatform *weaponPlatform) override;

	void destroy() override;
	void activate() override;
	void deactivate() override;
	void enable() override;
	void disable(bool) override;
	void stun() override;
	void calm(int) override;
	void reset() override;
	double getStatus() override;
	ItemProperty* getResistanceProperty() override;
	void updateRecoilForce(double) override;
	void fullyRecharge() override;
	void weaponReloadTimeChanged(int, int) override;
	bool check(Tank*) override;
	SimpleWeaponController* getController() override;
	DistanceWeakening* getWeakening() override;
};

#endif // REMOTETWINSWEAPON_H
