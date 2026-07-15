#ifndef DEFAULTEFFECTSMATERIALREGISTRY_H
#define DEFAULTEFFECTSMATERIALREGISTRY_H

#include "TextureMaterialRegistryBase.h"
#include "EffectsMaterialRegistry.h"

//extends TextureMaterialRegistryBase implements EffectsMaterialRegistry

class DefaultEffectsMaterialRegistry : public TextureMaterialRegistryBase, public EffectsMaterialRegistry
{
public:
	DefaultEffectsMaterialRegistry();

protected:
	std::shared_ptr<BitmapData> getTexture(std::shared_ptr<BitmapData> bitmapData, bool _arg_2) override;
	std::shared_ptr<AnimatedPaintMaterial> getAnimatedPaint(MultiframeTextureResource*, std::shared_ptr<BitmapData>, std::shared_ptr<BitmapData>, const Long&) override;
	std::shared_ptr<PaintMaterial> getPaint(TextureResource*, std::shared_ptr<BitmapData>, std::shared_ptr<BitmapData>, const Long&) override;
	std::shared_ptr<TextureMaterial> getMaterial(std::shared_ptr<BitmapData>, bool = true) override;
	void addMaterial(std::shared_ptr<TextureMaterial>) override;
	void releaseMaterial(std::shared_ptr<TextureMaterial>) override;
	void setMipMapping(bool) override;
	void clear() override;
};

#endif // DEFAULTEFFECTSMATERIALREGISTRY_H
