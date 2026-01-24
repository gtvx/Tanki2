#ifndef RESOURCEREGISTRY_H
#define RESOURCEREGISTRY_H

class Resource;
class Long;

class ResourceRegistry
{
public:
	virtual void registerResource(Resource*) = 0;
	virtual Resource* getResource(const Long *id) = 0;
};

#endif // RESOURCEREGISTRY_H
