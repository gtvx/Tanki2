#ifndef DEFAULTTANKSKINMATERIALSFACTORY_H
#define DEFAULTTANKSKINMATERIALSFACTORY_H

#include "TankSkinMaterialsFactory.h"

class TextureMaterialRegistry;

class DefaultTankSkinMaterialsFactory : public TankSkinMaterialsFactory
{
public:

	static TextureMaterialRegistry *textureMaterialRegistry;

	DefaultTankSkinMaterialsFactory();

	SkinMaterials* createSkinMaterials(TankSkin*, IColoring*) override;
	SkinMaterials* createDeadSkinMaterials(TankSkin*, TextureResource*) override;
};

#endif // DEFAULTTANKSKINMATERIALSFACTORY_H
