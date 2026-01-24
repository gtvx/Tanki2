#include "TwinsWeapon.h"
#include "game/alternativa/tanks/battle/objects/tank/WeaponPlatform.h"
#include "game/alternativa/tanks/utils/EncryptedIntImpl.h"
#include "../shared/SimpleWeaponController.h"
#include "game/alternativa/tanks/models/weapons/targeting/CommonTargetingSystem.h"
#include "game/alternativa/tanks/models/weapons/targeting/processor/TargetingDirectionProcessor.h"
#include "../weakening/DistanceWeakening.h"
#include "../WeaponObject.h"
#include "game/alternativa/math/Vector3.h"
#include "../AllGlobalGunParams.h"
#include "game/alternativa/tanks/models/weapons/targeting/TargetingResult.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "getBattleRunner.h"
#include "TwinsWeaponCallback.h"
#include "getTimer.h"
#include "../common/WeaponCommonData.h"
#include "TwinsAmmunition.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "ITwinsSFXModel.h"
#include "TwinsShot.h"
#include "game/alternativa/tanks/physics/TanksCollisionDetector.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "TwinsEffects.h"
#include <QDebug>




TwinsWeapon::TwinsWeapon(IGameObject *user, IGameObject *twins_object, TwinsCC *twinsCC)
{
	stunEnergy = 0;
	stunned = false;
	enabled = false;
	currentBarrel = 0;
	recoilForce = 0;
	shotId = 0;

	this->weaponObject = new WeaponObject(twins_object);
	DistanceWeakening *distanceWeakening = new DistanceWeakening;
	this->weaponObject->distanceWeakening(distanceWeakening);

	WeaponCommonData *commonData  = this->weaponObject->commonData();
	TwinsWeaponCallback *callback = twins_object->adapt<TwinsWeaponCallback>().get();


	ITwinsSFXModel *sfxModel = twins_object->adapt<ITwinsSFXModel>().get();

	TwinsAmmunition *ammunition = new TwinsAmmunition(this->weaponObject, twinsCC, sfxModel->getSFXData(), callback);

	TargetingSystem *targetingSystem = new CommonTargetingSystem(user, this->weaponObject, distanceWeakening->getDistance());


	targetingSystem->getProcessor()->setShotFromMuzzle();

	SimpleWeaponController *controller = new SimpleWeaponController();

	this->recoilForce = commonData->getRecoilForce();
	this->controller = controller;
	this->targetingSystem = targetingSystem;
	this->callback = callback;
	this->ammunition = ammunition;
	this->effects = sfxModel->getPlasmaWeaponEffects();
	this->weakening = distanceWeakening;

	nextTime = new EncryptedIntImpl();
}


void TwinsWeapon::init(WeaponPlatform *weaponPlatform)
{
	this->weaponPlatform = weaponPlatform;
	this->controller->init();
}


void TwinsWeapon::destroy()
{
	this->targetingSystem = nullptr;
	this->effects = nullptr;
	this->controller->destroy();
	this->controller = nullptr;
}


void TwinsWeapon::activate()
{
	//qDebug("TwinsWeapon::activate()");
	getBattleRunner()->addLogicUnit(this);
}


void TwinsWeapon::deactivate()
{
	//qDebug("TwinsWeapon::deactivate()");
	getBattleRunner()->removeLogicUnit(this);
}


void TwinsWeapon::enable()
{
	if (!this->enabled)
	{
		this->enabled = true;
		this->controller->discardStoredAction();
	}
}


void TwinsWeapon::disable(bool _arg_1)
{
	(void)_arg_1;
	this->enabled = false;
}


void TwinsWeapon::reset()
{
	this->controller->discardStoredAction();
	this->nextTime->setInt(0);
	this->currentBarrel = 0;
}


double TwinsWeapon::getStatus()
{
	if (this->stunned)
	{
		return this->stunEnergy;
	}

	double _local_1 = (1 - ((this->nextTime->getInt() - getTimer()) / this->weaponObject->getReloadTimeMS()));
	return ((_local_1 > 1) ? 1 : _local_1);
}


void TwinsWeapon::runLogic(int _arg_1, int _arg_2)
{
	(void)_arg_2;

	if (this->enabled)
	{
		if (((this->controller->wasActive()) && (_arg_1 >= this->nextTime->getInt())))
		{
			this->shoot(_arg_1);
		}
	}
	this->controller->discardStoredAction();
}


void TwinsWeapon::shoot(int _arg_1)
{
	AllGlobalGunParams allGunParams;

	this->nextTime->setInt((_arg_1 + this->weaponObject->getReloadTimeMS()));
	this->weaponPlatform->getAllGunParams(&allGunParams, this->currentBarrel);
	this->weaponPlatform->getBody()->addWorldForceScaled(&allGunParams.muzzlePosition, &allGunParams.direction, -this->recoilForce);
	this->weaponPlatform->addDust(1);

	this->effects->createShotEffects(this->weaponPlatform->getTurret3D(), this->weaponPlatform->getLocalMuzzlePosition(this->currentBarrel));

	if (BattleUtils::isTurretAboveGround(this->weaponPlatform->getBody(), &allGunParams))
	{
		this->doRealShot(_arg_1, &allGunParams);
	}
	else
	{
		this->doDummyShot(_arg_1);
	}

	this->currentBarrel = ((this->currentBarrel + 1) % this->weaponPlatform->getNumberOfBarrels());
}


void TwinsWeapon::doRealShot(int _arg_1, AllGlobalGunParams *_arg_2)
{
	Vector3 shotDirection;

	if (this->barrelCollidesWithStatic(&_arg_2->barrelOrigin, &_arg_2->direction, this->weaponPlatform->getBarrelLength(this->currentBarrel)))
	{
		shotDirection.copy(&_arg_2->direction);
	}
	else
	{
		TargetingResult *_local_4 = this->targetingSystem->target(_arg_2);
		shotDirection.copy(_local_4->getDirection());
	}

	TwinsShot *twinsShot = this->ammunition->getShot();
	twinsShot->addToGame(_arg_2, &shotDirection, this->weaponPlatform->getBody(), false, ++shotId);

	this->callback->onShot(_arg_1, twinsShot->getShotId(), this->currentBarrel, &shotDirection);
}


bool TwinsWeapon::barrelCollidesWithStatic(const Vector3 *_arg_1, const Vector3 *_arg_2, double _arg_3)
{
	RayHit rayHit;
	return getBattleRunner()->getCollisionDetector()->raycastStatic(_arg_1, _arg_2, CollisionGroup::STATIC, _arg_3, nullptr, &rayHit);
}


void TwinsWeapon::doDummyShot(int _arg_1)
{
	this->callback->onDummyShot(_arg_1, this->currentBarrel);
}


ItemProperty* TwinsWeapon::getResistanceProperty()
{
	qDebug("used TwinsWeapon::getResistanceProperty");
	//return ItemProperty::TWINS_RESISTANCE;
	return nullptr;
}


void TwinsWeapon::updateRecoilForce(double)
{

}


void TwinsWeapon::fullyRecharge()
{
	this->nextTime->setInt(0);
	this->stunEnergy = 1;
}


void TwinsWeapon::weaponReloadTimeChanged(int _arg_1, int _arg_2)
{
	this->nextTime->setInt((this->nextTime->getInt() + _arg_2) - _arg_1);
}


void TwinsWeapon::stun()
{
	this->stunEnergy = this->getStatus();
	this->stunned = true;
}


void TwinsWeapon::calm(int _arg_1)
{
	this->nextTime->setInt(this->nextTime->getInt() + _arg_1);
	this->stunned = false;
}


bool TwinsWeapon::check(Tank *)
{
	qDebug("used TwinsWeapon::check");
	return false;
}


SimpleWeaponController* TwinsWeapon::getController()
{
	qDebug("used TwinsWeapon::getController");
	return nullptr;
}


DistanceWeakening* TwinsWeapon::getWeakening()
{
	qDebug("used TwinsWeapon::getWeakening");
	return nullptr;
}
