#ifndef TARGETPRIORITYCALCULATOR_H
#define TARGETPRIORITYCALCULATOR_H

class Tank;

class TargetPriorityCalculator
{
public:
	 virtual double getTargetPriority(Tank *tank, double, double) = 0;
};

#endif // TARGETPRIORITYCALCULATOR_H
