#ifndef BOXTRIANGLECOLLIDER_H
#define BOXTRIANGLECOLLIDER_H

#include "../Collider.h"
#include "game/alternativa/math/Vector3.h"

class CollisionBox;
class CollisionTriangle;

class BoxTriangleCollider : public Collider
{
public:
	BoxTriangleCollider(double);
	bool haveCollision(CollisionShape*, CollisionShape*);
	void getContacts(CollisionShape*, CollisionShape*, MyList<ShapeContact*>*);

private:
	bool testOverlapOnMainAxis(CollisionBox*, CollisionTriangle*, Vector3*, Vector3*);
	bool testOverlapOnDerivedAxis(CollisionBox*, CollisionTriangle*, Vector3*, Vector3*, Vector3*);
	void findContacts(CollisionBox*, CollisionTriangle*, Vector3*, MyList<ShapeContact*>*);
	bool registerOverlap(double, Vector3*);
    Vector3 minOverlapAxis;
	double minOverlap;
	double epsilon;
};

#endif // BOXTRIANGLECOLLIDER_H
