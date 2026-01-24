#ifndef MATHUTILS_H
#define MATHUTILS_H


class MathUtils
{
public:
	static const double PI2;

	static bool numbersEqual(double, double, double);
	static double sign(double value);
	static double clamp(double a, double b, double c);
	static double numberSign(double a, double b);
	static int getBitValue(int, int);
	static int changeBitValue(int, int, bool);
	static double clampAngle(double value);
	static double moveValueTowards(double, double, double);
	static double clampAngleDelta(double, double);
	static double toRadians(double);
	static double snap(double, double, double);
};

#endif // MATHUTILS_H
