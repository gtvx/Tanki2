#ifndef COLLISIONRECT_H
#define COLLISIONRECT_H

#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/math/Vector3.h"

class CollisionRect : public CollisionShape
{
public:
	CollisionRect(const Vector3*, int, const PhysicsMaterial*);
	Vector3 hs;
	double raycast(const Vector3*, const Vector3*, double, Vector3*) override;
	AABB *calculateAABB() override;

protected:
	CollisionShape *createPrimitive();
};

#endif // COLLISIONRECT_H
