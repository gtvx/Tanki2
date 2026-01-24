#include "ResourcesData.h"
#include "file.h"
#include "path.h"
#include <QString>


ResourcesData::ResourcesData()
{

}


QByteArray ResourcesData::getMap()
{
	return File::read(path::getResources() + "/MAP/00000000 1DD34325 0000016B AD440CC9/map.xml");
}


QByteArray ResourcesData::getMapPropLibs()
{
	return File::read(path::getResources() + "/MAP/00000000 1DD34325 0000016B AD440CC9/proplibs.xml");
}
