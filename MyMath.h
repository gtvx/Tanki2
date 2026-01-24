#ifndef __MATH_H
#define __MATH_H

namespace MyMath
{
	const double PI = 3.141592653589;
	const double PI2 = 6.28318530717959;
	const double SQRT1_2 = 0.70710678118654752440;
	const double LN2 = 0.6931471805599453;

	static inline double inf()
	{
		return *((double*)"\x00\x00\x00\x00\x00\x00\xF0\x7F");
	}

	static inline double ninf()
	{
		return *((double*)"\x00\x00\x00\x00\x00\x00\xF0\xFF");
	}

	static inline double nan()
	{
		return *((double*)"\x00\x00\x00\x00\x00\x00\xF8\x7F");
	}

	static inline double sin(double v)
	{
		return __builtin_sin(v);
	}

	static inline double cos(double v)
	{
		return __builtin_cos(v);
	}

	static inline double asin(double v)
	{
		return __builtin_asin(v);
	}

	static inline double atan2(double Y, double X)
	{
		return __builtin_atan2(Y, X);
	}

	static inline double sqrt(double v)
	{
		return __builtin_sqrt(v);
	}

	static inline double log(double v)
	{
		return ::__builtin_log(v);
	}

	static inline double pow(double base, double pow)
	{
		return ::__builtin_pow(base, pow);
	}

	static inline double max(double a, double b)
	{
		return (a > b) ? a : b;
	}

	static inline double min(double a, double b)
	{
		return (a < b) ? a : b;
	}

	static inline double abs(double x)
	{
		return __builtin_fabs(x);
	}

	static inline double atan(double a)
	{
		return ::__builtin_atan(a);
	}

	static inline double acos(double a)
	{
		return __builtin_acos(a);
	}

	static inline double tan(double a)
	{
		return __builtin_tan(a);
	}

	static inline double ceil(double a)
	{
		return __builtin_ceil(a);
	}

	static inline double fmod(double num, double denom)
	{
		return __builtin_fmod(num, denom);
	}

	static inline double exp(double a)
	{
		return __builtin_exp(a);
	}

	static inline double floor(double a)
	{
		return __builtin_floor(a);
	}

	static inline double round(double v)
	{
		return __builtin_round(v);
	}

	static inline bool isFinite(double value)
	{
		return __builtin_isfinite(value);
	}

	static inline bool isNaN(double value)
	{
		return __builtin_isnan(value);
	}

	bool isInf(double);

	static inline float f_inf()
	{
		return *((float*)"\x00\x00\x80\x7F");
	}

	static inline float f_ninf()
	{
		return *((float*)"\x00\x00\x80\xFF");
	}

	static inline float f_nan()
	{
		return *((float*)"\x00\x00\xC0\xFF");
	}

	double random();
}

#endif // __MATH_H
