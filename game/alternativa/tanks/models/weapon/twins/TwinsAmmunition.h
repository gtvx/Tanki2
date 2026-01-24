#ifndef TWINSAMMUNITION_H
#define TWINSAMMUNITION_H

class WeaponObject;
class TwinsCC;
class TwinsSFXData;
class TwinsWeaponCallback;
class TwinsShot;
class DistanceWeakening;
class Splash;
class BattleService;

class TwinsAmmunition
{
	DistanceWeakening *weakening;
	double impactForce;
	TwinsCC *twinsInitParams;
	TwinsSFXData *sfxData;
	TwinsWeaponCallback *callback;
	Splash *splash;

public:

	static BattleService *battleService;

	TwinsAmmunition(WeaponObject *weaponObject, TwinsCC *twinsCC, TwinsSFXData *sfxData, TwinsWeaponCallback *callback = nullptr);
	TwinsShot* getShot();
};

#endif // TWINSAMMUNITION_H
