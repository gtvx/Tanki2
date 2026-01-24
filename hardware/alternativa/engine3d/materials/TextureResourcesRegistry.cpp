#include "TextureResourcesRegistry.h"
#include "hardware/alternativa/gfx/core/BitmapTextureResource.h"
#include <QHash>

static QHash<BitmapData*, std::shared_ptr<BitmapTextureResource>> texture2Resource;


TextureResourcesRegistry::TextureResourcesRegistry()
{

}


BitmapTextureResource* TextureResourcesRegistry::getTextureResourcePtr(BitmapData *bitmapData, bool _arg_2, bool _arg_3, bool _arg_4)
{
	if (texture2Resource.contains(bitmapData))
	{
		BitmapTextureResource *resource = texture2Resource[bitmapData].get();
		resource->increaseReferencesCount();
		return resource;
	}

	std::shared_ptr<BitmapTextureResource> resource = std::make_shared<BitmapTextureResource>(bitmapData, _arg_2, _arg_3, _arg_4);
	texture2Resource[bitmapData] = resource;
	return resource.get();
}


std::shared_ptr<BitmapTextureResource> TextureResourcesRegistry::getTextureResourceS(BitmapData *bitmapData, bool _arg_2, bool _arg_3, bool _arg_4)
{
	if (texture2Resource.contains(bitmapData))
	{
		std::shared_ptr<BitmapTextureResource> resource = texture2Resource[bitmapData];
		resource->increaseReferencesCount();
		return resource;
	}

	std::shared_ptr<BitmapTextureResource> resource = std::make_shared<BitmapTextureResource>(bitmapData, _arg_2, _arg_3, _arg_4);
	texture2Resource[bitmapData] = resource;
	return resource;
}


void TextureResourcesRegistry::releaseTextureResources()
{
	QHashIterator<BitmapData*, std::shared_ptr<BitmapTextureResource>> i(texture2Resource);

	while (i.hasNext())
	{
		i.value()->forceDispose();
	}
}


void TextureResourcesRegistry::release(BitmapData *bitmapData)
{
	if (texture2Resource.contains(bitmapData))
	{
		texture2Resource.remove(bitmapData);
	}
}
