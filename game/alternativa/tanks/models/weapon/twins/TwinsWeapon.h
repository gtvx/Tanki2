#ifndef TWINSWEAPON_H
#define TWINSWEAPON_H

#include "game/alternativa/tanks/battle/LogicUnit.h"
#include "game/alternativa/tanks/battle/objects/tank/Weapon.h"

class WeaponPlatform;
class AllGlobalGunParams;
class Vector3;
class ItemProperty;
class EncryptedInt;
class SimpleWeaponController;
class TargetingSystem;
class IGameObject;
class TwinsCC;
class WeaponObject;
class DistanceWeakening;
class TwinsEffects;
class TwinsWeaponCallback;
class TwinsAmmunition;


//extends BattleRunnerProvider implements Weapon, LogicUnit

class TwinsWeapon :
		public virtual Weapon,
		public virtual LogicUnit
{
	WeaponPlatform *weaponPlatform;
	bool stunned;
	bool enabled;
	int currentBarrel;
	int shotId;
	EncryptedInt *nextTime;
	SimpleWeaponController *controller;
	TargetingSystem *targetingSystem;
	WeaponObject *weaponObject;
	DistanceWeakening *weakening;
	TwinsEffects *effects;
	double stunEnergy;
	double recoilForce;
	TwinsWeaponCallback *callback;
	TwinsAmmunition *ammunition;

public:
	TwinsWeapon(IGameObject *user, IGameObject *twins_object, TwinsCC *twinsCC);

	void init(WeaponPlatform *weaponPlatform) override;
	void destroy() override;
	void activate() override;
	void deactivate() override;
	void enable() override;
	void disable(bool) override;
	void reset() override;
	double getStatus() override;
	void runLogic(int, int) override;
	ItemProperty *getResistanceProperty()  override;
	void updateRecoilForce(double)  override;
	void fullyRecharge();
	void weaponReloadTimeChanged(int, int) override;
	void stun() override;
	void calm(int) override;
	bool check(Tank*) override;
	SimpleWeaponController* getController() override;
	DistanceWeakening* getWeakening() override;

private:
	void shoot(int _arg_1);
	void doRealShot(int _arg_1, AllGlobalGunParams *_arg_2);
	bool barrelCollidesWithStatic(const Vector3 *_arg_1, const Vector3 *_arg_2, double _arg_3);
	void doDummyShot(int _arg_1);
};

#endif // TWINSWEAPON_H
