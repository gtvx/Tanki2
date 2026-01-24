#ifndef COLLISIONSPHERE_H
#define COLLISIONSPHERE_H

#include "game/alternativa/physics/collision/CollisionShape.h"

class CollisionSphere : public CollisionShape
{
public:
	CollisionSphere(double, int, const PhysicsMaterial*);
	double raycast(const Vector3*, const Vector3*, double, Vector3*) override;
	double r;
	AABB *calculateAABB() override;
};

#endif // COLLISIONSPHERE_H
