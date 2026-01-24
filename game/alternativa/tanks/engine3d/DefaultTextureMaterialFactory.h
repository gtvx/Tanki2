#ifndef DEFAULTTEXTUREMATERIALFACTORY_H
#define DEFAULTTEXTUREMATERIALFACTORY_H

#include "TextureMaterialFactory.h"

class DefaultTextureMaterialFactory : public TextureMaterialFactory
{
public:
	std::shared_ptr<TextureMaterial> createTextureMaterial(BitmapData*, bool) override;
};

#endif // DEFAULTTEXTUREMATERIALFACTORY_H
