#include "ShapeContact.h"
#include "Body.h"
#include "collision/CollisionShape.h"
#include "PhysicsMaterial.h"
#include "MyMath.h"
#include "MyList.h"

static MyList<ShapeContact*> pool;
static int poolSize = 0;



ShapeContact::ShapeContact()
{
	shape1 = nullptr;
	shape2 = nullptr;
	satisfied = false;
}


void ShapeContact::calculatePersistentFrameData()
{

	Body *_loc1_ = this->shape1->body;
	Body *_loc2_ = this->shape2->body;
	this->restitution = this->shape1->material->restitution;
	double _loc3_ = this->shape2->material->restitution;
	if (_loc3_ < this->restitution)
	{
		this->restitution = _loc3_;
	}
	this->friction = this->shape1->material->friction;
	double _loc4_ = this->shape2->material->friction;
	if (_loc4_ < this->friction)
	{
		this->friction = _loc4_;
	}
	if ((_loc1_->slipperyMode && !_loc2_->movable) || (_loc2_->slipperyMode && !_loc1_->movable))
	{
		this->friction = 0;
	}

	Vector3 *_loc5_ = &this->shape1->body->state.position;

	this->r1.setX(this->position.getX() - _loc5_->getX());
	this->r1.setY(this->position.getY() - _loc5_->getY());
	this->r1.setZ(this->position.getZ() - _loc5_->getZ());
	_loc5_ = &this->shape2->body->state.position;
	this->r2.setX(this->position.getX() - _loc5_->getX());
	this->r2.setY(this->position.getY() - _loc5_->getY());
	this->r2.setZ(this->position.getZ() - _loc5_->getZ());
	if (MyMath::abs(this->normal.getX()) < MyMath::abs(this->normal.getY()))
	{
		this->tangent1.cross2(&this->normal, Vector3::X_AXIS);
		this->tangent1.normalize();
	}
	else
	{
		this->tangent1.cross2(&this->normal, Vector3::Y_AXIS);
		this->tangent1.normalize();
	}
	this->tangent2.cross2(&this->normal, &this->tangent1);
	this->normalImpulse = 0;
	this->tangentImpulse1 = 0;
	this->tangentImpulse2 = 0;
	this->normalSpeedDelta = 0;
	this->tangentSpeedDelta1 = 0;
	this->tangentSpeedDelta2 = 0;
	if (_loc1_->movable)
	{
		this->normalAngularInertiaTerm1 = this->calculateAngularInertiaTerm(&this->normal, &this->r1, &_loc1_->invInertiaWorld);
		this->tangentAngularInertiaTerm11 = this->calculateAngularInertiaTerm(&this->tangent1 ,&this->r1, &_loc1_->invInertiaWorld);
		this->tangentAngularInertiaTerm12 = this->calculateAngularInertiaTerm(&this->tangent2, &this->r1, &_loc1_->invInertiaWorld);
		this->normalSpeedDelta = this->normalSpeedDelta + (_loc1_->invMass + this->normalAngularInertiaTerm1);
		this->tangentSpeedDelta1 = this->tangentSpeedDelta1 + (_loc1_->invMass + this->tangentAngularInertiaTerm11);
		this->tangentSpeedDelta2 = this->tangentSpeedDelta2 + (_loc1_->invMass + this->tangentAngularInertiaTerm12);
	}
	if (_loc2_->movable)
	{
		this->normalAngularInertiaTerm2 = this->calculateAngularInertiaTerm(&this->normal,&this->r2, &_loc2_->invInertiaWorld);
		this->tangentAngularInertiaTerm21 = this->calculateAngularInertiaTerm(&this->tangent1 ,&this->r2, &_loc2_->invInertiaWorld);
		this->tangentAngularInertiaTerm22 = this->calculateAngularInertiaTerm(&this->tangent2, &this->r2, &_loc2_->invInertiaWorld);
		this->normalSpeedDelta = this->normalSpeedDelta + (_loc2_->invMass + this->normalAngularInertiaTerm2);
		this->tangentSpeedDelta1 = this->tangentSpeedDelta1 + (_loc2_->invMass + this->tangentAngularInertiaTerm21);
		this->tangentSpeedDelta2 = this->tangentSpeedDelta2 + (_loc2_->invMass + this->tangentAngularInertiaTerm22);
	}
	this->collisionSpeed = this->getSeparationVelocity();
	if (this->collisionSpeed < 0)
	{
		this->collisionSpeed = -this->restitution * this->collisionSpeed;
	}
	else
	{
		this->collisionSpeed = 0;
	}

}


void ShapeContact::calcualteDynamicFrameData(double param1, double param2, double param3, double dt)
{
	Body *_loc5_ = this->shape1->body;
	Body *_loc6_ = this->shape2->body;
	this->normalSpeedDelta = 0;
	this->tangentSpeedDelta1 = 0;
	this->tangentSpeedDelta2 = 0;
	if (_loc5_->movable)
	{
		this->normalSpeedDelta = this->normalSpeedDelta + (_loc5_->invMass + this->normalAngularInertiaTerm1);
		this->tangentSpeedDelta1 = this->tangentSpeedDelta1 + (_loc5_->invMass + this->tangentAngularInertiaTerm11);
		this->tangentSpeedDelta2 = this->tangentSpeedDelta2 + (_loc5_->invMass + this->tangentAngularInertiaTerm12);
	}
	if (_loc6_->movable)
	{
		this->normalSpeedDelta = this->normalSpeedDelta + (_loc6_->invMass + this->normalAngularInertiaTerm2);
		this->tangentSpeedDelta1 = this->tangentSpeedDelta1 + (_loc6_->invMass + this->tangentAngularInertiaTerm21);
		this->tangentSpeedDelta2 = this->tangentSpeedDelta2 + (_loc6_->invMass + this->tangentAngularInertiaTerm22);
	}
	if (this->penetration > param1)
	{
		double _loc7_ = this->penetration - param1;
		if (_loc7_ > param3)
		{
			_loc7_ = param3;
		}
		this->contactSeparationSpeed = param2 * _loc7_ / dt;
	}
	else
	{
		this->contactSeparationSpeed = 0;
	}
}


ShapeContact* ShapeContact::create()
{
	if (poolSize == 0)
		return new ShapeContact();

	poolSize--;
	ShapeContact *v = pool.at(poolSize);
	pool.set(poolSize, nullptr);
	return v;
}


void ShapeContact::dispose()
{
	this->shape1 = nullptr;
	this->shape2 = nullptr;

	pool.add_set(poolSize, this);
	poolSize++;
}


double ShapeContact::calculateAngularInertiaTerm(Vector3 *param1, Vector3 *param2, Matrix3 *param3)
{
	double _loc4_ = param2->getY() * param1->getZ() - param2->getZ() * param1->getY();
	double _loc5_ = param2->getZ() * param1->getX() - param2->getX() * param1->getZ();
	double _loc6_ = param2->getX() * param1->getY() - param2->getY() * param1->getX();
	double _loc7_ = param3->m00 * _loc4_ + param3->m01 * _loc5_ + param3->m02 * _loc6_;
	double _loc8_ = param3->m10 * _loc4_ + param3->m11 * _loc5_ + param3->m12 * _loc6_;
	double _loc9_ = param3->m20 * _loc4_ + param3->m21 * _loc5_ + param3->m22 * _loc6_;
	_loc4_ = _loc8_ * param2->getZ() - _loc9_ * param2->getY();
	_loc5_ = _loc9_ * param2->getX() - _loc7_ * param2->getZ();
	_loc6_ = _loc7_ * param2->getY() - _loc8_ * param2->getX();
	return _loc4_ * param1->getX() + _loc5_ * param1->getY() + _loc6_ * param1->getZ();
}


double ShapeContact::getSeparationVelocity()
{
	Vector3 *_loc1_ = &this->shape1->body->state.angularVelocity;
	double _loc2_ = _loc1_->getY() * this->r1.getZ() - _loc1_->getZ() * this->r1.getY();
	double _loc3_ = _loc1_->getZ() * this->r1.getX() - _loc1_->getX() * this->r1.getZ();
	double _loc4_ = _loc1_->getX() * this->r1.getY() - _loc1_->getY() * this->r1.getX();
	Vector3 *_loc5_ = &this->shape1->body->state.velocity;
	double _loc6_ = _loc5_->getX() + _loc2_;
	double _loc7_ = _loc5_->getY() + _loc3_;
	double _loc8_ = _loc5_->getZ() + _loc4_;
	_loc1_ = &this->shape2->body->state.angularVelocity;
	_loc2_ = _loc1_->getY() * this->r2.getZ() - _loc1_->getZ() * this->r2.getY();
	_loc3_ = _loc1_->getZ() * this->r2.getX() - _loc1_->getX() * this->r2.getZ();
	_loc4_ = _loc1_->getX() * this->r2.getY() - _loc1_->getY() * this->r2.getX();
	_loc5_ = &this->shape2->body->state.velocity;
	_loc6_ = _loc6_ - (_loc5_->getX() + _loc2_);
	_loc7_ = _loc7_ - (_loc5_->getY() + _loc3_);
	_loc8_ = _loc8_ - (_loc5_->getZ() + _loc4_);
	return _loc6_ * this->normal.getX() + _loc7_ * this->normal.getY() + _loc8_ * this->normal.getZ();
}
