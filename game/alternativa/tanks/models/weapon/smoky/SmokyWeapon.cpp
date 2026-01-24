#include "SmokyWeapon.h"
#include "game/alternativa/tanks/utils/EncryptedIntImpl.h"
#include "Error.h"
#include "getTimer.h"
#include "MyMath.h"
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "game/alternativa/tanks/battle/objects/tank/WeaponPlatform.h"
#include "game/alternativa/tanks/models/weapon/shared/SimpleWeaponController.h"
#include "game/alternativa/tanks/models/weapon/WeaponObject.h"
#include "game/alternativa/tanks/models/weapon/WeaponForces.h"
#include "getBattleRunner.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/models/weapon/AllGlobalGunParams.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/alternativa/tanks/models/weapon/common/HitInfo.h"
#include "game/alternativa/tanks/models/weapons/targeting/TargetingResult.h"
#include "game/alternativa/tanks/models/weapons/targeting/TargetingSystem.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankSkin.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "game/alternativa/tanks/models/weapon/weakening/DistanceWeakening.h"


SmokyWeapon::SmokyWeapon(WeaponObject *weaponObject,
						 WeaponForces *weaponForces,
						 TargetingSystem *targetingSystem,
						 DistanceWeakening *weakening,
						 ISmokyEffects *effects,
						 SmokyCallback *callback,
						 SimpleWeaponController *controller)
{
	this->nextTime = new EncryptedIntImpl();
	this->weaponForces = weaponForces;
	this->targetingSystem = targetingSystem;
	this->weakening = weakening;
	this->effects = effects;
	this->callback = callback;
	this->controller = controller;
	this->weaponObject = weaponObject;
	this->stunned = false;
	this->enabled = false;
	this->stunEnergy = 0;
	this->bullet_count_max = 20;
}


void SmokyWeapon::init(WeaponPlatform *weaponPlatform)
{
	this->weaponPlatform = weaponPlatform;
	//if (local == true)
	//	this->controller->init();
	this->reset();
}


void SmokyWeapon::destroy()
{
	this->nextTime = nullptr;
	this->weaponForces = nullptr;
	this->targetingSystem = nullptr;
	this->weakening = nullptr;
	this->effects = nullptr;
	this->callback = nullptr;
	this->controller->destroy();
	this->controller = nullptr;
}


void SmokyWeapon::activate()
{
	getBattleRunner()->addLogicUnit(this);
}


void SmokyWeapon::deactivate()
{
	getBattleRunner()->removeLogicUnit(this);
}


void SmokyWeapon::enable()
{
	this->enabled = true;
	this->controller->discardStoredAction();
}


void SmokyWeapon::disable(bool)
{
	this->enabled = false;
}


void SmokyWeapon::stun()
{
	throw Error(158);
}


void SmokyWeapon::calm(int)
{
	throw Error(159);
}


void SmokyWeapon::reset()
{
	this->nextTime->setInt(getTimer());
}


double SmokyWeapon::getStatus()
{
	if (this->stunned)
		return this->stunEnergy;
	double v = (1 - ((this->nextTime->getInt() - getTimer()) / this->weaponObject->getReloadTimeMS()));
	return MathUtils::clamp(v, 0, 1);
}


ItemProperty *SmokyWeapon::getResistanceProperty()
{
	return nullptr;
	//return ItemProperty::SMOKY_RESISTANCE;
}


void SmokyWeapon::updateRecoilForce(double v)
{
	this->weaponForces->setRecoilForce(v);
}


void SmokyWeapon::fullyRecharge()
{
	this->nextTime->setInt(0);
	this->stunEnergy = 1;
}


void SmokyWeapon::weaponReloadTimeChanged(int a, int b)
{
	this->nextTime->setInt((this->nextTime->getInt() + b) - a);
}


void SmokyWeapon::runLogic(int a, int)
{
	if (this->controller->wasActive())
	{
		if ((this->enabled && (a >= this->nextTime->getInt())) && !this->stunned)
		{
			this->shoot(a);
		}
		this->controller->discardStoredAction();
	}
}


bool SmokyWeapon::check(Tank *tank)
{
	AllGlobalGunParams gunParams;

	this->weaponPlatform->getAllGunParams(&gunParams);

	HitInfo hitInfo;
	if ((BattleUtils::isTurretAboveGround(this->weaponPlatform->getBody(), &gunParams)) && this->getTarget(&gunParams, &hitInfo)) {
		if (BattleUtils::isTankBody(hitInfo.body)) {
			if (tank->getHealth() == 0)
				return false;

			return hitInfo.body->tank == tank;
		}
	}

	return false;
}


SimpleWeaponController* SmokyWeapon::getController()
{
	return controller;
}


DistanceWeakening* SmokyWeapon::getWeakening()
{
	return weakening;
}


void SmokyWeapon::shoot(int t)
{
	this->nextTime->setInt(t + this->weaponObject->getReloadTimeMS());

	AllGlobalGunParams gunParams;

	this->weaponPlatform->getAllGunParams(&gunParams);
	//this->weaponPlatform->getBody()->addWorldForceScaled(&gunParams.barrelOrigin, &gunParams.direction, (-this->weaponForces->getRecoilForce()));
	//this->weaponPlatform->addDust();
	//this->effects.createShotEffects(this->weaponPlatform.getLocalMuzzlePosition(), this->weaponPlatform.getTurret3D());

	HitInfo hitInfo;

	if ((BattleUtils::isTurretAboveGround(this->weaponPlatform->getBody(), &gunParams)) && this->getTarget(&gunParams, &hitInfo))
	{
		//this->effects.createExplosionEffects(hitInfo.position);

		if (BattleUtils::isTankBody(hitInfo.body))
		{
			Tank *tank = hitInfo.body->tank;
			if (tank->getHealth() != 0)
			{
				double v = this->weakening->getImpactCoeff(hitInfo.distance) * 1.2;
				tank->applyWeaponHit(&hitInfo.position, &hitInfo.direction, this->weaponForces->getImpactForce() * v);
				//this->callback.onShotTarget(_arg_1, hitInfo.position, hitInfo.body);
			}
		}
		else
		{
			//this->effects.createExplosionMark(gunParams.barrelOrigin, hitInfo.position);
			//this->callback.onShotStatic(_arg_1, hitInfo.position);
		}
	}
	else
	{
		//this->callback.onShot(t);
	}
}


bool SmokyWeapon::getTarget(AllGlobalGunParams *allGlobalGunParams, HitInfo *hitInfo)
{
	TargetingResult *targetingResult = this->targetingSystem->target(allGlobalGunParams);
	hitInfo->setResult(allGlobalGunParams, targetingResult);
	return targetingResult->hasAnyHit();
}

