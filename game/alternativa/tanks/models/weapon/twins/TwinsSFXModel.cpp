#include "TwinsSFXModel.h"
#include "entrance/platform/client/fp10/core/resource/types/MultiframeTextureResource.h"
#include "game/alternativa/tanks/engine3d/TextureAnimation.h"
#include "TwinsSFXData.h"
#include "game/alternativa/tanks/utils/GraphicsUtils.h"
#include "game/alternativa/tanks/engine3d/DefaultEffectsMaterialRegistry.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "TwinsShotParams.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/sfx/shoot/twins/TwinsShootSFXCC.h"
#include "TwinsEffects.h"
#include "flash/display/BitmapData.h"
#include "entrance/platform/client/fp10/core/resource/types/TextureResource.h"


BattleService *TwinsSFXModel::battleService;
EffectsMaterialRegistry *TwinsSFXModel::materialRegistry = new DefaultEffectsMaterialRegistry();




static std::shared_ptr<TextureMaterial> getMuzzleFlashMaterial(BitmapData *bitmapData)
{
	std::shared_ptr<TextureMaterial> material = TwinsSFXModel::materialRegistry->getMaterial(bitmapData);
	material->resolution = (TwinsEffects::FLASH_SIZE / bitmapData->height());
	return material;
}


TwinsSFXModel::TwinsSFXModel()
{

}


static std::shared_ptr<TextureAnimation> getTextureAnimation(MultiframeTextureResource *resource, double _arg_2)
{
	std::shared_ptr<TextureAnimation> _local_3 = GraphicsUtils::getTextureAnimationFromResource(TwinsSFXModel::materialRegistry, resource);
	_local_3->material->resolution = (_arg_2 / resource->getFrameWidth());
	return _local_3;
}


static std::shared_ptr<TextureAnimation> getPlasmaAnimation(MultiframeTextureResource *resource)
{
	return getTextureAnimation(resource, TwinsShotParams::SPRITE_SIZE);
}


static std::shared_ptr<TextureAnimation> getExplosionAnimation(MultiframeTextureResource *resource)
{
	return getTextureAnimation(resource, TwinsShotParams::EXPLOSION_SPRITE_SIZE);
}


static void releaseMaterials(TwinsSFXData *data)
{
	TwinsSFXModel::materialRegistry->releaseMaterial(data->muzzleFlashMaterial);
	TwinsSFXModel::materialRegistry->releaseMaterial(data->shotAnimation->material);
	TwinsSFXModel::materialRegistry->releaseMaterial(data->explosionAnimation->material);
	TwinsSFXModel::materialRegistry->releaseMaterial(data->hitMarkMaterial);
}


TwinsEffects* TwinsSFXModel::getPlasmaWeaponEffects()
{
	return getData<TwinsEffects>().get();
}


TwinsSFXData* TwinsSFXModel::getSFXData()
{
	return getData<TwinsSFXData>().get();
}


void TwinsSFXModel::objectLoadedPost()
{
	TwinsShootSFXCC *cc = getInitParam();
	TwinsSFXData *data = new TwinsSFXData();
	data->muzzleFlashMaterial = getMuzzleFlashMaterial(cc->muzzleFlashTexture->data);
	data->shotAnimation = getPlasmaAnimation(cc->shotTexture);
	data->explosionAnimation = getExplosionAnimation(cc->explosionTexture);
	data->hitMarkMaterial = materialRegistry->getMaterial(cc->hitMarkTexture->data);

	//data->shotSound = cc.shotSound.sound;

	//LightingSfx *_local_3 = new LightingSfx(getInitParam().lightingSFXEntity);
	//data->shotLightingAnimation = _local_3.createAnimation("shot");
	//data->shellLightingAnimation = _local_3.createAnimation("bullet");
	//data->hitLightingAnimation = _local_3.createAnimation("hit");

	putData<TwinsSFXData>(std::shared_ptr<TwinsSFXData>(data));
	putData<TwinsEffects>(std::make_shared<TwinsEffects>(battleService, data));
}


void TwinsSFXModel::objectUnloaded()
{
	releaseMaterials(this->getSFXData());
}
