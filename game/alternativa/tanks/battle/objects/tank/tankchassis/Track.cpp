#include "Track.h"
#include "SuspensionRay.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/physics/Body.h"
#include "MyMath.h"


Track::Track(Body *body, int numRays, Vector3 *vector, double value, SuspensionParams *suspensionParams, int side)
{
	this->rays = nullptr;
	this->body = body;
	this->side = side;
	this->numContacts =  0;
	this->animationSpeed = 0;
	this->setTrackParams(numRays, vector, value, suspensionParams);
}


bool Track::hasContactsWithStatic()
{
	for (int i = 0; i < rays->count(); i++)
	{
		SuspensionRay *suspensionRay = rays->at(i);
		if (suspensionRay->hasCollision && !BattleUtils::isTankBody(suspensionRay->rayHit->shape->body))
		{
			return true;
		}
	}
	return false;
}


void Track::setTrackParams(int numRays, Vector3 *vector, double value, SuspensionParams *suspensionParams)
{
	this->numRays = numRays;
	this->suspensionParams = suspensionParams;
	this->rays = new QVector<SuspensionRay*>;
	this->rays->reserve(numRays);
	double _loc5_ = value / double(numRays - 1);
	int index = 0;
	while (index < numRays)
	{
		Vector3 *vector3 = new Vector3(vector->getX(), (vector->getY() + (0.5 * value)) - (index * _loc5_), vector->getZ());
		this->rays->append(new SuspensionRay(this->body, vector3, Vector3::DOWN, suspensionParams));
		index++;
	}
}


void Track::calculateSuspensionContacts(double param1)
{
	this->numContacts = 0;
	this->averageSurfaceVelocity.setX(0);
	this->averageSurfaceVelocity.setY(0);
	this->averageSurfaceVelocity.setZ(0);
	Vector3 *_loc2_ = &this->body->state.velocity;
	int _loc3_ = 0;
	while (_loc3_ < this->numRays)
	{
		SuspensionRay *_loc4_ = this->rays->at(_loc3_);
		_loc4_->update(param1);
		if (_loc4_->hasCollision)
		{
			this->numContacts++;
			this->body->addWorldForceScaled(_loc4_->getGlobalOrigin(), _loc4_->getGlobalDirection(),-_loc4_->springForce);
			this->averageSurfaceVelocity.setX(this->averageSurfaceVelocity.getX() + _loc4_->contactVelocity.getX());
			this->averageSurfaceVelocity.setY(this->averageSurfaceVelocity.getY() + _loc4_->contactVelocity.getY());
			this->averageSurfaceVelocity.setZ(this->averageSurfaceVelocity.getZ() + _loc4_->contactVelocity.getZ());
			double _loc5_ = _loc2_->getX() - _loc4_->contactVelocity.getX();
			double _loc6_ = _loc2_->getY() - _loc4_->contactVelocity.getY();
			double _loc7_ = _loc2_->getZ() - _loc4_->contactVelocity.getZ();
			_loc4_->speed = MyMath::sqrt(_loc5_ * _loc5_ + _loc6_ * _loc6_ + _loc7_ * _loc7_);
		}
		else
		{
			_loc4_->speed = 0;
		}
		_loc3_++;
	}
	if (this->numContacts > 1)
	{
		this->averageSurfaceVelocity.setX(this->averageSurfaceVelocity.getX() / this->numContacts);
		this->averageSurfaceVelocity.setY(this->averageSurfaceVelocity.getY() / this->numContacts);
		this->averageSurfaceVelocity.setZ(this->averageSurfaceVelocity.getZ() / this->numContacts);
	}
}


void Track::setCollisionGroup(int g)
{
	int index = 0;
	while (index < this->numRays)
	{
		rays->at(index)->collisionGroup = g;
		index++;
	}
}


void Track::setAnimationSpeed(double _arg_1, double _arg_2)
{
	if (this->animationSpeed < _arg_1)
	{
		double _local_3 = (this->animationSpeed + _arg_2);
		this->animationSpeed = ((_local_3 > _arg_1) ? _arg_1 : _local_3);
	}
	else
	{
		if (this->animationSpeed > _arg_1)
		{
			double _local_3 = (this->animationSpeed - _arg_2);
			this->animationSpeed = ((_local_3 < _arg_1) ? _arg_1 : _local_3);
		}
	}
}
