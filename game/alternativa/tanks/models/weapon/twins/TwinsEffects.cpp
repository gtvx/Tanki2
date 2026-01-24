#include "TwinsEffects.h"
#include "game/alternativa/tanks/sfx/PlaneMuzzleFlashEffect.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/utils/objectpool/ObjectPool.h"
#include "TwinsSFXData.h"

const int TwinsEffects::FLASH_SIZE = 120;

static const int FLASH_LIFE_TIME = 50;


TwinsEffects::TwinsEffects(BattleService *battleService, TwinsSFXData *sfxData)
{
	this->battleService = battleService;
	this->sfxData = sfxData;
}


void TwinsEffects::createShotEffects(std::shared_ptr<Object3D> object, const Vector3 *_arg_2)
{
	this->createGraphicEffect(_arg_2, object);
	//this->createMuzzleLightEffect(_arg_2, object);
	//this->createSoundEffect(object);
}


void TwinsEffects::createGraphicEffect(const Vector3 *_arg_1, std::shared_ptr<Object3D> _arg_2)
{
	return;

	PlaneMuzzleFlashEffect *_local_3 = this->battleService->getObjectPool()->getObject<PlaneMuzzleFlashEffect>();

	_local_3->init(_arg_1, _arg_2, this->sfxData->muzzleFlashMaterial, FLASH_LIFE_TIME, FLASH_SIZE, FLASH_SIZE);
	this->battleService->addGraphicEffect(_local_3);
}

#if 0
private function createMuzzleLightEffect(_arg_1:Vector3, _arg_2:Object3D):void
{
	var _local_3:AnimatedLightEffect = AnimatedLightEffect(this.battleService.getObjectPool().getObject(AnimatedLightEffect));
	var _local_4:MuzzlePositionProvider = MuzzlePositionProvider(this.battleService.getObjectPool().getObject(MuzzlePositionProvider));
	_local_4.init(_arg_2, _arg_1);
	_local_3.init(_local_4, this.sfxData.shotLightingAnimation);
	this.battleService.addGraphicEffect(_local_3);
}

private function createSoundEffect(_arg_1:Object3D):void
{
	var _local_2:Number;
	var _local_3:Sound3D;
	if (this.sfxData.shotSound != null)
	{
		_local_2 = 0.8;
		_local_3 = Sound3D.create(this.sfxData.shotSound, _local_2);
		BattleUtils.tmpVector.reset(_arg_1.x, _arg_1.y, _arg_1.z);
		this.battleService.addSound3DEffect(Sound3DEffect.create(BattleUtils.tmpVector, _local_3));
	};
}
#endif
