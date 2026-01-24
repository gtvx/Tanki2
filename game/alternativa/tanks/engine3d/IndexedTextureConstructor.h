#ifndef INDEXEDTEXTURECONSTRUCTOR_H
#define INDEXEDTEXTURECONSTRUCTOR_H

#include "game/alternativa/utils/textureutils/TextureConstructor.h"

class IndexedTextureConstructor : public TextureConstructor
{
public:

	int index;

	IndexedTextureConstructor();

	void disposeTexture();
};

#endif // INDEXEDTEXTURECONSTRUCTOR_H
