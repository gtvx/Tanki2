#include "TexturedBSPsCollection.h"
#include "game/alternativa/proplib/objects/PropMesh.h"
#include "game/alternativa/proplib/utils/TextureByteDataMap.h"
#include "hardware/alternativa/engine3d/objects/BSP.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"


TexturedBSPsCollection::TexturedBSPsCollection(PropMesh *propMesh, const QString &name)
{
	this->propMesh = propMesh;
	this->textureName = name;
}

void TexturedBSPsCollection::addBsp(std::shared_ptr<BSP> bsp)
{
	this->bsps.append(bsp);
}

void TexturedBSPsCollection::setMaterial(std::shared_ptr<TextureMaterial> textureMaterial)
{
	for (std::shared_ptr<BSP> item : bsps)
	{
		item->setMaterialToAllFaces(textureMaterial);
	}
}


TextureByteData* TexturedBSPsCollection::getTextureData()
{
	return this->propMesh->textures->getValue(this->textureName);
}
