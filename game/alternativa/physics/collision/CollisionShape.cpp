#include "CollisionShape.h"
#include "game/alternativa/math/Matrix4.h"


CollisionShape::CollisionShape(CollisionShape::Type type, int collisionGroup, const PhysicsMaterial *_material) :
	material(_material)
{
	this->type = type;
	this->collisionGroup = collisionGroup;
	this->localTransform = nullptr;
	this->body = nullptr;
}


AABB* CollisionShape::calculateAABB()
{
	return &this->aabb;
}


void CollisionShape::setBody(Body *boby, const Matrix4 *matrix4)
{
	if (this->body == boby)
	{
		return;
	}

	this->body = boby;

	if (boby != nullptr)
	{
		if (matrix4 != nullptr)
		{
			if (this->localTransform == nullptr)
			{
				this->localTransform = new Matrix4();
			}
			this->localTransform->copy(matrix4);
		}
		else
		{
			this->localTransform = nullptr;
		}
	}
}


double CollisionShape::raycast(const Vector3*, const Vector3*, double, Vector3*)
{
	return -1;
}
