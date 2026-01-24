#ifndef COLLISIONTRIANGLE_H
#define COLLISIONTRIANGLE_H

#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/math/Vector3.h"

class CollisionTriangle : public CollisionShape
{
public:
	CollisionTriangle(const Vector3*, const Vector3*, const Vector3*, int, const PhysicsMaterial*);
	Vector3 v0, v1, v2;
	Vector3 e0, e1, e2;
	double raycast(const Vector3*, const Vector3*, double, Vector3*) override;
	AABB* calculateAABB() override;

private:
	void initVertices(const Vector3*, const Vector3*, const Vector3*);
};

#endif // COLLISIONTRIANGLE_H
