#ifndef PROPMESH_H
#define PROPMESH_H

#include "PropObject.h"
#include <QVector>

class QByteArray;
class QString;
class Object;
class ByteArrayMap;
class TextureByteDataMap;
class Mesh;
class Occluder;


class PropMesh : public PropObject
{
public:

	static const QString DEFAULT_TEXTURE;

	TextureByteDataMap *textures;

	QVector<std::shared_ptr<Occluder>> *occluders;

	PropMesh(const QByteArray &modelData,
			 const QString &objectName,
			 QMap<QString, QString> *textureFiles,
			 ByteArrayMap *files,
			 TextureByteDataMap *imageMap,
			 const QString &name);

private:
	void parseModel(const QByteArray &modelData,
					const QString &objectName,
					QMap<QString, QString> *textureFiles,
					ByteArrayMap *files,
					TextureByteDataMap *imageMap, const QString &name);

	std::shared_ptr<Mesh> processObjects(const QByteArray &modelData, const QString &objectName, const QString &name);
	void addOccluder(std::shared_ptr<Mesh> mesh);
};

#endif // PROPMESH_H
