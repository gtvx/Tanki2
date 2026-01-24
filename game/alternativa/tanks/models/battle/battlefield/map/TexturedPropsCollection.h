#ifndef TEXTUREDPROPSCOLLECTION_H
#define TEXTUREDPROPSCOLLECTION_H

#include <memory>

class TextureByteData;
class TextureMaterial;

class TexturedPropsCollection
{
public:
	virtual TextureByteData* getTextureData() = 0;
	virtual void setMaterial(std::shared_ptr<TextureMaterial>) = 0;
};

#endif // TEXTUREDPROPSCOLLECTION_H
