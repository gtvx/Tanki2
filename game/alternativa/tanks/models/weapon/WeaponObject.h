#ifndef WEAPONOBJECT_H
#define WEAPONOBJECT_H

class VerticalAutoAiming;
class WeaponCommonData;
class IWeaponCommonModel;
class DistanceWeakening;
class IShotModel;
class IGameObject;

class WeaponObject
{
public:
	WeaponObject(IGameObject *object);

	VerticalAutoAiming *verticalAutoAiming();
	int getReloadTimeMS();
	WeaponCommonData *commonData();
	IWeaponCommonModel *weaponCommon();
	void distanceWeakening(DistanceWeakening *distanceWeakening);
	bool isLocal();
	void markAsRemote();

protected:
	bool remote;
	IGameObject *object;
};

#endif // WEAPONOBJECT_H
