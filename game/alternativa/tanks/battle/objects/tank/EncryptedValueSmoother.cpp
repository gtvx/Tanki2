#include "EncryptedValueSmoother.h"
#include "game/alternativa/tanks/utils/EncryptedNumberImpl.h"


EncryptedValueSmoother::EncryptedValueSmoother(double SpeedUp, double SpeedDown, double a, double b)
{
	this->smoothingSpeedUp = SpeedUp;
	this->smoothingSpeedDown = SpeedDown;
	this->targetValue = new EncryptedNumberImpl(a);
	this->currentValue = new EncryptedNumberImpl(b);
}


void EncryptedValueSmoother::reset(double value)
{
   this->currentValue->setNumber(value);
   this->targetValue->setNumber(value);
}


double EncryptedValueSmoother::update(double value)
{
   double _loc2_ = this->currentValue->getNumber();
   double _loc3_ = this->targetValue->getNumber();
   if (_loc2_ < _loc3_)
   {
	  _loc2_ = _loc2_ + this->smoothingSpeedUp * value;
	  if (_loc2_ > _loc3_)
	  {
		 _loc2_ = _loc3_;
	  }
   }
   else if (_loc2_ > _loc3_)
   {
	  _loc2_ = _loc2_ - this->smoothingSpeedDown * value;
	  if (_loc2_ < _loc3_)
	  {
		 _loc2_ = _loc3_;
	  }
   }
   this->currentValue->setNumber(_loc2_);
   return _loc2_;
}


void EncryptedValueSmoother::setTargetValue(double value)
{
   this->targetValue->setNumber(value);
}


double EncryptedValueSmoother::getTargetValue()
{
   return this->targetValue->getNumber();
}
