#include "TwinsAmmunition.h"
#include "../WeaponObject.h"
#include "../common/WeaponCommonData.h"
#include "TwinsShot.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/utils/objectpool/ObjectPool.h"
#include "../weakening/DistanceWeakening.h"


BattleService *TwinsAmmunition::battleService;


TwinsAmmunition::TwinsAmmunition(WeaponObject *weaponObject, TwinsCC *twinsCC, TwinsSFXData *sfxData, TwinsWeaponCallback *callback)
{
	this->weakening = new DistanceWeakening;
	weaponObject->distanceWeakening(this->weakening);
	this->impactForce = weaponObject->commonData()->getImpactForce();
	this->twinsInitParams = twinsCC;
	this->sfxData = sfxData;
	this->splash = nullptr; //Splash(weaponObject->getObject()->adapt(Splash));
	this->callback = callback;
}

TwinsShot* TwinsAmmunition::getShot()
{
	TwinsShot *twinsShot = battleService->getObjectPool()->getObject<TwinsShot>();
	twinsShot->init(this->impactForce, this->twinsInitParams, this->sfxData, this->weakening, this->callback, this->splash);
	return twinsShot;
}
