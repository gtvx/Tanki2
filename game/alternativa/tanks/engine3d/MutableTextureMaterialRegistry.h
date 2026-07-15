#ifndef MUTABLETEXTUREMATERIALREGISTRY_H
#define MUTABLETEXTUREMATERIALREGISTRY_H

#include "TextureMaterialRegistryBase.h"

class MutableTextureRegistry;

class MutableTextureMaterialRegistry : public TextureMaterialRegistryBase
{
	MutableTextureRegistry *textureRegistry;

public:
	MutableTextureMaterialRegistry(TextureMaterialFactory *materialFactory, MutableTextureRegistry *textureRegistry);

protected:
	std::shared_ptr<BitmapData> getTexture(std::shared_ptr<BitmapData> bitmapData, bool _arg_2) override;

private:
	void onTextureChange();
	void _updateTexture(TextureMaterial *textureMaterial);
};

#endif // MUTABLETEXTUREMATERIALREGISTRY_H
