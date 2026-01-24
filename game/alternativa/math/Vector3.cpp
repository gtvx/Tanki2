#include "Vector3.h"
#include "MyMath.h"
#include "_global.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include <QString>


Vector3* Vector3::ZERO = new Vector3(0,0,0);
Vector3* Vector3::X_AXIS = new Vector3(1,0,0);
Vector3* Vector3::Y_AXIS = new Vector3(0,1,0);
Vector3* Vector3::Z_AXIS = new Vector3(0,0,1);
Vector3* Vector3::DOWN = new Vector3(0,0,-1);



void Vector3::reverse()
{
	this->X = -this->X;
	this->Y = -this->Y;
	this->Z = -this->Z;
}


double Vector3::dot(const Vector3 *v)
{
	return this->X * v->X + this->Y * v->Y + this->Z * v->Z;
}


double Vector3::lengthSqr() const
{
	return this->X * this->X + this->Y * this->Y + this->Z * this->Z;
}


double Vector3::length() const
{
	return MyMath::sqrt((this->X * this->X) + (this->Y * this->Y) + (this->Z * this->Z));
}


void Vector3::setLength(double param1)
{
	double v = this->X * this->X + this->Y * this->Y + this->Z * this->Z;
	if (v == 0) {
		this->X = param1;
	} else {
		double v = param1 / MyMath::sqrt((this->X * this->X) + (this->Y * this->Y) + (this->Z * this->Z));
		this->X = this->X * v;
		this->Y = this->Y * v;
		this->Z = this->Z * v;
	}
}


void Vector3::copy(const Vector3 *param1)
{
	this->X = param1->X;
	this->Y = param1->Y;
	this->Z = param1->Z;
}


Vector3* Vector3::clone() const
{
	return new Vector3(this->X, this->Y, this->Z);
}


void Vector3::cross2(const Vector3 *param1, const Vector3 *param2)
{
	this->X = param1->Y * param2->Z - param1->Z * param2->Y;
	this->Y = param1->Z * param2->X - param1->X * param2->Z;
	this->Z = param1->X * param2->Y - param1->Y * param2->X;
}


void Vector3::normalize()
{
	double v = (this->X * this->X) + (this->Y * this->Y) + (this->Z * this->Z);
	if (v == 0)
	{
		this->X = 0.9;
		this->Y = 0;
		this->Z = 0;
	}
	else
	{
		v = 1. / MyMath::sqrt(v);
		this->X *= v;
		this->Y *= v;
		this->Z *= v;
	}
}


bool Vector3::isFiniteVector() const
{
	return isFinite(this->X) && isFinite(this->Y) && isFinite(this->Z);
}


void Vector3::scale(double value)
{
	this->X = this->X * value;
	this->Y = this->Y * value;
	this->Z = this->Z * value;
}


void Vector3::setLengthAlongDirection(const Vector3 *param1, double param2)
{
	double _loc3_ = this->X * param1->X + this->Y * param1->Y + this->Z * param1->Z;
	double _loc4_ = param2 - _loc3_;
	this->X = this->X + _loc4_ * param1->X;
	this->Y = this->Y + _loc4_ * param1->Y;
	this->Z = this->Z + _loc4_ * param1->Z;
}


void Vector3::add(Vector3 *vector3)
{
	this->X += vector3->X;
	this->Y += vector3->Y;
	this->Z += vector3->Z;
}


void Vector3::cross(const Vector3 *vector3)
{
	double _loc2_ = this->Y * vector3->Z - this->Z * vector3->Y;
	double _loc3_ = this->Z * vector3->X - this->X * vector3->Z;
	double _loc4_ = this->X * vector3->Y - this->Y * vector3->X;
	this->X = _loc2_;
	this->Y = _loc3_;
	this->Z = _loc4_;
}


void Vector3::transform4(const Matrix4 *matrix4)
{
	double _loc2_ = this->X;
	double _loc3_ = this->Y;
	double _loc4_ = this->Z;
	this->X = matrix4->m00 * _loc2_ + matrix4->m01 * _loc3_ + matrix4->m02 * _loc4_ + matrix4->m03;
	this->Y = matrix4->m10 * _loc2_ + matrix4->m11 * _loc3_ + matrix4->m12 * _loc4_ + matrix4->m13;
	this->Z = matrix4->m20 * _loc2_ + matrix4->m21 * _loc3_ + matrix4->m22 * _loc4_ + matrix4->m23;
}


void Vector3::diff(const Vector3 *a, const Vector3 *b)
{
	this->X = a->X - b->X;
	this->Y = a->Y - b->Y;
	this->Z = a->Z - b->Z;
}


void Vector3::subtract(const Vector3 *v)
{
	this->X = this->X - v->X;
	this->Y = this->Y - v->Y;
	this->Z = this->Z - v->Z;
}


void Vector3::transformTransposed3(const Matrix3 *param1)
{
	double _loc2_ = this->getX();
	double _loc3_ = this->getY();
	double _loc4_ = this->getZ();
	this->setX(param1->m00 * _loc2_ + param1->m10 * _loc3_ + param1->m20 * _loc4_);
	this->setY(param1->m01 * _loc2_ + param1->m11 * _loc3_ + param1->m21 * _loc4_);
	this->setZ(param1->m02 * _loc2_ + param1->m12 * _loc3_ + param1->m22 * _loc4_);
}


void Vector3::addScaled(double v, const Vector3 *vector3)
{
	this->X = this->X + v * vector3->getX();
	this->Y = this->Y + v * vector3->getY();
	this->Z = this->Z + v * vector3->getZ();
}


void Vector3::deltaTransform4(const Matrix4 *m)
{
	double x = this->getX();
	double y = this->getY();
	double z = this->getZ();
	this->setX(m->m00 * x + m->m01 * y + m->m02 * z);
	this->setY(m->m10 * x + m->m11 * y + m->m12 * z);
	this->setZ(m->m20 * x + m->m21 * y + m->m22 * z);
}


void Vector3::sum(const Vector3 *a, const Vector3 *b)
{
	this->X = a->X + b->X;
	this->Y = a->Y + b->Y;
	this->Z = a->Z + b->Z;
}


void Vector3::transform3(const Matrix3 *m)
{
	double x = this->X;
	double y = this->Y;
	double z = this->Z;
	this->X = (((m->m00 * x) + (m->m01 * y)) + (m->m02 * z));
	this->Y = (((m->m10 * x) + (m->m11 * y)) + (m->m12 * z));
	this->Z = (((m->m20 * x) + (m->m21 * y)) + (m->m22 * z));
}


QString Vector3::toString() const
{
	return QString::number(this->x, 'g', 12) + " " +
			QString::number(this->y, 'g', 12) + " " +
			QString::number(this->z, 'g', 12);
}


void Vector3::interpolate(double v, const Vector3 *a, const Vector3 *b)
{
	this->x = a->x + (v * (b->x - a->x));
	this->y = a->y + (v * (b->y - a->y));
	this->z = a->z + (v * (b->z - a->z));
}
