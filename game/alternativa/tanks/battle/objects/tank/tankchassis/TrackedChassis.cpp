#include "TrackedChassis.h"
#include "game/alternativa/physics/Body.h"
#include "SuspensionParams.h"
#include "game/alternativa/tanks/battle/objects/tank/ValueSmoother.h"
#include "game/alternativa/tanks/battle/objects/tank/TankConst.h"
#include "Track.h"
#include "game/alternativa/tanks/utils/EncryptedNumberImpl.h"
#include "game/alternativa/physics/PhysicsScene.h"
#include "MyMath.h"
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "SuspensionRay.h"


static const double MIN_ACCELERATION = 400;
const int TrackedChassis::TURN_SPEED_COUNT = 7;

static Vector3 _xAxis;
static Vector3 _yAxis;
static Vector3 _zAxis;
static Vector3 _relativeVelocity;
static Vector3 _surfaceVelocity;
static Vector3 _surfaceAngularVelocity;
static Vector3 _relativeAngularVelocity;
static Vector3 _midPoint;
static Vector3 _forceVector;


TrackedChassis::TrackedChassis(Body *body, SuspensionParams *suspensionParams, ValueSmoother *maxSpeedSmoother, Vector3 *vector3)
{
	this->movementDirection = 0;
	this->turnDirection = 0;
	this->turnSpeedNumber = 0;
	this->inverseBackTurnMovement = false;
	this->_acceleration = new EncryptedNumberImpl(0);
	this->_reverseAcceleration = new EncryptedNumberImpl(0);
	this->_sideAcceleration = new EncryptedNumberImpl(0);
	this->_turnAcceleration = new EncryptedNumberImpl(0);
	this->_reverseTurnAcceleration = new EncryptedNumberImpl(0);
	this->_stabilizationAcceleration = new EncryptedNumberImpl(0);
	this->body = body;
	this->suspensionParams = suspensionParams;
	this->maxSpeedSmoother = maxSpeedSmoother;
	this->createTracks(TankConst::NUM_RAYS_PER_TRACK, vector3);
}


void TrackedChassis::createTracks(int param1, Vector3 *param2)
{
	const double _loc3_ = param2->getY() * 0.8;
	const double _loc4_ = param2->getX() - 40.;
	this->leftTrack = new Track(this->body, param1, new Vector3(-0.5 * _loc4_, 0, -0.5 * param2->getZ() + TankConst::RAY_OFFSET), _loc3_, this->suspensionParams, -1);
	this->rightTrack = new Track(this->body, param1, new Vector3(0.5 * _loc4_, 0, -0.5 * param2->getZ() + TankConst::RAY_OFFSET), _loc3_, this->suspensionParams, 1);
}


void TrackedChassis::applyForces(double param1, double param2, double param3)
{
	this->adjustSuspensionSpringCoeff();
	this->calculateSuspensionContacts(param3);
	this->applyMovementForces(param1,param2,param3);
	this->applySlopeHack();
}


void TrackedChassis::setAcceleration(double value)
{
	this->_acceleration->setNumber(value);
}


void TrackedChassis::applySlopeHack()
{
	if (this->rightTrack->numContacts >= this->rightTrack->numRays >> 1 || this->leftTrack->numContacts >= this->leftTrack->numRays >> 1)
	{
		const Matrix3 *baseMatrix = &this->body->baseMatrix;
		const Vector3 *gravity = this->body->scene->gravity;
		const double _loc3_ = gravity->getX() * baseMatrix->m02 + gravity->getY() * baseMatrix->m12 + gravity->getZ() * baseMatrix->m22;
		const double _loc4_ = gravity->length();
		const double _loc5_ = MyMath::SQRT1_2 * _loc4_;
		if (_loc3_ < -_loc5_ || _loc3_ > _loc5_)
		{
			double _loc6_ = (baseMatrix->m02 * _loc3_ - gravity->getX()) * this->body->mass;
			double _loc7_ = (baseMatrix->m12 * _loc3_ - gravity->getY()) * this->body->mass;
			double _loc8_ = (baseMatrix->m22 * _loc3_ - gravity->getZ()) * this->body->mass;
			this->body->addForceXYZ(_loc6_,_loc7_,_loc8_);
		}
	}
}


void TrackedChassis::applyMovementForces(double param1, double param2, double param3)
{
	if (this->leftTrack->numContacts + this->rightTrack->numContacts > 0)
	{
		this->doApplyMovementForces(param1, param2, param3);
	}
}


void TrackedChassis::doApplyMovementForces(double param1, double param2, double param3)
{
	Vector3 *_loc4_ = &this->body->state.velocity;
	Vector3 *_loc5_ = &this->body->state.angularVelocity;
	Matrix3 *_loc6_ = &this->body->baseMatrix;
	_xAxis.setX(_loc6_->m00);
	_xAxis.setY(_loc6_->m10);
	_xAxis.setZ(_loc6_->m20);
	_yAxis.setX(_loc6_->m01);
	_yAxis.setY(_loc6_->m11);
	_yAxis.setZ(_loc6_->m21);
	_zAxis.setX(_loc6_->m02);
	_zAxis.setY(_loc6_->m12);
	_zAxis.setZ(_loc6_->m22);
	double _loc7_ = 1.;
	double _loc8_ = MyMath::PI / 4.;
	double _loc9_ = TankConst::MAX_SLOPE_ANGLE;
	if (_zAxis.getZ() < MyMath::cos(_loc8_))
	{
		if (_zAxis.getZ() < MyMath::cos(_loc9_))
		{
			_loc7_ = 0;
		}
		else
		{
			_loc7_ = (_loc9_ - MyMath::acos(_zAxis.getZ())) / (_loc9_ - _loc8_);
		}
	}
	this->calculateSurfaceVelocities(&_surfaceVelocity, &_surfaceAngularVelocity);
	_relativeVelocity.setX(_loc4_->getX() - _surfaceVelocity.getX());
	_relativeVelocity.setY(_loc4_->getY() - _surfaceVelocity.getY());
	_relativeVelocity.setZ(_loc4_->getZ() - _surfaceVelocity.getZ());
	_relativeAngularVelocity.setX(_loc5_->getX() - _surfaceAngularVelocity.getX());
	_relativeAngularVelocity.setY(_loc5_->getY() - _surfaceAngularVelocity.getY());
	_relativeAngularVelocity.setZ(_loc5_->getZ() - _surfaceAngularVelocity.getZ());
	double _loc10_ = _relativeVelocity.getX() * _yAxis.getX() + _relativeVelocity.getY() * _yAxis.getY() + _relativeVelocity.getZ() * _yAxis.getZ();
	double _loc11_ = _relativeAngularVelocity.getX() * _zAxis.getX() + _relativeAngularVelocity.getY() * _zAxis.getY() + _relativeAngularVelocity.getZ() * _zAxis.getZ();
	double _loc12_ = _relativeVelocity.getX() * _xAxis.getX() + _relativeVelocity.getY() * _xAxis.getY() + _relativeVelocity.getZ() * _xAxis.getZ();
	double _loc13_ = this->_sideAcceleration->getNumber() * _loc7_ * param3;
	if (_loc12_ < 0)
	{
		if (_loc13_ > -_loc12_)
		{
			_loc12_ = 0;
		}
		else
		{
			_loc12_ = _loc12_ + _loc13_;
		}
	}
	else if (_loc12_ > 0)
	{
		if (_loc13_ > _loc12_)
		{
			_loc12_ = 0;
		}
		else
		{
			_loc12_ = _loc12_ - _loc13_;
		}
	}
	_relativeVelocity.setLengthAlongDirection(&_xAxis, _loc12_);
	_loc4_->setX(_surfaceVelocity.getX() + _relativeVelocity.getX());
	_loc4_->setY(_surfaceVelocity.getY() + _relativeVelocity.getY());
	_loc4_->setZ(_surfaceVelocity.getZ() + _relativeVelocity.getZ());
	int _loc14_ = this->leftTrack->numContacts;
	int _loc15_ = this->rightTrack->numContacts;
	double _loc16_ = this->_acceleration->getNumber();
	double _loc17_ = this->_turnAcceleration->getNumber();
	if (_loc14_ > 0 || _loc15_ > 0)
	{
		double _loc18_ = 0;
		if (this->movementDirection == 0)
		{
			_loc18_ = -MathUtils::sign(_loc10_) * _loc16_ * param3;
			if (MathUtils::sign(_loc10_) != MathUtils::sign(_loc10_ + _loc18_))
			{
				_loc18_ = -_loc10_;
			}
		}
		else
		{
			if (MathUtils::sign(_loc10_) * MathUtils::sign(this->movementDirection) < 0)
			{
				_loc16_ = this->_reverseAcceleration->getNumber();
			}
			_loc18_ = this->movementDirection * _loc16_ * param3;
		}
		double _loc19_ = MathUtils::clamp(_loc10_ + _loc18_, -param1, param1);
		double _loc20_ = _loc19_ - _loc10_;
		double _loc21_ = 1;
		double _loc22_ = MathUtils::clamp(1 - MyMath::abs(_loc10_ / param1), 0, 1);
		if (_loc22_ < _loc21_ && this->movementDirection * MathUtils::sign(_loc10_) > 0)
		{
			_loc20_ = _loc20_ * (_loc22_ / _loc21_);
		}
		double _loc23_ = _loc20_ / param3;
		if (MyMath::abs(_loc23_) < MIN_ACCELERATION && MyMath::abs(_loc19_) > 0.5 * this->maxSpeedSmoother->getTargetValue())
		{
			_loc23_ = MathUtils::numberSign(_loc23_, 0.1) * MIN_ACCELERATION;
		}
		double _loc24_ = _loc23_ * this->body->mass;
		int _loc25_ = _loc14_ + _loc15_;
		//double _loc26_ = _loc24_ * (_loc25_ + 0.21 * (20 - _loc25_)) / 10.;
		double _loc26_ = ((_loc24_ * (_loc25_ + (0.21 * (20 - _loc25_)))) / 10);
		double _loc27_ = _loc26_ / _loc25_;
		double  _loc28_ = MyMath::PI / 4.;
		double _loc29_ = MyMath::PI / 3.;
		int _loc30_ = 0;
		while (_loc30_ < TankConst::NUM_RAYS_PER_TRACK)
		{
			this->applyForceFromRay(this->leftTrack->rays->at(_loc30_), &_yAxis, _loc27_, _loc29_, _loc28_);
			this->applyForceFromRay(this->rightTrack->rays->at(_loc30_), &_yAxis, _loc27_, _loc29_, _loc28_);
			_loc30_++;
		}
		double _loc31_ = 1;
		if (_loc14_ == 0 || _loc15_ == 0)
		{
			_loc31_ = 0.5;
		}
		double _loc32_ = 0;
		if (this->turnDirection == 0)
		{
			_loc32_ = -MathUtils::sign(_loc11_) * this->_stabilizationAcceleration->getNumber() * _loc7_ * param3;
			if (MathUtils::sign(_loc11_) != MathUtils::sign(_loc11_ + _loc32_))
			{
				_loc32_ = -_loc11_;
			}
		}
		else
		{
			if (this->isReversedTurn(this->turnDirection, _loc11_, this->movementDirection, this->inverseBackTurnMovement))
			{
				_loc17_ = this->_reverseTurnAcceleration->getNumber();
			}
			_loc32_ = this->turnDirection * _loc17_ * _loc7_ * param3;
			if (this->movementDirection == -1 && this->inverseBackTurnMovement)
			{
				_loc32_ = -_loc32_;
			}
		}
		double _loc33_ = param2;
		if (this->turnDirection != 0)
		{
			_loc33_ = param2 * double(this->turnSpeedNumber) / double(TURN_SPEED_COUNT);
		}
		double _loc34_ = _loc33_ * _loc31_;
		double _loc35_ = MathUtils::clamp(_loc11_ + _loc32_, -_loc34_, _loc34_);
		_relativeAngularVelocity.setLengthAlongDirection(&_zAxis,_loc35_);
		_loc5_->setX(_surfaceAngularVelocity.getX() + _relativeAngularVelocity.getX());
		_loc5_->setY(_surfaceAngularVelocity.getY() + _relativeAngularVelocity.getY());
		_loc5_->setZ(_surfaceAngularVelocity.getZ() + _relativeAngularVelocity.getZ());
	}

}


void TrackedChassis::calculateSuspensionContacts(double value)
{
	this->leftTrack->calculateSuspensionContacts(value);
	this->rightTrack->calculateSuspensionContacts(value);
}


void TrackedChassis::adjustSuspensionSpringCoeff()
{
	double _local_1 = this->body->scene->gravity->length() * this->body->mass;
	this->suspensionParams->springCoeff = (_local_1 / ((2 * TankConst::NUM_RAYS_PER_TRACK) * (this->suspensionParams->maxRayLength - this->suspensionParams->nominalRayLength)));
}


void TrackedChassis::setReverseAcceleration(double value)
{
	this->_reverseAcceleration->setNumber(value);
}


void TrackedChassis::setSideAcceleration(double value)
{
	this->_sideAcceleration->setNumber(value);
}


void TrackedChassis::setTurnAcceleration(double value)
{
	this->_turnAcceleration->setNumber(value);
}


void TrackedChassis::setReverseTurnAcceleration(double value)
{
	this->_reverseTurnAcceleration->setNumber(value);
}


void TrackedChassis::setStabilizationAcceleration(double value)
{
	this->_stabilizationAcceleration->setNumber(value);
}


void TrackedChassis::calculateSurfaceVelocities(Vector3 *param1, Vector3 *param2)
{
	Vector3 *_loc6_ = nullptr;
	double _loc3_ = 1. / double(this->leftTrack->numContacts + this->rightTrack->numContacts);
	double _loc7_ = 0;
	double _loc8_ = 0;
	double _loc9_ = 0;
	int _loc5_ = 0;
	while (_loc5_ < TankConst::NUM_RAYS_PER_TRACK)
	{
		SuspensionRay *_loc4_ = this->leftTrack->rays->at(_loc5_);
		if (_loc4_->hasCollision)
		{
			_loc6_ = &_loc4_->rayHit->position;
			_loc7_ = _loc7_ + _loc6_->getX();
			_loc8_ = _loc8_ + _loc6_->getY();
			_loc9_ = _loc9_ + _loc6_->getZ();
		}
		_loc4_ = this->rightTrack->rays->at(_loc5_);
		if (_loc4_->hasCollision)
		{
			_loc6_ = &_loc4_->rayHit->position;
			_loc7_ = _loc7_ + _loc6_->getX();
			_loc8_ = _loc8_ + _loc6_->getY();
			_loc9_ = _loc9_ + _loc6_->getZ();
		}
		_loc5_++;
	}
	_loc7_ = _loc7_ * _loc3_;
	_loc8_ = _loc8_ * _loc3_;
	_loc9_ = _loc9_ * _loc3_;
	_midPoint.setX(_loc7_);
	_midPoint.setY(_loc8_);
	_midPoint.setZ(_loc9_);
	param1->setX(0);
	param1->setY(0);
	param1->setZ(0);
	param2->setX(0);
	param2->setY(0);
	param2->setZ(0);
	_loc5_ = 0;
	while (_loc5_ < TankConst::NUM_RAYS_PER_TRACK)
	{
		this->addVelocitiesFromRay(this->leftTrack->rays->at(_loc5_), &_midPoint, param1, param2);
		this->addVelocitiesFromRay(this->rightTrack->rays->at(_loc5_), &_midPoint, param1, param2);
		_loc5_++;
	}
	param1->setX(param1->getX() * _loc3_);
	param1->setY(param1->getY() * _loc3_);
	param1->setZ(param1->getZ() * _loc3_);
	param2->setX(param2->getX() * _loc3_);
	param2->setY(param2->getY() * _loc3_);
	param2->setZ(param2->getZ() * _loc3_);
}


void TrackedChassis::applyForceFromRay(SuspensionRay *param1, Vector3 *param2, double param3, double param4, double param5)
{
	if (param1->hasCollision)
	{
		double _loc6_ = param2->getX();
		double _loc7_ = param2->getY();
		double _loc8_ = param2->getZ();
		double _loc9_ = _loc6_ * _loc6_ + _loc7_ * _loc7_ + _loc8_ * _loc8_;
		if (_loc9_ > 0.00001)
		{
			double _loc10_ = MyMath::acos(param1->rayHit->normal.getZ());
			if (_loc10_ < 0)
			{
				_loc10_ = -_loc10_;
			}
			if (_loc10_ < param4)
			{
				double _loc11_ = param3 / MyMath::sqrt(_loc9_);
				if (_loc10_ > param5)
				{
					_loc11_ = _loc11_ * ((param4 - _loc10_) / (param4 - param5));
				}
				_forceVector.setX(_loc6_ * _loc11_);
				_forceVector.setY(_loc7_ * _loc11_);
				_forceVector.setZ(_loc8_ * _loc11_);
				this->body->addWorldForceAtLocalPoint(param1->getOrigin(), &_forceVector);
			}
		}
	}
}


bool TrackedChassis::isReversedTurn(int param1, double param2, int param3, bool param4)
{
	int _loc5_ = param4 && param3 < 0 ? -1 : 1;
	return double(param1) * param2 * _loc5_ < 0;
}


void TrackedChassis::addVelocitiesFromRay(SuspensionRay *param1, Vector3 *param2, Vector3 *param3, Vector3 *param4)
{
	if (param1->hasCollision == true)
	{
		param3->setX(param3->getX() + param1->contactVelocity.getX());
		param3->setY(param3->getY() + param1->contactVelocity.getY());
		param3->setZ(param3->getZ() + param1->contactVelocity.getZ());
		Vector3 *_loc5_ = &param1->rayHit->position;
		double _loc6_ = _loc5_->getX() - param2->getX();
		double _loc7_ = _loc5_->getY() - param2->getY();
		double _loc8_ = _loc5_->getZ() - param2->getZ();
		double _loc9_ = _loc6_ * _loc6_ + _loc7_ * _loc7_ + _loc8_ * _loc8_;
		if (_loc9_ > 1)
		{
			double _loc10_ = 1. / _loc9_;
			Vector3 *_loc11_ = &param1->contactVelocity;
			double _loc12_ = (_loc7_ * _loc11_->getZ() - _loc8_ * _loc11_->getY()) * _loc10_;
			double _loc13_ = (_loc8_ * _loc11_->getX() - _loc6_ * _loc11_->getZ()) * _loc10_;
			double _loc14_ = (_loc6_ * _loc11_->getY() - _loc7_ * _loc11_->getX()) * _loc10_;
			param4->setX(param4->getX() + _loc12_);
			param4->setY(param4->getY() + _loc13_);
			param4->setZ(param4->getZ() + _loc14_);
		}
	}
}


void TrackedChassis::setTracksCollisionGroup(int param1)
{
	this->leftTrack->setCollisionGroup(param1);
	this->rightTrack->setCollisionGroup(param1);
}


double TrackedChassis::getAcceleration()
{
	return this->_acceleration->getNumber();
}


int TrackedChassis::getActualMovementDirection()
{
	return this->movementDirection;
}


int TrackedChassis::getActualTurnDirection()
{
	return this->turnDirection;
}
