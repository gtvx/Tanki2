#include "BarrelElevator.h"
#include "game/alternativa/tanks/battle/objects/tank/SimpleValueSmoother.h"
#include "game/alternativa/tanks/utils/BitMask.h"
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "MyMath.h"


BarrelElevator::BarrelElevator(double defaultElevation, double minElevation, double maxElevation, double maxTurnSpeed, double turnAcceleration)
{
	this->maxTurnSpeed = 0;
	this->turnSpeed = 0;
	this->locks = new BitMask();
	this->userControl = 0;
	this->realControl = 0;
	this->elevationDirection = 0;

	this->maxTurnSpeedSmoother = new SimpleValueSmoother(0.3, 10, 0, 0);
	this->defaultElevation = defaultElevation;
	this->minElevation = minElevation;
	this->maxElevation = maxElevation;
	this->prevElevation = defaultElevation;
	this->currElevation = defaultElevation;
	this->interpolatedElevation = defaultElevation;
	this->setMaxTurnSpeed(maxTurnSpeed, true);
	this->turnAcceleration = turnAcceleration;
}


int BarrelElevator::getRealControl()
{
	return this->realControl;
}


double BarrelElevator::getBarrelPhysicsElevation()
{
	return this->currElevation;
}


void BarrelElevator::reset()
{
	this->prevElevation = this->defaultElevation;
	this->currElevation = this->defaultElevation;
	this->interpolatedElevation = this->defaultElevation;
}


void BarrelElevator::setUserControl(int param1)
{
	this->userControl = param1;
	if (this->isNotLocked())
	{
		this->setRealControl(param1);
	}
}


void BarrelElevator::rotate(double param1, Matrix3*)
{
	this->maxTurnSpeed = this->maxTurnSpeedSmoother->update(param1);
	this->prevElevation = this->currElevation;
	if (this->isLocked())
	{
		this->turnSpeed = 0;
		this->elevationDirection = 0;
		return;
	}
	switch(this->realControl)
	{
	case STOP:
		this->stop();
		break;
	case UP:
		this->rotateInDirection(1, param1);
		break;
	case DOWN:
		this->rotateInDirection(-1, param1);
		break;
	case CENTER:
		this->center(param1);
	}
}


void BarrelElevator::stop()
{
	this->elevationDirection = 0;
	this->turnSpeed = 0;
}


void BarrelElevator::rotateInDirection(int param1, double param2)
{
	if (this->elevationDirection != param1)
	{
		this->elevationDirection = param1;
		this->turnSpeed = 0;
	}
	this->currElevation = this->currElevation + this->turnSpeed * param2;
	this->turnSpeed = MathUtils::moveValueTowards(this->turnSpeed,param1 * this->maxTurnSpeed, this->turnAcceleration * param2);
	if (this->currElevation < this->minElevation || this->currElevation > this->maxElevation)
	{
		this->currElevation = MathUtils::clamp(this->currElevation, this->minElevation, this->maxElevation);
		this->elevationDirection = 0;
		this->turnSpeed = 0;
	}
}


void BarrelElevator::center(double param1)
{
	if (this->currElevation == this->defaultElevation)
	{
		this->elevationDirection = 0;
		this->turnSpeed = 0;
	}
	else
	{
		this->elevationDirection = ((this->currElevation < this->defaultElevation) ? 1 : -1);
		this->currElevation = MathUtils::moveValueTowards(this->currElevation,this->defaultElevation, MyMath::abs(this->turnSpeed * param1));
		this->turnSpeed = MathUtils::moveValueTowards(this->turnSpeed,this->elevationDirection * this->maxTurnSpeed,this->turnAcceleration * param1);
	}
}


void BarrelElevator::interpolate(double param1, int)
{
	this->interpolatedElevation = (this->prevElevation + (param1 * (this->currElevation - this->prevElevation)));
}


bool BarrelElevator::isLocked()
{
	return this->locks->isNotEmpty();
}


bool BarrelElevator::isNotLocked()
{
	return this->locks->isEmpty();
}


bool BarrelElevator::isRotating()
{
	return this->elevationDirection != 0;
}


double BarrelElevator::getBarrelInterpolatedElevation()
{
	return this->interpolatedElevation;
}


void BarrelElevator::setBarrelElevation(double param1)
{
	this->currElevation = param1;
}


void BarrelElevator::setMaxTurnSpeed(double param1, bool param2)
{
	if (param2)
	{
		this->maxTurnSpeed = param1;
		this->maxTurnSpeedSmoother->reset(param1);
	}
	else
	{
		this->maxTurnSpeedSmoother->setTargetValue(param1);
	}
}


double BarrelElevator::getTurnAcceleration()
{
	return this->turnAcceleration;
}


void BarrelElevator::setTurnAcceleration(double param1)
{
	this->turnAcceleration = param1;
}


void BarrelElevator::lock(int param1)
{
	bool _loc2_ = this->isNotLocked();
	this->locks->setBits(param1);
	if (_loc2_ && this->isLocked())
	{
		this->setRealControl(STOP);
	}
}


void BarrelElevator::unlock(int param1)
{
	bool _loc2_ = this->isLocked();
	this->locks->clearBits(param1);
	if (_loc2_ && this->isNotLocked())
	{
		this->setRealControl(this->userControl);
	}
}


void BarrelElevator::setRealControl(int param1)
{
	this->realControl = param1;
}


double BarrelElevator::getTurretInterpolatedDirection()
{
	return 0;
}


double BarrelElevator::getTurretPhysicsDirection()
{
	return 0;
}


void BarrelElevator::updatePhysics(Body*)
{

}


void BarrelElevator::setTurretPhysicsDirection(double)
{

}


TurretControlType BarrelElevator::getTurretRealControlType()
{
	return TurretControlType::ROTATION_DIRECTION;
}


double BarrelElevator::getTurretRealControlInput()
{
	return 0;
}


int BarrelElevator::getTurretTurnSpeedNumber()
{
	return 0;
}


void BarrelElevator::setTurretControlState(TurretControlType, double, int)
{

}

void BarrelElevator::setGyroscopePower(double)
{

}
