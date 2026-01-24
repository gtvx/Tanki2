#ifndef DISTANCEWEAKENING_H
#define DISTANCEWEAKENING_H


class DistanceWeakening
{
	double maximumDamageRadius;
	double minimumDamageRadius;
	double minimumDamagePercent;
	double falloffInterval;

public:

	DistanceWeakening() {}
	DistanceWeakening(double maximumDamageRadius, double minimumDamageRadius, double minimumDamagePercent);

	void init(double maximumDamageRadius, double minimumDamageRadius, double minimumDamagePercent);

	double getImpactCoeff(double param1);
	double getFullDamageDistance();
	double getDistance();
};

#endif // DISTANCEWEAKENING_H
