#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "MyList.h"
class BodyContact;
class Vector3;
class IRayCollisionFilter;
class RayHit;

class CollisionDetector
{
public:
	virtual void getBodyContacts(MyList<BodyContact*>*) = 0;
	virtual bool raycast(const Vector3*, const Vector3*, int, double, IRayCollisionFilter*, RayHit*) = 0;
	virtual bool raycastStatic(const Vector3*, const Vector3*, int, double, IRayCollisionFilter*, RayHit*) = 0;
};

#endif // COLLISIONDETECTOR_H
