#ifndef TARGETINGPRIORITYCALCULATOR_H
#define TARGETINGPRIORITYCALCULATOR_H

#include <QVector>

class TargetPriorityCalculator;
class RayHit;
class TargetingSector;

class TargetingPriorityCalculator
{
	TargetPriorityCalculator *priorityEvalutor;
	double weakingCoef;

public:
	TargetingPriorityCalculator(TargetPriorityCalculator *priorityEvalutor, double weakingCoef = 0);
	double getPriorityForRayHits(double, QVector<RayHit>*);
	double getPriorityForSectors(double, QVector<TargetingSector*>*);
	double getPriorityForSector(TargetingSector*, double);

private:
	double getPriorityForRayHit(double, const RayHit*);
};

#endif // TARGETINGPRIORITYCALCULATOR_H
