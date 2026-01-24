#ifndef TEXTUREMATERIALREGISTRY_H
#define TEXTUREMATERIALREGISTRY_H

#include <memory>

class AnimatedPaintMaterial;
class MultiframeTextureResource;
class BitmapData;
class Long;
class TextureResource;
class TextureMaterial;
class PaintMaterial;

class TextureMaterialRegistry
{
public:
	virtual std::shared_ptr<AnimatedPaintMaterial> getAnimatedPaint(MultiframeTextureResource*, BitmapData*, BitmapData*, const Long&) = 0;
	virtual std::shared_ptr<PaintMaterial> getPaint(TextureResource*, BitmapData*, BitmapData*, const Long&) = 0;
	virtual std::shared_ptr<TextureMaterial> getMaterial(BitmapData*, bool = true) = 0;
	virtual void addMaterial(std::shared_ptr<TextureMaterial>) = 0;
	virtual void releaseMaterial(std::shared_ptr<TextureMaterial>) = 0;
	virtual void setMipMapping(bool) = 0;
	virtual void clear() = 0;
};

#endif // TEXTUREMATERIALREGISTRY_H
