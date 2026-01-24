#include "CollisionTriangle.h"
#include "game/alternativa/math/Vector3.h"


CollisionTriangle::CollisionTriangle(const Vector3 *v0, const Vector3 *v1, const Vector3 *v2,
									 int collisionGroup, const PhysicsMaterial *material) :
	CollisionShape(Type::TRIANGLE, collisionGroup, material)
{
	this->initVertices(v0, v1, v2);
}


void CollisionTriangle::initVertices(const Vector3 *v0, const Vector3 *v1, const Vector3 *v2)
{
	this->v0.copy(v0);
	this->v1.copy(v1);
	this->v2.copy(v2);
	this->e0.diff(v1, v0);
	this->e0.normalize();
	this->e1.diff(v2, v1);
	this->e1.normalize();
	this->e2.diff(v0, v2);
	this->e2.normalize();
}


double CollisionTriangle::raycast(const Vector3 *param1, const Vector3 *param2, double param3, Vector3 *param4)
{
	const Matrix4 *_loc5_ = &this->transform;
	double _loc6_ = param2->getX() * _loc5_->m02 + param2->getY() * _loc5_->m12 + param2->getZ() * _loc5_->m22;
	if (_loc6_ < param3 && _loc6_ > -param3)
	{
		return -1;
	}
	double _loc7_ = param1->getX() - _loc5_->m03;
	double _loc8_ = param1->getY() - _loc5_->m13;
	double _loc9_ = param1->getZ() - _loc5_->m23;
	double _loc10_ = _loc7_ * _loc5_->m02 + _loc8_ * _loc5_->m12 + _loc9_ * _loc5_->m22;
	double _loc11_ = -_loc10_ / _loc6_;
	if (_loc11_ < 0)
	{
		return -1;
	}
	double _loc12_ = _loc7_ * _loc5_->m00 + _loc8_ * _loc5_->m10 + _loc9_ * _loc5_->m20;
	double _loc13_ = _loc7_ * _loc5_->m01 + _loc8_ * _loc5_->m11 + _loc9_ * _loc5_->m21;
	_loc7_ = _loc12_ + _loc11_ * (param2->getX() * _loc5_->m00 + param2->getY() * _loc5_->m10 + param2->getZ() * _loc5_->m20);
	_loc8_ = _loc13_ + _loc11_ * (param2->getX() * _loc5_->m01 + param2->getY() * _loc5_->m11 + param2->getZ() * _loc5_->m21);
	if (this->e0.getX() * (_loc8_ - this->v0.getY()) - this->e0.getY() *
	   (_loc7_ - this->v0.getX()) < 0 || this->e1.getX() * (_loc8_ - this->v1.getY()) - this->e1.getY() *
	   (_loc7_ - this->v1.getX()) < 0 || this->e2.getX() * (_loc8_ - this->v2.getY()) - this->e2.getY() *
	   (_loc7_ - this->v2.getX()) < 0)
	{
		return -1;
	}
	if (param2->getX() * _loc5_->m02 + param2->getY() * _loc5_->m12 + param2->getZ() * _loc5_->m22 > 0)
	{
		param4->setX(-_loc5_->m02);
		param4->setY(-_loc5_->m12);
		param4->setZ(-_loc5_->m22);
	}
	else
	{
		param4->setX(_loc5_->m02);
		param4->setY(_loc5_->m12);
		param4->setZ(_loc5_->m22);
	}
	return _loc11_;
}


AABB* CollisionTriangle::calculateAABB()
{
	AABB *_loc1_ = &this->aabb;
	const Matrix4 *_loc2_ = &this->transform;
	double _loc6_ = 0.005 * _loc2_->m02;
	double _loc7_ = 0.005 * _loc2_->m12;
	double _loc8_ = 0.005 * _loc2_->m22;
	double _loc4_ = this->v0.getX() * _loc2_->m00 + this->v0.getY() * _loc2_->m01;
	_loc1_->minX = _loc1_->maxX = _loc4_ + _loc6_;
	double _loc5_ = _loc4_ - _loc6_;
	if (_loc5_ > _loc1_->maxX)
	{
		_loc1_->maxX = _loc5_;
	}
	else if (_loc5_ < _loc1_->minX)
	{
		_loc1_->minX = _loc5_;
	}
	_loc4_ = this->v0.getX() * _loc2_->m10 + this->v0.getY() * _loc2_->m11;
	_loc1_->minY = _loc1_->maxY = _loc4_ + _loc7_;
	_loc5_ = _loc4_ - _loc7_;
	if (_loc5_ > _loc1_->maxY)
	{
		_loc1_->maxY = _loc5_;
	}
	else if (_loc5_ < _loc1_->minY)
	{
		_loc1_->minY = _loc5_;
	}
	_loc4_ = this->v0.getX() * _loc2_->m20 + this->v0.getY() * _loc2_->m21;
	_loc1_->minZ = _loc1_->maxZ = _loc4_ + _loc8_;
	_loc5_ = _loc4_ - _loc8_;
	if (_loc5_ > _loc1_->maxZ)
	{
		_loc1_->maxZ = _loc5_;
	}
	else if (_loc5_ < _loc1_->minZ)
	{
		_loc1_->minZ = _loc5_;
	}
	_loc4_ = this->v1.getX() * _loc2_->m00 + this->v1.getY() * _loc2_->m01;
	_loc5_ = _loc4_ + _loc6_;
	if (_loc5_ > _loc1_->maxX)
	{
		_loc1_->maxX = _loc5_;
	}
	else if (_loc5_ < _loc1_->minX)
	{
		_loc1_->minX = _loc5_;
	}
	_loc5_ = _loc4_ - _loc6_;
	if (_loc5_ > _loc1_->maxX)
	{
		_loc1_->maxX = _loc5_;
	}
	else if (_loc5_ < _loc1_->minX)
	{
		_loc1_->minX = _loc5_;
	}
	_loc4_ = this->v1.getX() * _loc2_->m10 + this->v1.getY() * _loc2_->m11;
	_loc5_ = _loc4_ + _loc7_;
	if (_loc5_ > _loc1_->maxY)
	{
		_loc1_->maxY = _loc5_;
	}
	else if (_loc5_ < _loc1_->minY)
	{
		_loc1_->minY = _loc5_;
	}
	_loc5_ = _loc4_ - _loc7_;
	if (_loc5_ > _loc1_->maxY)
	{
		_loc1_->maxY = _loc5_;
	}
	else if (_loc5_ < _loc1_->minY)
	{
		_loc1_->minY = _loc5_;
	}
	_loc4_ = this->v1.getX() * _loc2_->m20 + this->v1.getY() * _loc2_->m21;
	_loc5_ = _loc4_ + _loc8_;
	if (_loc5_ > _loc1_->maxZ)
	{
		_loc1_->maxZ = _loc5_;
	}
	else if (_loc5_ < _loc1_->minZ)
	{
		_loc1_->minZ = _loc5_;
	}
	_loc5_ = _loc4_ - _loc8_;
	if (_loc5_ > _loc1_->maxZ)
	{
		_loc1_->maxZ = _loc5_;
	}
	else if (_loc5_ < _loc1_->minZ)
	{
		_loc1_->minZ = _loc5_;
	}
	_loc4_ = this->v2.getX() * _loc2_->m00 + this->v2.getY() * _loc2_->m01;
	_loc5_ = _loc4_ + _loc6_;
	if (_loc5_ > _loc1_->maxX)
	{
		_loc1_->maxX = _loc5_;
	}
	else if (_loc5_ < _loc1_->minX)
	{
		_loc1_->minX = _loc5_;
	}
	_loc5_ = _loc4_ - _loc6_;
	if (_loc5_ > _loc1_->maxX)
	{
		_loc1_->maxX = _loc5_;
	}
	else if (_loc5_ < _loc1_->minX)
	{
		_loc1_->minX = _loc5_;
	}
	_loc4_ = this->v2.getX() * _loc2_->m10 + this->v2.getY() * _loc2_->m11;
	_loc5_ = _loc4_ + _loc7_;
	if (_loc5_ > _loc1_->maxY)
	{
		_loc1_->maxY = _loc5_;
	}
	else if (_loc5_ < _loc1_->minY)
	{
		_loc1_->minY = _loc5_;
	}
	_loc5_ = _loc4_ - _loc7_;
	if (_loc5_ > _loc1_->maxY)
	{
		_loc1_->maxY = _loc5_;
	}
	else if (_loc5_ < _loc1_->minY)
	{
		_loc1_->minY = _loc5_;
	}
	_loc4_ = this->v2.getX() * _loc2_->m20 + this->v2.getY() * _loc2_->m21;
	_loc5_ = _loc4_ + _loc8_;
	if (_loc5_ > _loc1_->maxZ)
	{
		_loc1_->maxZ = _loc5_;
	}
	else if (_loc5_ < _loc1_->minZ)
	{
		_loc1_->minZ = _loc5_;
	}
	_loc5_ = _loc4_ - _loc8_;
	if (_loc5_ > _loc1_->maxZ)
	{
		_loc1_->maxZ = _loc5_;
	}
	else if (_loc5_ < _loc1_->minZ)
	{
		_loc1_->minZ = _loc5_;
	}
	_loc1_->minX = _loc1_->minX + _loc2_->m03;
	_loc1_->maxX = _loc1_->maxX + _loc2_->m03;
	_loc1_->minY = _loc1_->minY + _loc2_->m13;
	_loc1_->maxY = _loc1_->maxY + _loc2_->m13;
	_loc1_->minZ = _loc1_->minZ + _loc2_->m23;
	_loc1_->maxZ = _loc1_->maxZ + _loc2_->m23;
	return _loc1_;
}
