#ifndef BOXBOXCOLLIDER_H
#define BOXBOXCOLLIDER_H

#include "../Collider.h"
#include "game/alternativa/math/Vector3.h"

struct CollisionBox;
struct Vector3;

class BoxBoxCollider : public Collider
{
public:
	BoxBoxCollider(double epsilon);
	bool haveCollision(CollisionShape*, CollisionShape*) override;
	void getContacts(CollisionShape*, CollisionShape*, MyList<ShapeContact*>*contact) override;

private:
	double epsilon;
	double minOverlap;
	Vector3 minOverlapAxis;

	bool registerOverlap(double param1, Vector3 *param2);
	bool testOverlapOnMainAxis(CollisionBox *box_a, CollisionBox *box_b, Vector3 *v1, Vector3 *v2);
	bool testOverlapOnDerivedAxis(CollisionBox *param1, CollisionBox *param2, Vector3 *param3, Vector3 *param4, Vector3 *param5);
};

#endif // BOXBOXCOLLIDER_H
