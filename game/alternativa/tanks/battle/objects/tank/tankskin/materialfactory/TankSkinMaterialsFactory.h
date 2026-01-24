#ifndef TANKSKINMATERIALSFACTORY_H
#define TANKSKINMATERIALSFACTORY_H

class TankSkin;
class IColoring;
class SkinMaterials;
class TextureResource;

class TankSkinMaterialsFactory
{
public:
	virtual SkinMaterials* createSkinMaterials(TankSkin*, IColoring*) = 0;
	virtual SkinMaterials* createDeadSkinMaterials(TankSkin*, TextureResource*) = 0;
};

#endif // TANKSKINMATERIALSFACTORY_H
