#ifndef RESOURCESDATA_H
#define RESOURCESDATA_H

class QByteArray;

class ResourcesData
{
public:
	ResourcesData();

	static QByteArray getMap();

	static QByteArray getMapPropLibs();
};

#endif // RESOURCESDATA_H
