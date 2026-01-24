#include "CollisionRect.h"
#include "game/alternativa/math/Matrix4.h"


static const double EPSILON = 0.005;


CollisionRect::CollisionRect(const Vector3 *param1, int collisionGroup, const PhysicsMaterial *material) :
	CollisionShape(Type::RECT, collisionGroup, material)
{
	this->hs.copy(param1);
}


double CollisionRect::raycast(const Vector3 *param1, const Vector3 *param2, double param3, Vector3 *param4)
{
	const Matrix4 *_loc5_ = &this->transform;
	double _loc6_ = param1->getX() - _loc5_->m03;
	double _loc7_ = param1->getY() - _loc5_->m13;
	double _loc8_ = param1->getZ() - _loc5_->m23;
	double _loc9_ = _loc5_->m00 * _loc6_ + _loc5_->m10 * _loc7_ + _loc5_->m20 * _loc8_;
	double _loc10_ = _loc5_->m01 * _loc6_ + _loc5_->m11 * _loc7_ + _loc5_->m21 * _loc8_;
	double _loc11_ = _loc5_->m02 * _loc6_ + _loc5_->m12 * _loc7_ + _loc5_->m22 * _loc8_;
	_loc6_ = _loc5_->m00 * param2->getX() + _loc5_->m10 * param2->getY() + _loc5_->m20 * param2->getZ();
	_loc7_ = _loc5_->m01 * param2->getX() + _loc5_->m11 * param2->getY() + _loc5_->m21 * param2->getZ();
	_loc8_ = _loc5_->m02 * param2->getX() + _loc5_->m12 * param2->getY() + _loc5_->m22 * param2->getZ();
	if (_loc8_ > -param3 && _loc8_ < param3)
		return -1;

	double _loc12_ = -_loc11_ / _loc8_;
	if (_loc12_ < 0)
		return -1;

	_loc9_ = _loc9_ + _loc6_ * _loc12_;
	_loc10_ = _loc10_ + _loc7_ * _loc12_;
	_loc11_ = 0;
	if (_loc9_ < -this->hs.getX() - param3 || _loc9_ > this->hs.getX() + param3 || _loc10_ < -this->hs.getY() - param3 || _loc10_ > this->hs.getY() + param3)
		return -1;

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
	return _loc12_;
}


AABB* CollisionRect::calculateAABB()
{
	const Matrix4 *_loc1_ = &transform;
	double _loc2_ = _loc1_->m00 < 0?double(-_loc1_->m00):double(_loc1_->m00);
	double _loc3_ = _loc1_->m01 < 0?double(-_loc1_->m01):double(_loc1_->m01);
	double _loc4_ = _loc1_->m02 < 0?double(-_loc1_->m02):double(_loc1_->m02);
	AABB *_loc5_ = &this->aabb;
	_loc5_->maxX = this->hs.getX() * _loc2_ + this->hs.getY() * _loc3_ + EPSILON * _loc4_;
	_loc5_->minX = -_loc5_->maxX;
	_loc2_ = _loc1_->m10 < 0?double(-_loc1_->m10):double(_loc1_->m10);
	_loc3_ = _loc1_->m11 < 0?double(-_loc1_->m11):double(_loc1_->m11);
	_loc4_ = _loc1_->m12 < 0?double(-_loc1_->m12):double(_loc1_->m12);
	_loc5_->maxY = this->hs.getX() * _loc2_ + this->hs.getY() * _loc3_ + EPSILON * _loc4_;
	_loc5_->minY = -_loc5_->maxY;
	_loc2_ = _loc1_->m20 < 0?double(-_loc1_->m20):double(_loc1_->m20);
	_loc3_ = _loc1_->m21 < 0?double(-_loc1_->m21):double(_loc1_->m21);
	_loc4_ = _loc1_->m22 < 0?double(-_loc1_->m22):double(_loc1_->m22);
	_loc5_->maxZ = this->hs.getX() * _loc2_ + this->hs.getY() * _loc3_ + EPSILON * _loc4_;
	_loc5_->minZ = -_loc5_->maxZ;
	_loc5_->minX = _loc5_->minX + _loc1_->m03;
	_loc5_->maxX = _loc5_->maxX + _loc1_->m03;
	_loc5_->minY = _loc5_->minY + _loc1_->m13;
	_loc5_->maxY = _loc5_->maxY + _loc1_->m13;
	_loc5_->minZ = _loc5_->minZ + _loc1_->m23;
	_loc5_->maxZ = _loc5_->maxZ + _loc1_->m23;
	return _loc5_;
}
