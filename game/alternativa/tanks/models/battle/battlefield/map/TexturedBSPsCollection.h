#ifndef TEXTUREDBSPSCOLLECTION_H
#define TEXTUREDBSPSCOLLECTION_H

#include "TexturedPropsCollection.h"
#include <QString>
#include <QVector>

class PropMesh;
class BSP;

class TexturedBSPsCollection : public TexturedPropsCollection
{
	QString textureName;
	PropMesh *propMesh;
	QVector<std::shared_ptr<BSP>> bsps;

public:

	TexturedBSPsCollection(PropMesh *propMesh, const QString &name);

	void addBsp(std::shared_ptr<BSP> bsp);

	void setMaterial(std::shared_ptr<TextureMaterial> textureMaterial);

	TextureByteData* getTextureData();
};

#endif // TEXTUREDBSPSCOLLECTION_H
