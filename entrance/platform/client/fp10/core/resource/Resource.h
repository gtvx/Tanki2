#ifndef RESOURCE_H
#define RESOURCE_H

#include "ResourceInfo.h"
#include "entrance/alternativa/types/Long.h"

class Resource
{
public:
	Resource(ResourceInfo *resourceInfo);
	Long getId();

protected:
	ResourceInfo *resourceInfo;
};

#endif // RESOURCE_H
