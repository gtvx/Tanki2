#include <cmath>
#include <stdint.h>

typedef union
{
	double value;
	uint64_t ull;
	struct { uint32_t low; uint32_t high; };
} T;

static inline double Convert(uint64_t ull)
{
	double *value = (double*)&ull;
	return *value;
}

static inline uint64_t Convert(const double &value)
{
	uint64_t *ull = (uint64_t*)&value;
	return *ull;
}

static double double_POSITIVE_INFINITY = Convert((uint64_t)0x7FF0000000000000ull);
static double double_NEGATIVE_INFINITY = Convert((uint64_t)0xFFF0000000000000ull);
static double double_NAN = Convert((uint64_t)0x7FF8000000000000ull);

//Является ли значение числом.
bool isFinite(double a) { return std::isfinite(a); }

//Является ли значение бесконечностью.
//POSITIVE_INFINITY
//NEGATIVE_INFINITY
bool isInf(double value)
{
	const T *t = (const T*)&value;
	uint32_t low = t->low;
	uint32_t high = t->high;
	low |= (high & 0x7fffffff) ^ 0x7ff00000;
	low |= -low;
	return (~(low >> 31) & (high >> 30)) != 0;
}


double POSITIVE_INFINITY() { return double_POSITIVE_INFINITY; } //Infinity
double NEGATIVE_INFINITY() { return double_NEGATIVE_INFINITY; }


double NaN() { return double_NAN; }

bool isNaN(double value)
{
	uint64_t h = Convert(value);
	h &= 0x7fffffffffffffffull;
	h = 0x7ff0000000000000ull - h;
	return (bool)(((uint64_t)h)>>63);
}
