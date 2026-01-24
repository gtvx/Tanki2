#include "Turret.h"
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "game/alternativa/tanks/battle/objects/tank/SimpleValueSmoother.h"
#include "MyMath.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/physics/BodyState.h"


const int Turret::TURN_SPEED_COUNT = 3;
static const double ANGLE_EPSILON = MathUtils::toRadians(0.5);


Turret::Turret(double maxTurnSpeed, double turnAcceleration) :
	spring(10, 1, 0)
{
	this->currDirection = 0;
	this->prevDirection = 0;
	this->turnSpeed = 0;
	this->controlInput = 0;
	this->interpolatedDirection = 0;
	this->turnSpeedNumber = 3;
	this->prevTurnDirection = 0;
	this->realControlInput = 0;
	this->barrelElevation = 0;
	this->gyroscopePower = 0;

	this->controlType = TurretControlType::ROTATION_DIRECTION;
	this->realControlType = TurretControlType::ROTATION_DIRECTION;
	this->maxTurnSpeedSmoother = new SimpleValueSmoother(0.3, 10, 0, 0);
	this->setMaxTurnSpeed(maxTurnSpeed, true);
	this->turnAcceleration = turnAcceleration;
}


void Turret::setTurretPhysicsDirection(double value)
{
	this->currDirection = MathUtils::clampAngle(value);
}


void Turret::setMaxTurnSpeed(double value, bool b)
{
	if (b) {
		this->maxTurnSpeed = b;
		this->maxTurnSpeedSmoother->reset(value);
	} else {
		this->maxTurnSpeedSmoother->setTargetValue(value);
	}
}


void Turret::setTurretControlState(TurretControlType param1, double param2, int param3)
{
	this->controlType = param1;
	this->controlInput = param2;
	this->turnSpeedNumber = param3;
	if (this->isNotLocked())
		this->setRealControlState(param1, param2);
}


void Turret::interpolate(double param1, int param2)
{
	this->spring.update(0.001 * param2, 0);
	double _loc3_ = MathUtils::clampAngleDelta(this->currDirection, this->prevDirection);
	this->interpolatedDirection = MathUtils::clampAngle(this->prevDirection + param1 * _loc3_ + this->spring.value);
}


double Turret::getTurretInterpolatedDirection()
{
	return this->interpolatedDirection; //Поворот
}


double Turret::getBarrelInterpolatedElevation()
{
	return this->barrelElevation;
}


void Turret::rotate(double param1, Matrix3 *param2)
{
	this->maxTurnSpeed = this->maxTurnSpeedSmoother->update(param1);
	this->prevDirection = this->currDirection;
	if (this->isLocked())
	{
		this->turnSpeed = 0;
		return;
	}
	switch(this->realControlType)
	{
	case TurretControlType::ROTATION_DIRECTION:
		this->updateDirectionalRotation(param1);
		break;
	case TurretControlType::TARGET_ANGLE_LOCAL:
		this->rotateToLocalDirection(param1, this->realControlInput);
		break;
	case TurretControlType::TARGET_ANGLE_WORLD:
		double v = this->getLocalDirectionFromWorldDirection(this->controlInput, param2);
		this->rotateToLocalDirection(param1, v);
		break;
	}
}


void Turret::updateDirectionalRotation(double param1)
{
	double _loc2_ = this->realControlInput;
	if (_loc2_ == 0)
	{
		this->turnSpeed = 0;
	}
	else
	{
		if (this->prevTurnDirection != _loc2_)
		{
			this->turnSpeed = 0;
		}
		this->currDirection = MathUtils::clampAngle(this->currDirection + this->turnSpeed * param1);
		this->turnSpeed = this->calculateTurnSpeed(this->turnSpeed, _loc2_, param1);
	}
	this->prevTurnDirection = _loc2_;
}


double Turret::getTurretPhysicsDirection()
{
	return this->currDirection;
}


bool Turret::isLocked()
{
	return !this->lockMask.isEmpty();
}


bool Turret::isNotLocked()
{
	return this->lockMask.isEmpty();
}


void Turret::setRealControlState(TurretControlType realControlType, double realControlInput)
{
	this->realControlType = realControlType;
	this->realControlInput = realControlInput;
}


void Turret::rotateToLocalDirection(double param1, double param2)
{
	double _loc3_ = MathUtils::clampAngleDelta(param2,this->currDirection);
	if (MyMath::abs(_loc3_) < ANGLE_EPSILON)
	{
		this->currDirection = param2;
		this->turnSpeed = 0;
		return;
	}
	if (_loc3_ * this->turnSpeed <= 0)
	{
		this->turnSpeed = 0;
	}
	double _loc4_ = this->turnSpeed * param1;
	if (MyMath::abs(_loc4_) > MyMath::abs(_loc3_))
	{
		this->currDirection = param2;
	}
	else
	{
		this->currDirection = MathUtils::clampAngle(this->currDirection + _loc4_);
	}

	double _loc5_ = MathUtils::sign(_loc3_);
	this->turnSpeed = this->calculateTurnSpeed(this->turnSpeed, _loc5_, param1);
}


double Turret::calculateTurnSpeed(double param1, double param2, double param3)
{
	double _loc4_ = this->maxTurnSpeed * this->turnSpeedNumber / TURN_SPEED_COUNT;
	return MathUtils::moveValueTowards(param1, param2 * _loc4_, this->turnAcceleration * param3);
}


double Turret::getLocalDirectionFromWorldDirection(double param1, Matrix3 *param2)
{
	Vector3 v;
	BattleUtils::fillDirectionVector(&v, param1);
	v.transformTransposed3(param2);
	return BattleUtils::getDirectionAngle(&v);
}


void Turret::setRemoteDirection(double value)
{
	this->spring.resetValue(MathUtils::clampAngleDelta(this->interpolatedDirection, value));
	this->prevDirection = value;
	this->currDirection = value;
}


void Turret::setBarrelElevation(double value)
{
	//value 0
	/*
	 *
	 * ArtilleryModel
	  public function getDefaultElevation() : Number
	  {
		 return getInitParam().initialTurretAngle;
	  }
	  */

	this->barrelElevation = value;
}


void Turret::updatePhysics(Body *body)
{
	this->applyGyroscopeEffect(body);
}


void Turret::reset()
{
	this->turnSpeed = 0;
	this->prevDirection = 0;
	this->currDirection = 0;
	this->interpolatedDirection = 0;
	this->maxTurnSpeedSmoother->reset(this->maxTurnSpeedSmoother->getTargetValue());
	this->spring.reset(0, 0);
}


void Turret::applyGyroscopeEffect(Body *body)
{
	if (this->gyroscopePower > 0)
	{
		Vector3 tmpVector, yAxisPrev, zAxisPrev, yAxisNext;

		body->prevState.orientation.getYAxis(&yAxisPrev);
		body->prevState.orientation.getZAxis(&zAxisPrev);
		body->state.orientation.getYAxis(&yAxisNext);
		tmpVector.cross2(&yAxisNext, &yAxisPrev);
		double _local_2 = tmpVector.dot(&zAxisPrev);
		double _local_3 = yAxisPrev.dot(&yAxisNext);
		double _local_4 = MyMath::atan2(_local_2, _local_3);
		this->currDirection = MathUtils::clampAngle((this->currDirection + (_local_4 * this->gyroscopePower)));
	}
}
