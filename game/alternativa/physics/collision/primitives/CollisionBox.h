#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/math/Vector3.h"

class CollisionBox : public CollisionShape
{
	Vector3 hs;

public:
	CollisionBox(const Vector3 *hs, int collisionGroup, const PhysicsMaterial *material);
	bool hide;
	int t;

	const Vector3* constHS() const
	{
		return &hs;
	}

	void setHS(const Vector3 *hs)
	{
		this->hs.copy(hs);
	}

	virtual double raycast(const Vector3*, const Vector3*, double, Vector3*) override;
	AABB *calculateAABB() override;
};

#endif // COLLISIONBOX_H
