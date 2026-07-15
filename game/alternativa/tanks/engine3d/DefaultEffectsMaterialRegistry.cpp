#include "DefaultEffectsMaterialRegistry.h"
#include "DefaultTextureMaterialFactory.h"


DefaultEffectsMaterialRegistry::DefaultEffectsMaterialRegistry() :
	TextureMaterialRegistryBase(new DefaultTextureMaterialFactory())
{

}


std::shared_ptr<BitmapData> DefaultEffectsMaterialRegistry::getTexture(std::shared_ptr<BitmapData> bitmapData, bool _arg_2)
{
	(void)_arg_2;
	return bitmapData;
}


std::shared_ptr<AnimatedPaintMaterial> DefaultEffectsMaterialRegistry::getAnimatedPaint(MultiframeTextureResource *_arg_1,
																						std::shared_ptr<BitmapData> lightMapBitmap,
																						std::shared_ptr<BitmapData> texture,
																						const Long &_arg_4)
{
	return TextureMaterialRegistryBase::getAnimatedPaint(_arg_1, lightMapBitmap, texture, _arg_4);
}


std::shared_ptr<PaintMaterial> DefaultEffectsMaterialRegistry::getPaint(TextureResource *_arg_1,
																		std::shared_ptr<BitmapData> _arg_2,
																		std::shared_ptr<BitmapData> _arg_3,
																		const Long &_arg_4)
{
	return TextureMaterialRegistryBase::getPaint(_arg_1, _arg_2, _arg_3, _arg_4);
}


std::shared_ptr<TextureMaterial> DefaultEffectsMaterialRegistry::getMaterial(std::shared_ptr<BitmapData> bitmap, bool v)
{
	return TextureMaterialRegistryBase::getMaterial(bitmap, v);
}


void DefaultEffectsMaterialRegistry::addMaterial(std::shared_ptr<TextureMaterial> material)
{
	TextureMaterialRegistryBase::addMaterial(material);
}


void DefaultEffectsMaterialRegistry::releaseMaterial(std::shared_ptr<TextureMaterial> material)
{
	TextureMaterialRegistryBase::releaseMaterial(material);
}


void DefaultEffectsMaterialRegistry::setMipMapping(bool v)
{
	TextureMaterialRegistryBase::setMipMapping(v);
}


void DefaultEffectsMaterialRegistry::clear()
{
	TextureMaterialRegistryBase::clear();
}
