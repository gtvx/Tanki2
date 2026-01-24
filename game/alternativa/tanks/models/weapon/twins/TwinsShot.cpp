#include "TwinsShot.h"
#include "game/alternativa/tanks/engine3d/AnimatedSprite3D.h"
#include "TwinsShotParams.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/scene3d/BattleScene3D.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/twins/TwinsCC.h"
#include "../weakening/DistanceWeakening.h"
#include "game/alternativa/tanks/utils/EncryptedInt.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "TwinsWeaponCallback.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "../splash/Splash.h"
#include <QRandomGenerator>
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "../../weapons/shell/states/DummyShellStates.h"
#include "TwinsSFXData.h"
#include "game/alternativa/tanks/sfx/StaticObject3DPositionProvider.h"
#include "game/alternativa/tanks/utils/objectpool/ObjectPool.h"
#include "game/alternativa/tanks/sfx/AnimatedSpriteEffect.h"
#include "game/alternativa/tanks/sfx/DecalEffect.h"
#include "MyMath.h"



TwinsShot::TwinsShot(Pool *pool) :
	InelasticShell(pool)
{
	this->sprite = new_AnimatedSprite3D(TwinsShotParams::SPRITE_SIZE, TwinsShotParams::SPRITE_SIZE);
	this->sprite->looped = true;
	this->impactForce = 0;
}


ShellStates* TwinsShot::createShellStates()
{
	return const_cast<DummyShellStates*>(DummyShellStates::INSTANCE);
}


void TwinsShot::init(double impactForce, TwinsCC *twinsCC, TwinsSFXData *sfxData, DistanceWeakening *weakening, TwinsWeaponCallback *callback, Splash *splash)
{
	this->impactForce = impactForce;
	this->twinsInitParams = twinsCC;
	this->weakening = weakening;
	this->sfxData = sfxData;
	this->callback = callback;
	this->splash = splash;
	this->sprite->setAnimationData(sfxData->shotAnimation.get());
	this->sprite->setFrameIndex((this->sprite->getNumFrames() * MyMath::random()));
	this->sprite->alpha = 1;
	this->sprite->rotation = MathUtils::PI2 * MyMath::random();
	//this->lightEffect = AnimatedLightEffect(battleService.getObjectPool().getObject(AnimatedLightEffect));
	//this->lightEffectPositionProvider = ExternalObject3DPositionProvider(battleService.getObjectPool().getObject(ExternalObject3DPositionProvider));
	//this->lightEffect->init(this->lightEffectPositionProvider, sfxData.shellLightingAnimation, AnimatedLightEffect.DEFAULT_MAX_DISTANCE, true);
}


void TwinsShot::addToGame(AllGlobalGunParams *_arg_1, const Vector3 *_arg_2, Body *_arg_3, bool _arg_4, int _arg_5)
{
	InelasticShell::addToGame(_arg_1, _arg_2, _arg_3, _arg_4, _arg_5);
	battleService->getBattleScene3D()->addObject(this->sprite);
	//battleService.addGraphicEffect(this->lightEffect);
}


double TwinsShot::getSpeed()
{
	return this->twinsInitParams->speed;
}


double TwinsShot::getMaxDistance()
{
	return this->weakening->getDistance();
}


void TwinsShot::processHitImpl(Body *body, const Vector3 *_arg_2, const Vector3 *_arg_3, double _arg_4, int _arg_5)
{
	InelasticShell::processHitImpl(body, _arg_2, _arg_3, _arg_4, _arg_5);

	double _local_6 = this->weakening->getImpactCoeff(_arg_4);

	this->createExplosionEffect(_arg_2, _local_6);

	bool _local_7 = false;//this->splash->applySplashForce(_arg_2, _local_6, body);

	if (BattleUtils::isTankBody(body))
	{
		Tank *tank = body->tank;
		tank->applyWeaponHit(_arg_2, _arg_3, (this->impactForce * _local_6));
		if (this->callback)
		{
			this->callback->onTargetHit(getShotId(), body, _arg_2);
		}
	}
	else
	{
		if (this->callback && _local_7)
		{
			this->callback->onStaticHit(getShotId(), _arg_2);
		}
		this->createHitMark(_arg_2);
	}

	this->destroy();
}


void TwinsShot::render(int _arg_1, int _arg_2)
{
	(void)_arg_1;

	double v = (double(_arg_2) / thousandth->getInt());

	this->sprite->update(v);
	this->sprite->x = interpolatedPosition->x;
	this->sprite->y = interpolatedPosition->y;
	this->sprite->z = interpolatedPosition->z;

	this->sprite->rotation = this->sprite->rotation - (3 * v);

	double distance = this->weakening->getDistance() - totalDistance;

	if (distance < TwinsShotParams::ALPHA_FADE_DISTANCE)
	{
		this->sprite->alpha = distance / TwinsShotParams::ALPHA_FADE_DISTANCE;
	}

	//this->lightEffectPositionProvider.setPosition(interpolatedPosition);
}


void TwinsShot::destroy()
{
	InelasticShell::destroy();
	battleService->getBattleScene3D()->removeObject(this->sprite);
	this->twinsInitParams = nullptr;
	this->callback = nullptr;
	shooterBody = nullptr;
	this->weakening = nullptr;
	this->sfxData = nullptr;
	this->sprite->material = nullptr;
	this->sprite->colorTransform = nullptr;
	//this->lightEffect.kill();
	//this->lightEffect = nullptr;
	//this->lightEffectPositionProvider = nullptr;
}


double TwinsShot::getRadius()
{
	return this->twinsInitParams->shellRadius;
}


void TwinsShot::createExplosionEffect(const Vector3 *_arg_1, double _arg_2)
{
	StaticObject3DPositionProvider *_local_3 = battleService->getObjectPool()->getObject<StaticObject3DPositionProvider>();
	_local_3->init(_arg_1, TwinsShotParams::EXPLOSION_OFFSET_TO_CAMERA);
	double _local_4 = (TwinsShotParams::EXPLOSION_SPRITE_SIZE * (1. + _arg_2)) / 2;
	AnimatedSpriteEffect *_local_5 = battleService->getObjectPool()->getObject<AnimatedSpriteEffect>();
	_local_5->init(_local_4, _local_4, this->sfxData->explosionAnimation, MathUtils::PI2 * MyMath::random(), _local_3, 0.5, 0.5);
	battleService->addGraphicEffect(_local_5);
	this->createExplosionLightEffect(_arg_1);
}


void TwinsShot::createExplosionLightEffect(const Vector3 *_arg_1)
{
	(void)_arg_1;
	//var _local_2:AnimatedLightEffect = AnimatedLightEffect(battleService.getObjectPool().getObject(AnimatedLightEffect));
	//var _local_3:StaticObject3DPositionProvider = StaticObject3DPositionProvider(battleService.getObjectPool().getObject(StaticObject3DPositionProvider));
	//_local_3.init(_arg_1, TwinsShotParams.EXPLOSION_OFFSET_TO_CAMERA);
	//_local_2.init(_local_3, this->sfxData.hitLightingAnimation);
	//battleService.addGraphicEffect(_local_2);
}


void TwinsShot::createHitMark(const Vector3 *_arg_1)
{
	DecalEffect *decalEffect = battleService->getObjectPool()->getObject<DecalEffect>();

	decalEffect->init(_arg_1,
					  this->sfxData->hitMarkMaterial,
					  barrelOrigin,
					  TwinsShotParams::HIT_MARK_RADIUS,
					  TwinsShotParams::HIT_MARK_LIFE_TIME,
					  TwinsShotParams::HIT_MARK_FADE_TIME);

	battleService->addGraphicEffect(decalEffect);
}
