#include "TankPhysicsScene.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/physics/Body.h"
#include "TankBody.h"
#include "game/alternativa/physics/BodyState.h"
#include "game/alternativa/math/Quaternion.h"
#include "MyMath.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "TanksCollisionDetector.h"
#include "game/alternativa/tanks/utils/Vector3Validator.h"
#include "game/alternativa/physics/PhysicsScene.h"


static const double MAX_UP_VELOCITY = 200;


TankPhysicsScene::TankPhysicsScene(int time, double gravity, BattleEventDispatcher*)
{
	//this->battleEventDispatcher = param3;
	this->physicsScene = new PhysicsScene();
	this->physicsScene->gravity->reset(0, 0, -gravity);
	this->gravityValidator = new Vector3Validator(this->physicsScene->gravity);
	this->physicsScene->collisionIterations = 4;
	this->physicsScene->contactIterations = 4;
	this->physicsScene->allowedPenetration = 5;
	this->collisionDetector = new TanksCollisionDetector();
	this->physicsScene->collisionDetector = this->collisionDetector;
	this->physicsScene->time = time;
}


int TankPhysicsScene::getPhysicsTime()
{
	return this->physicsScene->time;
}


void TankPhysicsScene::update(int param1)
{
	if (this->gravityValidator->isValid()) {
		this->physicsScene->update(param1);
		this->postPhysicsTankProcessor();
	} else {

	}
}


void TankPhysicsScene::postPhysicsTankProcessor()
{
	MyList<TankBody*> *vector_tankbody = this->collisionDetector->getTankBodies();
	for (int i = 0; i < vector_tankbody->count(); i++)
	{
		TankBody *_loc1_ = vector_tankbody->at(i);
		Body *_loc2_ = _loc1_->body;
		Vector3 *_loc3_ = &_loc2_->state.velocity;

		Vector3 _loc4_;
		_loc2_->state.orientation.getZAxis(&_loc4_);
		double _loc5_ = (((_loc3_->getX() * _loc4_.getX()) + (_loc3_->getY() * _loc4_.getY())) + (_loc3_->getZ() * _loc4_.getZ()));
		if (_loc4_.getZ() < -0.1 || (_loc4_.getZ() < 0.1 && _loc5_ < 0))
		{
			_loc5_ = -_loc5_;
			_loc4_.reverse();
		}
		_loc1_->updateSoaring();
		if (_loc1_->hasContactsWithStatic || _loc1_->wasContactWithStatic || _loc1_->hasContactsWithOtherBodies() || !_loc1_->isSoaring())
		{
			double _loc6_ = MAX_UP_VELOCITY;
			if (!_loc1_->hasContactsWithStatic && _loc1_->isSoaring() && _loc1_->previousUpVelocity > _loc6_)
			{
				_loc6_ = _loc1_->previousUpVelocity;
			}
			double _loc7_ = _loc1_->additionForcesSum.dot(&_loc4_);
			double _loc8_ = (((_loc7_ * _loc1_->body->invMass) * BattleRunner::PHYSICS_STEP_IN_MS) / 1000);
			if (_loc8_ < MAX_UP_VELOCITY)
			{
				_loc8_ = 0;
			}
			_loc6_ = MyMath::max(_loc8_, _loc6_);
			if (_loc5_ > _loc6_)
			{
				double _loc9_ = _loc5_ - _loc6_;
				_loc3_->setX(_loc3_->getX() - (_loc9_ * _loc4_.getX()));
				_loc3_->setY(_loc3_->getY() - (_loc9_ * _loc4_.getY()));
				_loc3_->setZ(_loc3_->getZ() - (_loc9_ * _loc4_.getZ()));
				_loc5_ = _loc6_;
			}
		}
		_loc1_->previousUpVelocity = _loc5_;
		_loc1_->additionForcesSum.reset();
	}
}


void TankPhysicsScene::addBody(TankBody *tankbody)
{
	this->physicsScene->addBody(tankbody->body);
	this->collisionDetector->addTankBody(tankbody);
}


void TankPhysicsScene::initStaticGeometry(MyList<CollisionShape*> *param1)
{
	this->collisionDetector->buildKdTree(param1);
}


TanksCollisionDetector* TankPhysicsScene::getCollisionDetector()
{
	return this->collisionDetector;
}


void TankPhysicsScene::removeBody(TankBody *tankBody)
{
	this->physicsScene->removeBody(tankBody->body);
	this->collisionDetector->removeTankBody(tankBody);
}
