#include "MutableTextureMaterialRegistry.h"
#include "MutableTextureRegistry.h"


MutableTextureMaterialRegistry::MutableTextureMaterialRegistry(TextureMaterialFactory *materialFactory, MutableTextureRegistry *textureRegistry) :
	TextureMaterialRegistryBase(materialFactory)
{
	this->textureRegistry = textureRegistry;
	//textureRegistry->addTextureChangeHandler(this->onTextureChange);
}


void MutableTextureMaterialRegistry::onTextureChange()
{
	//forEachMaterial(this->_updateTexture);
}


void MutableTextureMaterialRegistry::_updateTexture(TextureMaterial *textureMaterial)
{
	(void)textureMaterial;
	//MaterialEntry *materialEntry = getEntry(textureMaterial);
	//textureMaterial->texture = this->textureRegistry->getTexture(materialEntry->texture);
}


std::shared_ptr<BitmapData> MutableTextureMaterialRegistry::getTexture(std::shared_ptr<BitmapData> bitmapData, bool _arg_2)
{
	return this->textureRegistry->getTexture(bitmapData, _arg_2);
}

