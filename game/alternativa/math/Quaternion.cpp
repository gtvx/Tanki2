#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix3.h"
#include "MyMath.h"
#include "_global.h"
#include "Matrix4.h"
#include <QString>


void Quaternion::reset()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

void Quaternion::getZAxis(Vector3 *param1) const
{
	double _loc2_ = 2 * this->x * this->x;
	double _loc3_ = 2 * this->y * this->y;
	double _loc4_ = 2 * this->y * this->z;
	double _loc5_ = 2 * this->z * this->x;
	double _loc6_ = 2 * this->w * this->x;
	double _loc7_ = 2 * this->w * this->y;
	param1->setX(_loc5_ + _loc7_);
	param1->setY(_loc4_ - _loc6_);
	param1->setZ(1 - _loc2_ - _loc3_);
}


void Quaternion::getYAxis(Vector3 *_arg_1) const
{
	double _local_2 = ((2 * this->x) * this->x);
	double _local_3 = ((2 * this->z) * this->z);
	double _local_4 = ((2 * this->x) * this->y);
	double _local_5 = ((2 * this->y) * this->z);
	double _local_6 = ((2 * this->w) * this->x);
	double _local_7 = ((2 * this->w) * this->z);
	_arg_1->setX(_local_4 - _local_7);
	_arg_1->setY((1 - _local_2) - _local_3);
	_arg_1->setZ(_local_5 + _local_6);
}


void Quaternion::toMatrix3(Matrix3 *param1) const
{
	double _loc2_ = 2 * this->x * this->x;
	double _loc3_ = 2 * this->y * this->y;
	double _loc4_ = 2 * this->z * this->z;
	double _loc5_ = 2 * this->x * this->y;
	double _loc6_ = 2 * this->y * this->z;
	double _loc7_ = 2 * this->z * this->x;
	double _loc8_ = 2 * this->w * this->x;
	double _loc9_ = 2 * this->w * this->y;
	double _loc10_ = 2 * this->w * this->z;
	param1->m00 = 1 - _loc3_ - _loc4_;
	param1->m01 = _loc5_ - _loc10_;
	param1->m02 = _loc7_ + _loc9_;
	param1->m10 = _loc5_ + _loc10_;
	param1->m11 = 1 - _loc2_ - _loc4_;
	param1->m12 = _loc6_ - _loc8_;
	param1->m20 = _loc7_ - _loc9_;
	param1->m21 = _loc6_ + _loc8_;
	param1->m22 = 1 - _loc2_ - _loc3_;
}


void Quaternion::addScaledVector(const Vector3 *param1, double param2)
{
	double _loc3_ = param1->getX() * param2;
	double _loc4_ = param1->getY() * param2;
	double _loc5_ = param1->getZ() * param2;
	double _loc6_ = -this->x * _loc3_ - this->y * _loc4_ - this->z * _loc5_;
	double _loc7_ = _loc3_ * this->w + _loc4_ * this->z - _loc5_ * this->y;
	double _loc8_ = _loc4_ * this->w + _loc5_ * this->x - _loc3_ * this->z;
	double _loc9_ = _loc5_ * this->w + _loc3_ * this->y - _loc4_ * this->x;
	this->w = this->w + 0.5 * _loc6_;
	this->x = this->x + 0.5 * _loc7_;
	this->y = this->y + 0.5 * _loc8_;
	this->z = this->z + 0.5 * _loc9_;
	double _loc10_ = this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z;
	if (_loc10_ == 0)
	{
		this->w = 1;
	}
	else
	{
		_loc10_ = 1. / MyMath::sqrt(_loc10_);
		this->w = this->w * _loc10_;
		this->x = this->x * _loc10_;
		this->y = this->y * _loc10_;
		this->z = this->z * _loc10_;
	}
}


void Quaternion::copy(const Quaternion *param1)
{
	this->w = param1->w;
	this->x = param1->x;
	this->y = param1->y;
	this->z = param1->z;
}


bool Quaternion::isFiniteQuaternion() const
{
	return isFinite(this->w) && isFinite(this->x) && isFinite(this->y) && isFinite(this->z);
}


void Quaternion::setFromEulerAnglesXYZ(double x, double y, double z)
{
	this->setFromAxisAngleComponents(1, 0, 0, x);
	Quaternion _q;
	_q.setFromAxisAngleComponents(0, 1, 0, y);
	this->append(&_q);
	this->normalize();
	_q.setFromAxisAngleComponents(0, 0, 1, z);
	this->append(&_q);
	this->normalize();
}


void Quaternion::setFromEulerAngles(const Vector3 *v)
{
	this->setFromEulerAnglesXYZ(v->getX(), v->getY(), v->getZ());
}


void Quaternion::normalize()
{
	double v = this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z;
	if (v == 0)
	{
		this->w = 1;
	}
	else
	{
		v = 1. / MyMath::sqrt(v);
		this->w = this->w * v;
		this->x = this->x * v;
		this->y = this->y * v;
		this->z = this->z * v;
	}
}


void Quaternion::toMatrix4(Matrix4 *param1) const
{
	double _loc2_ = (2 * this->x) * this->x;
	double _loc3_ = (2 * this->y) * this->y;
	double _loc4_ = (2 * this->z) * this->z;
	double _loc5_ = (2 * this->x) * this->y;
	double _loc6_ = (2 * this->y) * this->z;
	double _loc7_ = (2 * this->z) * this->x;
	double _loc8_ = (2 * this->w) * this->x;
	double _loc9_ = (2 * this->w) * this->y;
	double _loc10_ = (2 * this->w) * this->z;
	param1->m00 = (1 - _loc3_) - _loc4_;
	param1->m01 = _loc5_ - _loc10_;
	param1->m02 = _loc7_ + _loc9_;
	param1->m10 = _loc5_ + _loc10_;
	param1->m11 = (1 - _loc2_) - _loc4_;
	param1->m12 = _loc6_ - _loc8_;
	param1->m20 = _loc7_ - _loc9_;
	param1->m21 = _loc6_ + _loc8_;
	param1->m22 = (1 - _loc2_) - _loc3_;
}


Vector3* Quaternion::getEulerAngles(Vector3 *param1) const
{
	double _loc2_ = 2 * this->x * this->x;
	double _loc3_ = 2 * this->y * this->y;
	double _loc4_ = 2 * this->z * this->z;
	double _loc5_ = 2 * this->x * this->y;
	double _loc6_ = 2 * this->y * this->z;
	double _loc7_ = 2 * this->z * this->x;
	double _loc8_ = 2 * this->w * this->x;
	double _loc9_ = 2 * this->w * this->y;
	double _loc10_ = 2 * this->w * this->z;
	double _loc11_ = 1 - _loc3_ - _loc4_;
	double _loc12_ = _loc5_ - _loc10_;
	double _loc13_ = _loc5_ + _loc10_;
	double _loc14_ = 1 - _loc2_ - _loc4_;
	double _loc15_ = _loc7_ - _loc9_;
	double _loc16_ = _loc6_ + _loc8_;
	double _loc17_ = 1 - _loc2_ - _loc3_;
	if (-1 < _loc15_ && _loc15_ < 1)
	{
		if (param1 == nullptr)
		{
			param1 = new Vector3(MyMath::atan2(_loc16_, _loc17_), -MyMath::asin(_loc15_), MyMath::atan2(_loc13_, _loc11_));
		}
		else
		{
			param1->setX(MyMath::atan2(_loc16_, _loc17_));
			param1->setY(-MyMath::asin(_loc15_));
			param1->setZ(MyMath::atan2(_loc13_, _loc11_));
		}
	}
	else if (param1 == nullptr)
	{
		param1 = new Vector3(0, 0.5 * (_loc15_ <= -1 ? MyMath::PI : -MyMath::PI), MyMath::atan2(-_loc12_, _loc14_));
	}
	else
	{
		param1->setX(0);
		double y = _loc15_ <= -1 ? MyMath::PI : -MyMath::PI;
		param1->setY(y * 0.5);
		param1->setZ(MyMath::atan2(-_loc12_, _loc14_));
	}
	return param1;
}


void Quaternion::setFromAxisAngleComponents(double param1, double param2, double param3, double param4)
{
	this->w = MyMath::cos(0.5 * param4);
	double v = MyMath::sin(0.5 * param4) / MyMath::sqrt(param1 * param1 + param2 * param2 + param3 * param3);
	this->x = param1 * v;
	this->y = param2 * v;
	this->z = param3 * v;
}


void Quaternion::append(const Quaternion *param1)
{
	double _loc2_ = param1->w * this->w - param1->x * this->x - param1->y * this->y - param1->z * this->z;
	double _loc3_ = param1->w * this->x + param1->x * this->w + param1->y * this->z - param1->z * this->y;
	double _loc4_ = param1->w * this->y + param1->y * this->w + param1->z * this->x - param1->x * this->z;
	double _loc5_ = param1->w * this->z + param1->z * this->w + param1->x * this->y - param1->y * this->x;
	this->w = _loc2_;
	this->x = _loc3_;
	this->y = _loc4_;
	this->z = _loc5_;
}


void Quaternion::slerp(const Quaternion *_arg_1, const Quaternion *_arg_2, double _arg_3)
{
	double _local_4 = 1;
	double _local_5 = ((((_arg_1->w * _arg_2->w) + (_arg_1->x * _arg_2->x)) + (_arg_1->y * _arg_2->y)) + (_arg_1->z * _arg_2->z));
	if (_local_5 < 0)
	{
		_local_5 = -(_local_5);
		_local_4 = -1;
	}
	if ((1 - _local_5) < 0.001)
	{
		double _local_6 = (1 - _arg_3);
		double _local_7 = (_arg_3 * _local_4);
		this->w = ((_arg_1->w * _local_6) + (_arg_2->w * _local_7));
		this->x = ((_arg_1->x * _local_6) + (_arg_2->x * _local_7));
		this->y = ((_arg_1->y * _local_6) + (_arg_2->y * _local_7));
		this->z = ((_arg_1->z * _local_6) + (_arg_2->z * _local_7));
		this->normalize();
	}
	else
	{
		double _local_8 = MyMath::acos(_local_5);
		double _local_9 = MyMath::sin(_local_8);
		double _local_10 = (MyMath::sin(((1 - _arg_3) * _local_8)) / _local_9);
		double _local_11 = ((MyMath::sin((_arg_3 * _local_8)) / _local_9) * _local_4);
		this->w = ((_arg_1->w * _local_10) + (_arg_2->w * _local_11));
		this->x = ((_arg_1->x * _local_10) + (_arg_2->x * _local_11));
		this->y = ((_arg_1->y * _local_10) + (_arg_2->y * _local_11));
		this->z = ((_arg_1->z * _local_10) + (_arg_2->z * _local_11));
	}
}

QString Quaternion::toString() const
{
	auto number = [](double value) {
		return QString::number(value, 'g', 12);
	};

	return number(this->x) + " " + number(this->y) + " " + number(this->z) + " " +number(this->w);
}
