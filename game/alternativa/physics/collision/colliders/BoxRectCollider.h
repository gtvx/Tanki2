#ifndef BOXRECTCOLLIDER_H
#define BOXRECTCOLLIDER_H

#include "../Collider.h"
#include "game/alternativa/math/Vector3.h"

struct CollisionBox;
struct CollisionRect;

class BoxRectCollider : public Collider
{
public:
	BoxRectCollider(double epsilon);
	bool haveCollision(CollisionShape*, CollisionShape*);
	void getContacts(CollisionShape*, CollisionShape*, MyList<ShapeContact*>*);

private:
	double epsilon;
	double minOverlap;
    Vector3 minOverlapAxis;
	bool testDerivedAxis(CollisionBox*, CollisionRect*, Vector3*, Vector3*, Vector3*);
	bool registerOverlap(double, Vector3*);
	bool testMainAxis(CollisionBox*, CollisionRect*, Vector3*, Vector3*);
};

#endif // BOXRECTCOLLIDER_H
