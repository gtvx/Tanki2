#ifndef TEXTURERESOURCE_H
#define TEXTURERESOURCE_H

#include "../Resource.h"

class BitmapData;

class TextureResource : public Resource
{
public:
	BitmapData *data;

	TextureResource(ResourceInfo *resourceInfo);
};

#endif // TEXTURERESOURCE_H
