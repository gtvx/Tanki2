#include "PhysicsScene.h"
#include "game/alternativa/math/Vector3.h"
#include "Body.h"
#include "BodyState.h"
#include "game/alternativa/tanks/utils/EncryptedIntImpl.h"
#include "game/alternativa/physics/contactislands/IslandsGenerator.h"
#include "game/alternativa/physics/collision/CollisionDetector.h"
#include "BodyContact.h"
#include "ShapeContact.h"
#include "game/alternativa/physics/contactislands/ContactIsland.h"


static const EncryptedIntImpl thousandth(1000);


PhysicsScene::PhysicsScene()
{
	this->islandsGenerator = new IslandsGenerator(this);

	gravity = new Vector3(0, 0, -9.8);
	collisionIterations = 4;
	contactIterations = 4;
	allowedPenetration = 0.01;
	time = 0;
	linSpeedFreezeLimit = 5;
	freezeSteps = 10;
	angSpeedFreezeLimit = 0.05;
	timeStamp = 0;
	dt = 0;
	penetrationErrorCorrection = 0.7;
	maxCorrectablePenetration = 10;
}


void PhysicsScene::update(int dt)
{
	if (this->bodies.count() == 0) {
		qDebug("this->bodies.count() == 0");
		return;
	}

	this->timeStamp++;
	this->time = this->time + dt;
	this->dt = double(dt) / thousandth.getInt();
	this->applyForces();
	this->detectCollisions();
	this->prepareBodyContacts(&this->bodyContacts, this->dt);
	this->islandsGenerator->generate(&this->bodyContacts, this->bodies.count());
	this->resolveCollisions(&this->islandsGenerator->contactIslands);
	this->intergateVelocities(this->dt);
	this->resolveContacts(&this->islandsGenerator->contactIslands);
	this->islandsGenerator->clear();
	this->disposeBodyContacts(&this->bodyContacts);
	this->integratePositions(this->dt);
	this->postPhysics();
}


void PhysicsScene::postPhysics()
{
	int count = this->bodies.count();
	int index = 0;
	while (index < count)
	{
		Body *body = this->bodies.at(index);
		body->clearAccumulators();
		body->calcDerivedData();

		if (body->canFreeze && !body->frozen)
		{
			BodyState *bodyState = &body->state;
			if (bodyState->velocity.length() < this->linSpeedFreezeLimit && bodyState->angularVelocity.length() < this->angSpeedFreezeLimit)
			{
				body->freezeCounter++;
				if (body->freezeCounter >= this->freezeSteps)
				{
					body->frozen = true;
				}
			}
			else
			{
				body->freezeCounter = 0;
				body->frozen = false;
			}
		}

		index++;
	}
}


void PhysicsScene::applyForces()
{
	int count = this->bodies.count();
	int index = 0;
	while (index < count)
	{
		Body *body = this->bodies.at(index);
		body->calcAccelerations();
		if (body->movable && !body->frozen)
		{
			body->acceleration.add(this->gravity);
		}
		index++;
	}
}


void PhysicsScene::detectCollisions()
{
	this->calculateBodiesDerivedData();
	this->collisionDetector->getBodyContacts(&this->bodyContacts);
}


void PhysicsScene::calculateBodiesDerivedData()
{
	int count = this->bodies.count();
	int index = 0;
	while (index < count)
	{
		Body *body = this->bodies.at(index);
		if (!body->frozen)
		{
			body->saveState();
			body->calcDerivedData();
		}
		index++;
	}
}


void PhysicsScene::prepareBodyContacts(MyList<BodyContact*> *vector, double dt)
{
	int count = vector->count();
	int index = 0;
	while (index < count)
	{
		BodyContact *bodyContact = vector->at(index);
		this->prepareShapeContacts(bodyContact->shapeContacts, dt);
		index++;
	}
}


void PhysicsScene::resolveCollisions(MyList<ContactIsland*> *vector)
{
	int count = vector->count();
	int index = 0;
	while (index < count)
	{
		vector->at(index)->collisionPhase(this->collisionIterations);;
		index++;
	}
}


void PhysicsScene::intergateVelocities(double dt)
{
	int count = this->bodies.count();
	int index = 0;
	while (index < count)
	{
		this->bodies.at(index)->integrateVelocity(dt);
		index++;
	}
}


void PhysicsScene::resolveContacts(MyList<ContactIsland*> *param1)
{
	int count = param1->count();
	int index = 0;
	while (index < count)
	{
		param1->at(index)->contactPhase(this->contactIterations);
		index++;
	}
}


void PhysicsScene::disposeBodyContacts(MyList<BodyContact*> *param1)
{
	int count = param1->count();
	int index = 0;
	while (index < count)
	{
		BodyContact *bodyContact = param1->at(index);
		bodyContact->dispose();
		index++;
	}
	param1->clear();
}


void PhysicsScene::integratePositions(double param1)
{
	int count = this->bodies.count();
	int index = 0;
	while (index < count)
	{
		Body *body = this->bodies.at(index);
		if (body->movable && !body->frozen)
		{
			body->integratePosition(param1);
			body->integratePseudoVelocity(param1);
		}
		index++;
	}
}


void PhysicsScene::prepareShapeContacts(MyList<ShapeContact*> *vector, double dt)
{
	int count = vector->count();
	int index = 0;
	while (index < count)
	{
		ShapeContact *shapeContact = vector->at(index);
		shapeContact->calculatePersistentFrameData();
		shapeContact->calcualteDynamicFrameData(this->allowedPenetration, this->penetrationErrorCorrection, this->maxCorrectablePenetration, dt);
		index++;
	}
}


void PhysicsScene::addBody(Body *body)
{
	body->scene = this;
	body->id = this->bodies.count();
	this->bodies.append(body);
}


void PhysicsScene::removeBody(Body *_body)
{
	int id = this->bodies.indexOf(_body);
	if (id > -1)
	{
		int count_1 = this->bodies.count() - 1;
		Body *body = this->bodies.at(count_1);
		this->bodies.set(id, body);
		body->id = id;
		this->bodies.resize(count_1);
		_body->scene = nullptr;
	}
}
