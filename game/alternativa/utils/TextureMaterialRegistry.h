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
	virtual std::shared_ptr<AnimatedPaintMaterial> getAnimatedPaint(MultiframeTextureResource*, std::shared_ptr<BitmapData>, std::shared_ptr<BitmapData>, const Long&) = 0;
	virtual std::shared_ptr<PaintMaterial> getPaint(TextureResource*, std::shared_ptr<BitmapData>, std::shared_ptr<BitmapData>, const Long&) = 0;
	virtual std::shared_ptr<TextureMaterial> getMaterial(std::shared_ptr<BitmapData>, bool = true) = 0;
	virtual void addMaterial(std::shared_ptr<TextureMaterial>) = 0;
	virtual void releaseMaterial(std::shared_ptr<TextureMaterial>) = 0;
	virtual void setMipMapping(bool) = 0;
	virtual void clear() = 0;
};

#endif // TEXTUREMATERIALREGISTRY_H
