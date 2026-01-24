#include "Object3DSModel.h"
#include "entrance/platform/client/fp10/core/registry/ResourceRegistry.h"
#include "game/projects/tanks/client/garage/models/item/object3ds/Object3DSCC.h"
#include "game/projects/tanks/clients/flash/resources/resource/Tanks3DSResource.h"

ResourceRegistry *Object3DSModel::resourceRegistry;


Object3DSModel::Object3DSModel()
{

}

Tanks3DSResource* Object3DSModel::getResource3DS()
{
	return static_cast<Tanks3DSResource*>(resourceRegistry->getResource(&getInitParam()->resourceId));
}
