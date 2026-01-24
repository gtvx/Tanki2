#include "Point.h"
#include "MyMath.h"

double Point::length() const
{
	return MyMath::sqrt((this->x * this->x) + (this->y * this->y));
}
