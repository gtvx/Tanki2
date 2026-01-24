#ifndef TANKS3DSRESOURCE_H
#define TANKS3DSRESOURCE_H

#include "entrance/platform/client/fp10/core/resource/Resource.h"
#include <QVector>
#include <QHash>
#include <memory>

class Object3D;
class Mesh;
class BitmapData;

class Tanks3DSResource : public Resource
{
public:

	QHash<QString, BitmapData*> textures;

	Tanks3DSResource(ResourceInfo*);
	QVector<std::shared_ptr<Object3D>> objects;
	QVector<std::shared_ptr<Object3D>> parents;
	QVector<std::shared_ptr<Object3D>>* getObjectsByName(const char *regexp, const char* name);
	bool buildResourceData(const QByteArray &data, const QString &name);

private:
	void initMesh(Mesh *mesh);
};

#endif // TANKS3DSRESOURCE_H
