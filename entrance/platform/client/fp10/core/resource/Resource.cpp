#include "Resource.h"

Resource::Resource(ResourceInfo *resourceInfo)
{
	this->resourceInfo = resourceInfo;
}

Long Resource::getId()
{
	return this->resourceInfo->id;
}

