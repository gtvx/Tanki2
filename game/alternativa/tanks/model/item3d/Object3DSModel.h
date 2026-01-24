#ifndef OBJECT3DSMODEL_H
#define OBJECT3DSMODEL_H

#include "game/projects/tanks/client/garage/models/item/object3ds/Object3DSModelBase.h"
#include "game/projects/tanks/clients/flash/resources/object3ds/IObject3DS.h"

class ResourceRegistry;
class Tanks3DSResource;

//extends Object3DSModelBase implements IObject3DSModelBase, IObject3DS

class Object3DSModel : public Object3DSModelBase, public IObject3DS
{
public:

	static ResourceRegistry *resourceRegistry;

	Object3DSModel();
	Tanks3DSResource* getResource3DS() override;
};

#endif // OBJECT3DSMODEL_H
