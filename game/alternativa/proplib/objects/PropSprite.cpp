#include "PropSprite.h"
#include "hardware/alternativa/engine3d/objects/Sprite3D.h"


PropSprite::PropSprite(TextureByteData *textureData, double originX, double originY, double scale) :
	PropObject(PropObjectType::SPRITE)
{
	this->textureData = textureData;
	this->scale = scale;
	std::shared_ptr<Sprite3D> sprite = new_Sprite3D(1, 1);
	sprite->originX = originX;
	sprite->originY = originY;
	object = sprite;
}
