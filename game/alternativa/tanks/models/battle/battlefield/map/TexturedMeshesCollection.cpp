#include "TexturedMeshesCollection.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "game/alternativa/proplib/objects/PropMesh.h"
#include "game/alternativa/proplib/utils/TextureByteDataMap.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"


TexturedMeshesCollection::TexturedMeshesCollection(PropMesh *ropMesh, const QString &name)
{
	this->propMesh = ropMesh;
	this->textureName = name;
}

void TexturedMeshesCollection::add(std::shared_ptr<Mesh> mesh, const QString &name)
{
	this->excludedMaterialName = name;
	this->meshes.append(mesh);
}

TextureByteData* TexturedMeshesCollection::getTextureData()
{
	return this->propMesh->textures->getValue(this->textureName);
}

void TexturedMeshesCollection::setMaterial(std::shared_ptr<TextureMaterial> textureMaterial)
{



	for (std::shared_ptr<Mesh> mesh : meshes)
	{
		QVector<Shared<Face>> faces = mesh->getFaces();

		for (Shared<Face> &face : faces)
		{
			if (!this->excludedMaterialName.isNull() && (QString::fromStdString(face->material->name) != this->excludedMaterialName))
			{
				face->material = textureMaterial;
			}
		}

		mesh->removeVertex( mesh->addVertex(0, 0, 0));
	}



}
