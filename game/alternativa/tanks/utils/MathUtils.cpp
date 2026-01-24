#include "MathUtils.h"
#include "MyMath.h"

const double MathUtils::PI2 = 2 * MyMath::PI;


bool MathUtils::numbersEqual(double param1, double param2, double param3)
{
	double _loc4_ = param1 - param2;
	if (_loc4_ < 0)
	{
		return _loc4_ > -param3;
	}
	return _loc4_ < param3;
}


double MathUtils::sign(double value)
{
	if (value < 0)
		return -1;

	if (value > 0)
		return 1;

	return 0;
}


double MathUtils::clamp(double a, double b, double c)
{
	if (a < b)
		return b;

	if (a > c)
		return c;

	return a;
}


double MathUtils::numberSign(double a, double b)
{
	if (a < 0)
		return a < -b ? -1 : 0;

	if (a > 0)
		return a > b ? 1: 0;

	return 0;
}


int MathUtils::getBitValue(int param1, int param2)
{
	return param1 >> param2 & 1;
}


int MathUtils::changeBitValue(int param1, int param2, bool param3)
{
	if (param3)
	{
		return param1 | 1 << param2;
	}
	return param1 & ~(1 << param2);
}


double MathUtils::clampAngle(double value)
{
	value = MyMath::fmod(value, PI2);

	if (value <= -MyMath::PI)
	{
		return PI2 + value;
	}
	if (value > MyMath::PI)
	{
		return value - PI2;
	}
	return value;
}


double MathUtils::moveValueTowards(double param1, double param2, double param3)
{
	if (param1 < param2)
	{
		param1 = param1 + param3;
		return param1 > param2 ? param2: param1;
	}
	if (param1 > param2)
	{
		param1 = param1 - param3;
		return param1 < param2 ? param2: param1;
	}
	return param1;
}


double MathUtils::clampAngleDelta(double param1, double param2)
{
	double _loc3_ = param1 - param2;
	if (_loc3_ <= -MyMath::PI)
	{
		return _loc3_ + PI2;
	}
	if (_loc3_ > MyMath::PI)
	{
		return _loc3_ - PI2;
	}
	return _loc3_;
}


double MathUtils::toRadians(double value)
{
	return (value * MyMath::PI) / 180;
}


double MathUtils::snap(double param1, double param2, double param3)
{
	if (param1 > (param2 - param3) && param1 < (param2 + param3))
		return param2;

	return param1;
}
