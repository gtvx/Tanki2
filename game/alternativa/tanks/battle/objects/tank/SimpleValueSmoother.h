#ifndef SIMPLEVALUESMOOTHER_H
#define SIMPLEVALUESMOOTHER_H

#include "ValueSmoother.h"

class SimpleValueSmoother : public ValueSmoother
{
	double currentValue;
	double targetValue;
	double smoothingSpeedUp;
	double smoothingSpeedDown;

public:
	SimpleValueSmoother(double smoothingSpeedUp, double smoothingSpeedDown, double targetValue, double currentValue);
	void reset(double value);
	double update(double value);
	void setTargetValue(double value);
	double getTargetValue();
};

#endif // SIMPLEVALUESMOOTHER_H
