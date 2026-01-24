#ifndef TARGETINGDIRECTIONCALCULATOR_H
#define TARGETINGDIRECTIONCALCULATOR_H

#include <QVector>

class TargetingDirection;
class AllGlobalGunParams;

class TargetingDirectionCalculator
{
public:
	 virtual QVector<TargetingDirection*>* getDirections(AllGlobalGunParams*) = 0;
};

#endif // TARGETINGDIRECTIONCALCULATOR_H
