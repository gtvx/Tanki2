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
	static void release(BitmapData *bitmapData);
	static BitmapTextureResource* getTextureResourcePtr(BitmapData *bitmapData, bool _arg_2, bool _arg_3, bool _arg_4);
	static std::shared_ptr<BitmapTextureResource> getTextureResourceS(BitmapData *bitmapData, bool _arg_2, bool _arg_3, bool _arg_4);
};

#endif // TEXTURERESOURCESREGISTRY_H
