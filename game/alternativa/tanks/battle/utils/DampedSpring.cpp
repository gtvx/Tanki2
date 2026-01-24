#include "DampedSpring.h"


DampedSpring::DampedSpring(double param1, double param2, double epsilon)
{
	velocity = 0.0;
	acceleration = 0.0;
	value = 0.0;
	this->a = 2 * param1 * param2;
	this->b = param1 * param1;
	this->epsilon = epsilon;
}


void DampedSpring::reset(double param1, double param2)
{
	this->value = param1;
	this->velocity = param2;
}


void DampedSpring::resetValue(double param1)
{
	this->value = param1;
}


void DampedSpring::update(double param1, double param2)
{
	double _loc3_ = this->value - param2 + param1 * this->velocity;
	this->velocity = this->velocity + param1 * this->acceleration;
	double _loc4_ = this->difference(_loc3_,0,this->epsilon);
	this->acceleration = -this->a * this->velocity - this->b * _loc4_;
	this->value = param2 + _loc3_;
}


double DampedSpring::difference(double param1, double param2, double param3)
{
	double _loc4_ = param1 - param2;
	if (_loc4_ > param3)
	{
		return _loc4_ - param3;
	}
	if (_loc4_ < -param3)
	{
		return _loc4_ + param3;
	}
	return 0;
}
