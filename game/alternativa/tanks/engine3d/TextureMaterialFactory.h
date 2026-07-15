#ifndef TEXTUREMATERIALFACTORY_H
#define TEXTUREMATERIALFACTORY_H

#include <memory>

class TextureMaterial;
class BitmapData;

class TextureMaterialFactory
{
public:
	virtual std::shared_ptr<TextureMaterial> createTextureMaterial(std::shared_ptr<BitmapData>, bool) = 0;
};

#endif // TEXTUREMATERIALFACTORY_H
