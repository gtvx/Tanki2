#include "CollisionBox.h"
#include "_global.h"


CollisionBox::CollisionBox(const Vector3 *hs, int collisionGroup, const PhysicsMaterial *material) :
	CollisionShape(Type::BOX, collisionGroup, material)
{
	this->hide = false;
	this->hs.copy(hs);
	this->t = 0;
}


double CollisionBox::raycast(const Vector3 *param1, const Vector3 *param2, double param3, Vector3 *param4)
{
	if (hide == true)
		return -1;

	double _loc8_ = NaN();
	double _loc9_ = NaN();
	const Matrix4 *_loc5_ = &this->transform;
	double _loc6_ = -1;
	double _loc7_ = 1.0e308;
	double _loc10_ = param1->getX() - _loc5_->m03;
	double _loc11_ = param1->getY() - _loc5_->m13;
	double _loc12_ = param1->getZ() - _loc5_->m23;
	double _loc13_ = _loc5_->m00 * _loc10_ + _loc5_->m10 * _loc11_ + _loc5_->m20 * _loc12_;
	double _loc14_ = _loc5_->m01 * _loc10_ + _loc5_->m11 * _loc11_ + _loc5_->m21 * _loc12_;
	double _loc15_ = _loc5_->m02 * _loc10_ + _loc5_->m12 * _loc11_ + _loc5_->m22 * _loc12_;
	_loc10_ = _loc5_->m00 * param2->getX() + _loc5_->m10 * param2->getY() + _loc5_->m20 * param2->getZ();
	_loc11_ = _loc5_->m01 * param2->getX() + _loc5_->m11 * param2->getY() + _loc5_->m21 * param2->getZ();
	_loc12_ = _loc5_->m02 * param2->getX() + _loc5_->m12 * param2->getY() + _loc5_->m22 * param2->getZ();
	if (_loc10_ < param3 && _loc10_ > -param3)
	{
		if (_loc13_ < -this->hs.getX() || _loc13_ > this->hs.getX())
		{
			return -1;
		}
	}
	else
	{
		_loc8_ = (-this->hs.getX() - _loc13_) / _loc10_;
		_loc9_ = (this->hs.getX() - _loc13_) / _loc10_;
		if (_loc8_ < _loc9_)
		{
			if (_loc8_ > _loc6_)
			{
				_loc6_ = _loc8_;
				param4->setX(-1);
				param4->setY(0);
				param4->setZ(0);
			}
			if (_loc9_ < _loc7_)
			{
				_loc7_ = _loc9_;
			}
		}
		else
		{
			if (_loc9_ > _loc6_)
			{
				_loc6_ = _loc9_;
				param4->setX(1);
				param4->setY(0);
				param4->setZ(0);
			}
			if (_loc8_ < _loc7_)
			{
				_loc7_ = _loc8_;
			}
		}
		if (_loc7_ < _loc6_)
		{
			return -1;
		}
	}
	if (_loc11_ < param3 && _loc11_ > -param3)
	{
		if (_loc14_ < -this->hs.getY() || _loc14_ > this->hs.getY())
		{
			return -1;
		}
	}
	else
	{
		_loc8_ = (-this->hs.getY() - _loc14_) / _loc11_;
		_loc9_ = (this->hs.getY() - _loc14_) / _loc11_;
		if (_loc8_ < _loc9_)
		{
			if (_loc8_ > _loc6_)
			{
				_loc6_ = _loc8_;
				param4->setY(-1);
				param4->setX(0);
				param4->setZ(0);
			}
			if (_loc9_ < _loc7_)
			{
				_loc7_ = _loc9_;
			}
		}
		else
		{
			if (_loc9_ > _loc6_)
			{
				_loc6_ = _loc9_;
				param4->setY(1);
				param4->setX(0);
				param4->setZ(0);
			}
			if (_loc8_ < _loc7_)
			{
				_loc7_ = _loc8_;
			}
		}
		if (_loc7_ < _loc6_)
		{
			return -1;
		}
	}
	if (_loc12_ < param3 && _loc12_ > -param3)
	{
		if (_loc15_ < -this->hs.getZ() || _loc15_ > this->hs.getZ())
		{
			return -1;
		}
	}
	else
	{
		_loc8_ = (-this->hs.getZ() - _loc15_) / _loc12_;
		_loc9_ = (this->hs.getZ() - _loc15_) / _loc12_;
		if (_loc8_ < _loc9_)
		{
			if (_loc8_ > _loc6_)
			{
				_loc6_ = _loc8_;
				param4->setZ(-1);
				param4->setX(0);
				param4->setY(0);
			}
			if (_loc9_ < _loc7_)
			{
				_loc7_ = _loc9_;
			}
		}
		else
		{
			if (_loc9_ > _loc6_)
			{
				_loc6_ = _loc9_;
				param4->setZ(1);
				param4->setX(0);
				param4->setY(0);
			}
			if (_loc8_ < _loc7_)
			{
				_loc7_ = _loc8_;
			}
		}
		if (_loc7_ < _loc6_)
		{
			return -1;
		}
	}
	_loc10_ = param4->getX();
	_loc11_ = param4->getY();
	_loc12_ = param4->getZ();
	param4->setX(_loc5_->m00 * _loc10_ + _loc5_->m01 * _loc11_ + _loc5_->m02 * _loc12_);
	param4->setY(_loc5_->m10 * _loc10_ + _loc5_->m11 * _loc11_ + _loc5_->m12 * _loc12_);
	param4->setZ(_loc5_->m20 * _loc10_ + _loc5_->m21 * _loc11_ + _loc5_->m22 * _loc12_);
	return _loc6_;
}


AABB* CollisionBox::calculateAABB()
{
	double _loc3_ = this->transform.m00 < 0 ? double(-this->transform.m00) : double(this->transform.m00);
	double _loc4_ = this->transform.m01 < 0 ? double(-this->transform.m01) : double(this->transform.m01);
	double _loc5_ = this->transform.m02 < 0 ? double(-this->transform.m02) : double(this->transform.m02);
	aabb.maxX = (((this->hs.getX() * _loc3_) + (this->hs.getY() * _loc4_)) + (this->hs.getZ() * _loc5_));
	aabb.minX = -aabb.maxX;
	_loc3_ = this->transform.m10 < 0 ? double(-this->transform.m10) : double(this->transform.m10);
	_loc4_ = this->transform.m11 < 0 ? double(-this->transform.m11) : double(this->transform.m11);
	_loc5_ = this->transform.m12 < 0 ? double(-this->transform.m12) : double(this->transform.m12);
	aabb.maxY = (((this->hs.getX() * _loc3_) + (this->hs.getY() * _loc4_)) + (this->hs.getZ() * _loc5_));
	aabb.minY = -aabb.maxY;
	_loc3_ = this->transform.m20 < 0 ? double(-this->transform.m20) : double(this->transform.m20);
	_loc4_ = this->transform.m21 < 0 ? double(-this->transform.m21) : double(this->transform.m21);
	_loc5_ = this->transform.m22 < 0 ? double(-this->transform.m22) : double(this->transform.m22);
	aabb.maxZ = ((this->hs.getX() * _loc3_) + (this->hs.getY() * _loc4_)) + (this->hs.getZ() * _loc5_);
	aabb.minZ = -aabb.maxZ;
	aabb.minX = aabb.minX + this->transform.m03;
	aabb.maxX = aabb.maxX + this->transform.m03;
	aabb.minY = aabb.minY + this->transform.m13;
	aabb.maxY = aabb.maxY + this->transform.m13;
	aabb.minZ = aabb.minZ + this->transform.m23;
	aabb.maxZ = aabb.maxZ + this->transform.m23;
	return &aabb;
}
