#ifndef TEXTUREDMESHESCOLLECTION_H
#define TEXTUREDMESHESCOLLECTION_H

#include "TexturedPropsCollection.h"
#include <QString>
#include <QVector>

class PropMesh;
class Mesh;

class TexturedMeshesCollection : public TexturedPropsCollection
{
	QVector<std::shared_ptr<Mesh>> meshes;
	PropMesh *propMesh;
	QString textureName;
	QString excludedMaterialName;

public:

	TexturedMeshesCollection(PropMesh *ropMesh, const QString &name);

	void add(std::shared_ptr<Mesh> mesh, const QString &name = QString());

	TextureByteData* getTextureData();

	void setMaterial(std::shared_ptr<TextureMaterial> textureMaterial) override;
};

#endif // TEXTUREDMESHESCOLLECTION_H
