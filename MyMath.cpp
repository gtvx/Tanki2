#include <stdint.h>
#include <QRandomGenerator>

namespace MyMath
{
	namespace
	{
		typedef union
		{
			double value;
			uint64_t ull;
			struct { uint32_t low; uint32_t high; };
		} T;
	}

	//Является ли значение числом.
	//bool isFinite(double a) { return std::isfinite(a); }

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

	double random()
	{
		return QRandomGenerator::global()->generateDouble();
	}

	/*
	bool isNaN(double value)
	{
		ulonglong h = Convert(value);
		h &= 0x7fffffffffffffffull;
		h = 0x7ff0000000000000ull - h;
		return (bool)(((ulonglong)h)>>63);
	}
	*/
}
