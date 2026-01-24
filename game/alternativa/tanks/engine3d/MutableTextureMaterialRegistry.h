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
	BitmapData *getTexture(BitmapData *bitmapData, bool _arg_2);

private:
	void onTextureChange();
	void _updateTexture(TextureMaterial *textureMaterial);
};

#endif // MUTABLETEXTUREMATERIALREGISTRY_H
