#ifndef TEXTURERESOURCESREGISTRY_H
#define TEXTURERESOURCESREGISTRY_H

#include <memory>

class BitmapTextureResource;
class BitmapData;

class TextureResourcesRegistry
{
public:
	TextureResourcesRegistry();
	static void releaseTextureResources();
	static void release(std::shared_ptr<BitmapData> bitmapData);
	static BitmapTextureResource* getTextureResourcePtr(std::shared_ptr<BitmapData> bitmapData, bool mipMapping, bool stretchNotPowerOf2Textures, bool calculateMipMapsUsingGPU);
	static std::shared_ptr<BitmapTextureResource> getTextureResourceS(std::shared_ptr<BitmapData> bitmapData, bool mipMapping, bool stretchNotPowerOf2Textures, bool calculateMipMapsUsingGPU);
};

#endif // TEXTURERESOURCESREGISTRY_H
