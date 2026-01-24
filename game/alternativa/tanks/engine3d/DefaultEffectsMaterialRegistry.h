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
	BitmapData* getTexture(BitmapData *bitmapData, bool _arg_2) override;
	std::shared_ptr<AnimatedPaintMaterial> getAnimatedPaint(MultiframeTextureResource*, BitmapData*, BitmapData*, const Long&) override;
	std::shared_ptr<PaintMaterial> getPaint(TextureResource*, BitmapData*, BitmapData*, const Long&) override;
	std::shared_ptr<TextureMaterial> getMaterial(BitmapData*, bool = true) override;
	void addMaterial(std::shared_ptr<TextureMaterial>) override;
	void releaseMaterial(std::shared_ptr<TextureMaterial>) override;
	void setMipMapping(bool) override;
	void clear() override;
};

#endif // DEFAULTEFFECTSMATERIALREGISTRY_H
