#ifndef MAPRESOURCE_H
#define MAPRESOURCE_H

#include "entrance/platform/client/fp10/core/resource/Resource.h"
#include <QVector>

class PropLibRegistry;
class QDomElement;
class IResourceLoadingListener;
class ResourceRegistry;

class MapResource : public Resource
{
	QByteArray proplibsData;

public:

	static ResourceRegistry *resourceRegistry;


	PropLibRegistry *libRegistry;

	QVector<Long*> *libIds;

	MapResource(ResourceInfo *resourceInfo);

	void parsePropLibsInfo(const QDomElement &xml);
	QString getDescription();

	//bool loadBytes(const QByteArray &_arg_1, IResourceLoadingListener *listener);

	void load(const QByteArray &data);

private:
	void buildMap();
};

#endif // MAPRESOURCE_H
