#ifndef PROPLIBRESOURCE_H
#define PROPLIBRESOURCE_H

#include "entrance/platform/client/fp10/core/resource/Resource.h"

class PropLibrary;
class QByteArray;

class PropLibResource : public Resource
{
public:

	PropLibrary *lib;

	PropLibResource(ResourceInfo *resourceInfo);
	void createLibrary(const QByteArray &data);
};

#endif // PROPLIBRESOURCE_H
