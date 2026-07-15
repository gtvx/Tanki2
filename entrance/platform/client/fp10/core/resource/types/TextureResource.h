#ifndef TEXTURERESOURCE_H
#define TEXTURERESOURCE_H

#include "../Resource.h"
#include <memory>

class BitmapData;

class TextureResource : public Resource
{
public:
	std::shared_ptr<BitmapData> data;

	TextureResource(ResourceInfo *resourceInfo);
};

#endif // TEXTURERESOURCE_H
