#include "AABB.h"
#include "_global.h"
#include <QString>

#if 0
AABB::AABB()
{
	this->minX = NaN();
	this->minY = NaN();
	this->minZ = NaN();
	this->maxX = NaN();
	this->maxY = NaN();
	this->maxZ = NaN();
	//infinity();
}
#endif

void AABB::infinity()
{
	this->minX = 1.0e308;
	this->minY = 1.0e308;
	this->minZ = 1.0e308;
	this->maxX = -1.0e308;
	this->maxY = -1.0e308;
	this->maxZ = -1.0e308;
}


void AABB::addBoundBox(const AABB *aabb)
{
	this->minX = (aabb->minX < this->minX) ? aabb->minX : this->minX;
	this->minY = (aabb->minY < this->minY) ? aabb->minY : this->minY;
	this->minZ = (aabb->minZ < this->minZ) ? aabb->minZ : this->minZ;
	this->maxX = (aabb->maxX > this->maxX) ? aabb->maxX : this->maxX;
	this->maxY = (aabb->maxY > this->maxY) ? aabb->maxY : this->maxY;
	this->maxZ = (aabb->maxZ > this->maxZ) ? aabb->maxZ : this->maxZ;
}


bool AABB::intersects(const AABB *aabb, double value)
{
	return !(this->minX > aabb->maxX + value ||
			 this->maxX < aabb->minX - value ||
			 this->minY > aabb->maxY + value ||
			 this->maxY < aabb->minY - value ||
			 this->minZ > aabb->maxZ + value ||
			 this->maxZ < aabb->minZ - value);
}


void AABB::copyFrom(const AABB *aabb)
{
	this->minX = aabb->minX;
	this->minY = aabb->minY;
	this->minZ = aabb->minZ;
	this->maxX = aabb->maxX;
	this->maxY = aabb->maxY;
	this->maxZ = aabb->maxZ;
}


void AABB::addPoint(double x, double y, double z)
{
	if (x < this->minX)
	{
		this->minX = x;
	}

	if (x > this->maxX)
	{
		this->maxX = x;
	}

	if (y < this->minY)
	{
		this->minY = y;
	}

	if (y > this->maxY)
	{
		this->maxY = y;
	}

	if (z < this->minZ)
	{
		this->minZ = z;
	}

	if (z > this->maxZ)
	{
		this->maxZ = z;
	}
}


QString AABB::toString() const
{
	auto number = [](double value) {
		return QString::number(value, 'g', 12);
	};

	return number(this->minX) + " " + number(this->minY) + " " + number(this->minZ) + " " +
			number(this->maxX) + " " + number(this->maxY) + " " + number(this->maxZ);
}
