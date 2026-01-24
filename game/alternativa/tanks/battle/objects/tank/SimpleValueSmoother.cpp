#include "SimpleValueSmoother.h"


SimpleValueSmoother::SimpleValueSmoother(double smoothingSpeedUp,
										 double smoothingSpeedDown,
										 double targetValue,
										 double currentValue)
{
	this->smoothingSpeedUp = smoothingSpeedUp;
	this->smoothingSpeedDown = smoothingSpeedDown;
	this->targetValue = targetValue;
	this->currentValue = currentValue;
}


void SimpleValueSmoother::reset(double value)
{
	this->currentValue = value;
	this->targetValue = value;
}


double SimpleValueSmoother::update(double value)
{
	if (this->currentValue < this->targetValue)
	{
		this->currentValue = this->currentValue + this->smoothingSpeedUp * value;
		if (this->currentValue > this->targetValue)
		{
			this->currentValue = this->targetValue;
		}
	}
	else if (this->currentValue > this->targetValue)
	{
		this->currentValue = this->currentValue - this->smoothingSpeedDown * value;
		if (this->currentValue < this->targetValue)
		{
			this->currentValue = this->targetValue;
		}
	}
	return this->currentValue;
}


void SimpleValueSmoother::setTargetValue(double value)
{
	this->targetValue = value;
}


double SimpleValueSmoother::getTargetValue()
{
	return this->targetValue;
}
