#include "ResourceRegistryImpl.h"
#include "entrance/platform/client/fp10/core/resource/Resource.h"
#include "entrance/alternativa/types/LongHash.h"
#include <QDebug>


ResourceRegistryImpl::ResourceRegistryImpl()
{

}

void ResourceRegistryImpl::registerResource(Resource *resource)
{
	//qDebug() << "ResourceRegistryImpl::registerResource" << resource->get_id().toString();
	resourceById[resource->getId()] = resource;
	this->_resources.append(resource);
}

Resource* ResourceRegistryImpl::getResource(const Long *id)
{
	//qDebug() << "ResourceRegistryImpl::getResource" << id->toString();

	Resource *resource = this->resourceById[*id];

	if (resource == nullptr)
	{
		qDebug() << "error ResourceRegistryImpl::getResource" << id->toString();
	}

	return resource;
}
