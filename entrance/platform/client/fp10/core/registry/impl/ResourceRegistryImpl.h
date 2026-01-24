#ifndef RESOURCEREGISTRYIMPL_H
#define RESOURCEREGISTRYIMPL_H

#include "../ResourceRegistry.h"
#include <QHash>

class ResourceRegistryImpl : public ResourceRegistry
{
	QVector<Resource*> _resources;
	QHash<Long, Resource*> resourceById;

public:
	ResourceRegistryImpl();

	void registerResource(Resource*) override;
	Resource* getResource(const Long *id) override;

};

#endif // RESOURCEREGISTRYIMPL_H
