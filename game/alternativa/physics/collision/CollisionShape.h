#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include <stdint.h>
#include "game/alternativa/math/Matrix4.h"
#include "game/alternativa/physics/collision/types/AABB.h"
#include "game/alternativa/tanks/physics/CollisionGroup.h"

class Body;
class PhysicsMaterial;

class CollisionShape
{
public:

	enum class Type : uint8_t
	{
		BOX = 1,
		SPHERE = 2,
		RECT = 4,
		TRIANGLE = 8,
	};


	CollisionShape(CollisionShape::Type type, int collisionGroup, const PhysicsMaterial *material);

	virtual AABB* calculateAABB();
	void setBody(Body *boby, const Matrix4 *matrix4 = nullptr);
	virtual double raycast(const Vector3*, const Vector3*, double, Vector3*);

	AABB aabb;
	Matrix4 transform;
	Matrix4 *localTransform;
	Body *body;
	const PhysicsMaterial *material;
	int collisionGroup;
	CollisionShape::Type type;
};

#endif // COLLISIONSHAPE_H
