#include "CollisionSphere.h"
#include "game/alternativa/math/Vector3.h"
#include "MyMath.h"
#include "game/alternativa/math/Matrix4.h"


CollisionSphere::CollisionSphere(double param1, int collisionGroup, const PhysicsMaterial *material) :
	CollisionShape(Type::SPHERE, collisionGroup, material)
{
	this->r = param1;
}


double CollisionSphere::raycast(const Vector3 *param1, const Vector3 *param2, double, Vector3*)
{
	const Matrix4 *_loc5_ = &this->transform;
	double _loc6_ = param1->getX() - _loc5_->m03;
	double _loc7_ = param1->getY() - _loc5_->m13;
	double _loc8_ = param1->getZ() - _loc5_->m23;
	double _loc9_ = param2->getX() * _loc6_ + param2->getY() * _loc7_ + param2->getZ() * _loc8_;
	if (_loc9_ > 0)
		return -1;

	double _loc10_ = param2->getX() * param2->getX() + param2->getY() * param2->getY() + param2->getZ() * param2->getZ();
	double _loc11_ = _loc9_ * _loc9_ - _loc10_ * (_loc6_ * _loc6_ + _loc7_ * _loc7_ + _loc8_ * _loc8_ - this->r * this->r);
	if (_loc11_ < 0)
		return -1;

	return -(_loc9_ + MyMath::sqrt(_loc11_)) / _loc10_;
}


AABB* CollisionSphere::calculateAABB()
{
	AABB *aabb = &this->aabb;
	const Matrix4 *transform = &this->transform;
	aabb->maxX = transform->m03 + this->r;
	aabb->minX = transform->m03 - this->r;
	aabb->maxY = transform->m13 + this->r;
	aabb->minY = transform->m13 - this->r;
	aabb->maxZ = transform->m23 + this->r;
	aabb->minZ = transform->m23 - this->r;
	return aabb;
}
