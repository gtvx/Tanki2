#ifndef COLLIDER_H
#define COLLIDER_H

#include "MyList.h"

class CollisionShape;
class ShapeContact;

class Collider
{
public:
	virtual void getContacts(CollisionShape*, CollisionShape*, MyList<ShapeContact*>*) = 0;
	virtual bool haveCollision(CollisionShape*, CollisionShape*) = 0;
};

#endif // COLLIDER_H
