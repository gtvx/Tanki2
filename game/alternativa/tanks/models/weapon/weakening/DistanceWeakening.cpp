#include "DistanceWeakening.h"


DistanceWeakening::DistanceWeakening(double maximumDamageRadius,
									 double minimumDamageRadius,
									 double minimumDamagePercent)
{

	this->maximumDamageRadius = maximumDamageRadius;
	this->minimumDamageRadius = minimumDamageRadius;
	this->minimumDamagePercent = minimumDamagePercent;
	this->falloffInterval = minimumDamageRadius - maximumDamageRadius;
}


void DistanceWeakening::init(double maximumDamageRadius, double minimumDamageRadius, double minimumDamagePercent)
{
	this->maximumDamageRadius = maximumDamageRadius;
	this->minimumDamageRadius = minimumDamageRadius;
	this->minimumDamagePercent = minimumDamagePercent;
	this->falloffInterval = minimumDamageRadius - maximumDamageRadius;
}


double DistanceWeakening::getImpactCoeff(double param1)
{
	if (this->falloffInterval <= 0)
	{
		return 1;
	}

	if (param1 <= this->maximumDamageRadius)
	{
		return 1;
	}

	if (param1 >= this->minimumDamageRadius)
	{
		return 0.01 * this->minimumDamagePercent;
	}

	return 0.01 * (this->minimumDamagePercent + (this->minimumDamageRadius - param1) * (100 - this->minimumDamagePercent) / this->falloffInterval);
}


double DistanceWeakening::getFullDamageDistance()
{
	return this->maximumDamageRadius;
}


double DistanceWeakening::getDistance()
{
	return this->minimumDamageRadius;
}
