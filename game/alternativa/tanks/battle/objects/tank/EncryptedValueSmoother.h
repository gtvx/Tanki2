#ifndef ENCRYPTEDVALUESMOOTHER_H
#define ENCRYPTEDVALUESMOOTHER_H

#include "ValueSmoother.h"

class EncryptedNumber;

class EncryptedValueSmoother : public ValueSmoother
{
	EncryptedNumber *currentValue;
	EncryptedNumber *targetValue;
	double smoothingSpeedUp;
	double smoothingSpeedDown;

public:
	EncryptedValueSmoother(double SpeedUp, double SpeedDown, double, double);
	void reset(double value);
	double update(double value);
	void setTargetValue(double value);
	double getTargetValue();
};

#endif // ENCRYPTEDVALUESMOOTHER_H
