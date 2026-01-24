#include "Body.h"
#include "collision/CollisionShape.h"
#include "game/alternativa/math/Quaternion.h"
#include "MyMath.h"
#include "game/alternativa/tanks/utils/EncryptedNumberImpl.h"
#include "Error.h"


static const double MAX_SPEED_Z = 25 * 60;
static const double rotationalDamping = 0.997;
static const double linearDamping = 0.997;


Body::Body(double mass, const Matrix3 *invInertia, double maxSpeedXY)
{
	this->tank = nullptr;
	this->mass = mass;
	this->invMass = 1. / mass;
	this->invInertia.copy(invInertia);

	this->maxSpeedXY = new EncryptedNumberImpl(maxSpeedXY);


	acceleration.reset();
	angularAcceleration.reset();
	pseudoVelocity.reset();
	pseudoAngularVelocity.reset();
	previousVelocity.reset();


	canFreeze = false;
	frozen = false;
	freezeCounter = 0;
	movable = true;
	numCollisionShapes = 0;
	minZAcceleration = -1100;
	maxZSpeedDelta = 600;

	slipperyMode = false;
	postCollisionFilter = nullptr;
	collisionShapes = nullptr;
	scene = nullptr;

	this->forceAccum.reset();
	this->torqueAccum.reset();
}


void Body::clearAccumulators()
{
	this->forceAccum.setX(0);
	this->forceAccum.setY(0);
	this->forceAccum.setZ(0);
	this->torqueAccum.setX(0);
	this->torqueAccum.setY(0);
	this->torqueAccum.setZ(0);
}


void Body::calcDerivedData()
{
	this->state.orientation.toMatrix3(&this->baseMatrix);
	this->invInertiaWorld.copy(&this->invInertia);
	this->invInertiaWorld.append(&this->baseMatrix);
	this->invInertiaWorld.prependTransposed(&this->baseMatrix);

	if (this->collisionShapes != nullptr)
	{
		this->aabb.infinity();

		int index = 0;

		while (index < this->numCollisionShapes)
		{
			CollisionShape *collisionShape = this->collisionShapes->at(index);

			collisionShape->transform.setFromMatrix3(&this->baseMatrix, &this->state.position);

			if (collisionShape->localTransform != nullptr)
				collisionShape->transform.prepend(collisionShape->localTransform);

			collisionShape->calculateAABB();

			this->aabb.addBoundBox(&collisionShape->aabb);
			index++;
		}
	}
}


void Body::calcAccelerations()
{
	this->acceleration.setX(this->forceAccum.getX() * this->invMass);
	this->acceleration.setY(this->forceAccum.getY() * this->invMass);
	this->acceleration.setZ(this->forceAccum.getZ() * this->invMass);
	this->angularAcceleration.setX(this->invInertiaWorld.m00 * this->torqueAccum.getX() + this->invInertiaWorld.m01 * this->torqueAccum.getY() + this->invInertiaWorld.m02 * this->torqueAccum.getZ());
	this->angularAcceleration.setY(this->invInertiaWorld.m10 * this->torqueAccum.getX() + this->invInertiaWorld.m11 * this->torqueAccum.getY() + this->invInertiaWorld.m12 * this->torqueAccum.getZ());
	this->angularAcceleration.setZ(this->invInertiaWorld.m20 * this->torqueAccum.getX() + this->invInertiaWorld.m21 * this->torqueAccum.getY() + this->invInertiaWorld.m22 * this->torqueAccum.getZ());
}


void Body::saveState()
{
	this->prevState.copy(&this->state);
}


void Body::integrateVelocity(double dt)
{
	this->integrateLinearVelocity(dt);
	this->integrateAngularVelocity(dt);
}


void Body::integratePosition(double param1)
{
	Vector3 *position = &this->state.position;
	Vector3 *velocity = &this->state.velocity;
	this->state.position.setX(position->getX() + velocity->getX() * param1);
	this->state.position.setY(position->getY() + velocity->getY() * param1);
	this->state.position.setZ(position->getZ() + velocity->getZ() * param1);
	this->state.orientation.addScaledVector(&this->state.angularVelocity, param1);
}


void Body::integratePseudoVelocity(double param1)
{
	this->state.position.setX(this->state.position.getX() + this->pseudoVelocity.getX() * param1);
	this->state.position.setY(this->state.position.getY() + this->pseudoVelocity.getY() * param1);
	this->state.position.setZ(this->state.position.getZ() + this->pseudoVelocity.getZ() * param1);
	this->state.orientation.addScaledVector(&this->pseudoAngularVelocity, param1);
	this->pseudoVelocity.reset();
	this->pseudoAngularVelocity.reset();
}


void Body::integrateAngularVelocity(double dt)
{
	this->state.angularVelocity.setX(this->state.angularVelocity.getX() + this->angularAcceleration.getX() * dt);
	this->state.angularVelocity.setY(this->state.angularVelocity.getY() + this->angularAcceleration.getY() * dt);
	this->state.angularVelocity.setZ(this->state.angularVelocity.getZ() + this->angularAcceleration.getZ() * dt);
	this->state.angularVelocity.setX(this->state.angularVelocity.getX() * rotationalDamping);
	this->state.angularVelocity.setY(this->state.angularVelocity.getY() * rotationalDamping);
	this->state.angularVelocity.setZ(this->state.angularVelocity.getZ() * rotationalDamping);
	if (this->state.angularVelocity.length() > 10)
	{
		this->state.angularVelocity.setLength(10);
	}
}


void Body::integrateLinearVelocity(double dt)
{
	this->previousVelocity.copy(&this->state.velocity);

	if (this->acceleration.getZ() < this->minZAcceleration)
	{
		this->acceleration.setZ(this->minZAcceleration);
	}

	this->state.velocity.setX(this->state.velocity.getX() + (this->acceleration.getX() * dt));
	this->state.velocity.setY(this->state.velocity.getY() + (this->acceleration.getY() * dt));
	this->state.velocity.setZ(this->state.velocity.getZ() + (this->acceleration.getZ() * dt));
	this->state.velocity.setX(this->state.velocity.getX() * linearDamping);
	this->state.velocity.setY(this->state.velocity.getY() * linearDamping);
	this->state.velocity.setZ(this->state.velocity.getZ() * linearDamping);


	double velocity_z = MyMath::abs(this->state.velocity.getZ());
	if (velocity_z > MAX_SPEED_Z)
	{
		this->state.velocity.setZ(this->state.velocity.getZ() * (MAX_SPEED_Z / velocity_z));
	}
	if (this->state.velocity.getZ() - this->previousVelocity.getZ() > this->maxZSpeedDelta)
	{
		this->state.velocity.setZ(this->previousVelocity.getZ() + this->maxZSpeedDelta);
	}
	double _loc3_ = this->state.velocity.getX();
	double _loc4_ = this->state.velocity.getY();
	double _loc5_ = MyMath::sqrt((_loc3_ * _loc3_) + (_loc4_ * _loc4_));
	double _loc6_ = this->maxSpeedXY->getNumber();

	if (_loc5_ > _loc6_)
	{
		double _loc7_ = _loc6_ / _loc5_;
		this->state.velocity.setX(this->state.velocity.getX() * _loc7_);
		this->state.velocity.setY(this->state.velocity.getY() * _loc7_);
	}

}


void Body::applyWorldPseudoImpulseAtLocalPoint(Vector3 *param1, Vector3 *param2, double v)
{
	double v2 = v * this->invMass;
	this->pseudoVelocity.setX(this->pseudoVelocity.getX() + (v2 * param2->getX()));
	this->pseudoVelocity.setY(this->pseudoVelocity.getY() + (v2 * param2->getY()));
	this->pseudoVelocity.setZ(this->pseudoVelocity.getZ() + (v2 * param2->getZ()));
	double _loc5_ = (((param1->getY() * param2->getZ()) - (param1->getZ() * param2->getY())) * v);
	double _loc6_ = (((param1->getZ() * param2->getX()) - (param1->getX() * param2->getZ())) * v);
	double _loc7_ = (((param1->getX() * param2->getY()) - (param1->getY() * param2->getX())) * v);
	this->pseudoAngularVelocity.setX(this->pseudoAngularVelocity.getX() + ((this->invInertiaWorld.m00 * _loc5_) + (this->invInertiaWorld.m01 * _loc6_) + (this->invInertiaWorld.m02 * _loc7_)));
	this->pseudoAngularVelocity.setY(this->pseudoAngularVelocity.getY() + ((this->invInertiaWorld.m10 * _loc5_) + (this->invInertiaWorld.m11 * _loc6_) + (this->invInertiaWorld.m12 * _loc7_)));
	this->pseudoAngularVelocity.setZ(this->pseudoAngularVelocity.getZ() + ((this->invInertiaWorld.m20 * _loc5_) + (this->invInertiaWorld.m21 * _loc6_) + (this->invInertiaWorld.m22 * _loc7_)));
}


void Body::applyWorldImpulseAtLocalPoint(Vector3 *param1, Vector3 *param2, double param3)
{
	double _loc4_ = param3 * this->invMass;
	this->state.velocity.setX(this->state.velocity.getX() + _loc4_ * param2->getX());
	this->state.velocity.setY(this->state.velocity.getY() + _loc4_ * param2->getY());
	this->state.velocity.setZ(this->state.velocity.getZ() + _loc4_ * param2->getZ());
	double _loc5_ = (((param1->getY() * param2->getZ()) - (param1->getZ() * param2->getY())) * param3);
	double _loc6_ = (((param1->getZ() * param2->getX()) - (param1->getX() * param2->getZ())) * param3);
	double _loc7_ = (((param1->getX() * param2->getY()) - (param1->getY() * param2->getX())) * param3);
	this->state.angularVelocity.setX(this->state.angularVelocity.getX() + (((this->invInertiaWorld.m00 * _loc5_) + (this->invInertiaWorld.m01 * _loc6_)) + (this->invInertiaWorld.m02 * _loc7_)));
	this->state.angularVelocity.setY(this->state.angularVelocity.getY() + (((this->invInertiaWorld.m10 * _loc5_) + (this->invInertiaWorld.m11 * _loc6_)) + (this->invInertiaWorld.m12 * _loc7_)));
	this->state.angularVelocity.setZ(this->state.angularVelocity.getZ() + (((this->invInertiaWorld.m20 * _loc5_) + (this->invInertiaWorld.m21 * _loc6_)) + (this->invInertiaWorld.m22 * _loc7_)));
}


void Body::addCollisionShape(CollisionShape *collisionShape, const Matrix4 *matrix)
{
	if (collisionShape == nullptr)
		throw ArgumentError("Parameter is null");

	if (this->collisionShapes == nullptr) {
		this->collisionShapes = new QVector<CollisionShape*>;
		this->numCollisionShapes = 0;
	}
	this->collisionShapes->append(collisionShape);
	this->numCollisionShapes = this->collisionShapes->count();
	collisionShape->setBody(this, matrix);
}


void Body::setMaxSpeedXY(double maxSpeedXY)
{
	this->maxSpeedXY->setNumber(maxSpeedXY);
}


void Body::interpolate(double param1, Vector3 *param2, Quaternion *param3)
{
	double v = 1. - param1;
	param2->setX((this->prevState.position.getX() * v) + (this->state.position.getX() * param1));
	param2->setY((this->prevState.position.getY() * v) + (this->state.position.getY() * param1));
	param2->setZ((this->prevState.position.getZ() * v) + (this->state.position.getZ() * param1));
	param3->w = (this->prevState.orientation.w * v) + (this->state.orientation.w * param1);
	param3->x = (this->prevState.orientation.x * v) + (this->state.orientation.x * param1);
	param3->y = (this->prevState.orientation.y * v) + (this->state.orientation.y * param1);
	param3->z = (this->prevState.orientation.z * v) + (this->state.orientation.z * param1);
}


void Body::addForceXYZ(double x, double y, double z)
{
	this->forceAccum.setX(this->forceAccum.getX() + x);
	this->forceAccum.setY(this->forceAccum.getY() + y);
	this->forceAccum.setZ(this->forceAccum.getZ() + z);
}


void Body::addWorldForceAtLocalPoint(const Vector3 *param1, Vector3 *param2)
{
	Vector3 vector;
	this->baseMatrix.transformVector(param1, &vector);
	this->forceAccum.add(param2);
	vector.cross(param2);
	this->torqueAccum.add(&vector);
}


void Body::addWorldForceScaled(const Vector3 *param1, const Vector3 *param2, double v)
{
	double X = v * param2->getX();
	double Y = v * param2->getY();
	double Z = v * param2->getZ();
	this->forceAccum.setX(this->forceAccum.getX() + X);
	this->forceAccum.setY(this->forceAccum.getY() + Y);
	this->forceAccum.setZ(this->forceAccum.getZ() + Z);
	Vector3 *position = &this->state.position;
	double X2 = param1->getX() - position->getX();
	double Y2 = param1->getY() - position->getY();
	double Z2 = param1->getZ() - position->getZ();
	this->torqueAccum.setX(this->torqueAccum.getX() + (((Y2 * Z) - (Z2 * Y))));
	this->torqueAccum.setY(this->torqueAccum.getY() + (((Z2 * X) - (X2 * Z))));
	this->torqueAccum.setZ(this->torqueAccum.getZ() + (((X2 * Y) - (Y2 * X))));
}
