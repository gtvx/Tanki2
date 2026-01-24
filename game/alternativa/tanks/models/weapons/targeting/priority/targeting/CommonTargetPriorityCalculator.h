#ifndef COMMONTARGETPRIORITYCALCULATOR_H
#define COMMONTARGETPRIORITYCALCULATOR_H

#include "TargetPriorityCalculator.h"

class CommonTargetEvaluator;
class WeaponObject;

class CommonTargetPriorityCalculator : public TargetPriorityCalculator
{
	CommonTargetEvaluator *commonTargetEvaluator;
	double maxAngle;
	double fullDamageDistance;

public:
	CommonTargetPriorityCalculator(WeaponObject*);

	double getTargetPriority(Tank *tank, double, double) override;

protected:
	double getFullDamageDistance(WeaponObject*);
};

#endif // COMMONTARGETPRIORITYCALCULATOR_H
