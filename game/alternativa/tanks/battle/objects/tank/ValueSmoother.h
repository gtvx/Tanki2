#ifndef VALUESMOOTHER_H
#define VALUESMOOTHER_H

class ValueSmoother
{
public:
	virtual void reset(double) = 0;
	virtual double update(double) = 0;
	virtual void setTargetValue(double) = 0;
	virtual double getTargetValue() = 0;
};

#endif // VALUESMOOTHER_H
