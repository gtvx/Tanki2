#include "TexturedSpritesCollection.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "hardware/alternativa/engine3d/objects/Sprite3D.h"
#include "game/alternativa/proplib/objects/PropSprite.h"
#include "flash/display/BitmapData.h"


TexturedSpritesCollection::TexturedSpritesCollection(PropSprite *propSprite, const QString &name)
{
	this->propSprite = propSprite;
	(void)name;
}


void TexturedSpritesCollection::addSprite3D(std::shared_ptr<Sprite3D> sprite3D)
{
	this->sprites.append(sprite3D);
}


TextureByteData* TexturedSpritesCollection::getTextureData()
{
	return this->propSprite->textureData;
}


void TexturedSpritesCollection::setMaterial(std::shared_ptr<TextureMaterial> textureMaterial)
{
	BitmapData *bitmapData = textureMaterial->texture();

	for (std::shared_ptr<Sprite3D> sprite3D : this->sprites)
	{
		sprite3D->material = textureMaterial;
		double width = sprite3D->width;
		sprite3D->width = width * bitmapData->width();
		sprite3D->height = width * bitmapData->height();
	}
}
