#ifndef PROPSPRITE_H
#define PROPSPRITE_H

#include "PropObject.h"

class TextureByteData;

class PropSprite : public PropObject
{
public:

	TextureByteData *textureData;
	double scale;

	PropSprite(TextureByteData *textureData, double originX = 0.5, double originY = 0.5, double scale = 1);
};

#endif // PROPSPRITE_H
