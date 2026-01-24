#include "SuspensionRay.h"
#include "game/alternativa/tanks/models/weapon/RayCollisionFilter.h"
#include "game/alternativa/math/Matrix3.h"
#include "game/alternativa/physics/Body.h"
#include "SuspensionParams.h"
#include "game/alternativa/physics/PhysicsScene.h"
#include "game/alternativa/physics/collision/CollisionDetector.h"
#include "game/alternativa/tanks/battle/objects/tank/TankConst.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "getTimer.h"


SuspensionRay::SuspensionRay(Body *body, Vector3 *param2, Vector3 *param3, SuspensionParams *param4)
{
	this->hasCollision = false;
	this->rayHit = new RayHit();
	this->prevCompression = 0;
	this->body = body;
	this->collisionGroup = 0;
	this->springForce = 0;
	this->origin.copy(param2);
	this->direction.copy(param3);
	this->suspensionParams = param4;
	this->collisionFilter = new RayCollisionFilter(body);
	this->speed = 0;
}


Vector3* SuspensionRay::getOrigin()
{
	return &this->origin;
}


void SuspensionRay::update(double param1)
{
	this->raycast();
	if (this->hasCollision)
	{
		this->calculateSpringForce(param1);
		this->calculateContactVelocity();
	}
}


void SuspensionRay::raycast()
{
	Matrix3 *_loc1_ = &this->body->baseMatrix;
	this->globalDirection.setX(_loc1_->m00 * this->direction.getX() + _loc1_->m01 * this->direction.getY() + _loc1_->m02 * this->direction.getZ());
	this->globalDirection.setY(_loc1_->m10 * this->direction.getX() + _loc1_->m11 * this->direction.getY() + _loc1_->m12 * this->direction.getZ());
	this->globalDirection.setZ(_loc1_->m20 * this->direction.getX() + _loc1_->m21 * this->direction.getY() + _loc1_->m22 * this->direction.getZ());
	Vector3 *_loc2_ = &this->body->state.position;
	this->globalOrigin.setX(_loc1_->m00 * this->origin.getX() + _loc1_->m01 * this->origin.getY() + _loc1_->m02 * this->origin.getZ());
	this->globalOrigin.setY(_loc1_->m10 * this->origin.getX() + _loc1_->m11 * this->origin.getY() + _loc1_->m12 * this->origin.getZ());
	this->globalOrigin.setZ(_loc1_->m20 * this->origin.getX() + _loc1_->m21 * this->origin.getY() + _loc1_->m22 * this->origin.getZ());
	this->globalOrigin.setX(this->globalOrigin.getX() + _loc2_->getX());
	this->globalOrigin.setY(this->globalOrigin.getY() + _loc2_->getY());
	this->globalOrigin.setZ(this->globalOrigin.getZ() + _loc2_->getZ());
	if (this->hasCollision)
	{
		this->prevCompression = this->suspensionParams->maxRayLength - this->rayHit->t;
	}

	this->hasCollision = this->body->scene->collisionDetector->raycast(&this->globalOrigin,
																	   &this->globalDirection,
																	   this->collisionGroup,
																	   this->suspensionParams->maxRayLength,
																	   this->collisionFilter,
																	   this->rayHit);

	if (this->hasCollision)
	{
		this->hasCollision = this->rayHit->normal.getZ() > TankConst::MAX_SLOPE_ANGLE_COS;
	}
}


void SuspensionRay::calculateSpringForce(double param1)
{
	double _loc2_ = this->suspensionParams->maxRayLength - this->rayHit->t;
	this->springForce = this->suspensionParams->springCoeff * _loc2_;
	double _loc3_ = (_loc2_ - this->prevCompression) / param1;
	this->springForce = this->springForce + _loc3_ * this->suspensionParams->dampingCoeff;
	if (this->springForce < 0)
	{
		this->springForce = 0;
	}
}


void SuspensionRay::calculateContactVelocity()
{
	Body *_loc1_ = this->rayHit->shape->body;
	if (_loc1_->tank != nullptr)
	{
		const Vector3* _loc2_ = &_loc1_->state.position;
		const Vector3* _loc3_ = &_loc1_->state.velocity;
		const Vector3* _loc4_ = &_loc1_->state.angularVelocity;
		const Vector3* _loc5_ = &this->rayHit->position;
		const double _loc6_ = _loc5_->getX() - _loc2_->getX();
		const double _loc7_ = _loc5_->getY() - _loc2_->getY();
		const double _loc8_ = _loc5_->getZ() - _loc2_->getZ();
		this->contactVelocity.setX(_loc4_->getY() * _loc8_ - _loc4_->getZ() * _loc7_);
		this->contactVelocity.setY(_loc4_->getZ() * _loc6_ - _loc4_->getX() * _loc8_);
		this->contactVelocity.setZ(_loc4_->getX() * _loc7_ - _loc4_->getY() * _loc6_);
		this->contactVelocity.setX(this->contactVelocity.getX() + _loc3_->getX());
		this->contactVelocity.setY(this->contactVelocity.getY() + _loc3_->getY());
		this->contactVelocity.setZ(this->contactVelocity.getZ() + _loc3_->getZ());
	}
	else
	{
		this->contactVelocity.setX(0);
		this->contactVelocity.setY(0);
		this->contactVelocity.setZ(0);
	}
}


Vector3* SuspensionRay::getGlobalOrigin()
{
	return &this->globalOrigin;
}


Vector3* SuspensionRay::getGlobalDirection()
{
	return &this->globalDirection;
}
