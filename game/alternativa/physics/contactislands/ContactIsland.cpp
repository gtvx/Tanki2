#include "ContactIsland.h"
#include "game/alternativa/physics/ShapeContact.h"
#include "game/alternativa/physics/PhysicsScene.h"
#include "MyMath.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/physics/BodyContact.h"

static const int COLLISION_MODE = 0;
static const int CONTACT_MODE = 1;
static Vector3 _relativeVelocity;
static int poolSize = 0;
static MyList<ContactIsland*> pool;


ContactIsland::ContactIsland()
{
	prevLevelBodies = new MyList<Body*>;
	levelBodies = new MyList<Body*>;
	physicsScene = nullptr;
}


ContactIsland::~ContactIsland()
{
	delete prevLevelBodies;
	delete levelBodies;
}


void ContactIsland::contactPhase(int param1)
{
	this->resolveContacts(param1);
}


void ContactIsland::collisionPhase(int param1)
{
	this->resolveCollisions(param1);
}


void ContactIsland::dispose()
{
	this->physicsScene = nullptr;
	this->bodyContacts.clear();
	this->allShapeContacts.clear();
	this->levelShapeContacts.clear();
	this->prevLevelBodies->clear();
	this->levelBodies->clear();
	this->levelBodyContacts.clear();
	this->contactLevels.clear();

	pool.add_set(poolSize, this);
	poolSize++;
}


void ContactIsland::resolveContacts(int param1)
{
	this->processContacts(param1);
	this->contactLevels.init(&this->bodyContacts);
	this->contactLevels.getStaticLevel(&this->levelBodyContacts, this->levelBodies);
	if (this->levelBodyContacts.count() > 0)
	{
		this->getShapeContacts(&this->levelBodyContacts, &this->levelShapeContacts);
		this->resolveContactsForLevel(param1, &this->levelShapeContacts);
		this->calculatePseudoVelocities(param1, &this->levelShapeContacts);
		while (this->contactLevels.hasContacts())
		{
			{
				//swap
				auto v = this->prevLevelBodies;
				this->prevLevelBodies = this->levelBodies;
				this->levelBodies = v;
			}


			this->levelBodyContacts.clear();
			this->levelBodies->clear();
			this->contactLevels.getNextLevel(this->prevLevelBodies, &this->levelBodyContacts, this->levelBodies);
			this->setBodiesMobility(this->prevLevelBodies, false);
			this->levelShapeContacts.clear();
			this->getShapeContacts(&this->levelBodyContacts, &this->levelShapeContacts);
			int count = this->levelShapeContacts.count();
			int index = 0;
			while (index < count)
			{
				ShapeContact *_loc5_ = this->levelShapeContacts.at(index);
				_loc5_->calcualteDynamicFrameData(this->physicsScene->allowedPenetration, this->physicsScene->penetrationErrorCorrection,
												  this->physicsScene->maxCorrectablePenetration, this->physicsScene->dt);
				index++;
			}
			this->resolveContactsForLevel(param1, &this->levelShapeContacts);
			this->calculatePseudoVelocities(param1, &this->levelShapeContacts);
			this->setBodiesMobility(this->prevLevelBodies, true);
		}
	}
	else
	{
		this->getShapeContacts(&this->bodyContacts, &this->levelShapeContacts);
		this->resolveContactsForLevel(param1, &this->levelShapeContacts);
		this->calculatePseudoVelocities(param1, &this->levelShapeContacts);
	}
}


void ContactIsland::resolveCollisions(int param1)
{
	int _loc4_ = 0;
	int _loc2_ = this->allShapeContacts.count();
	int _loc3_ = 0;
	while (_loc3_ < param1)
	{
		this->shuffleContacts(&this->allShapeContacts);
		_loc4_ = 0;
		while (_loc4_ < _loc2_)
		{
			this->resolveContact(this->allShapeContacts.at(_loc4_), COLLISION_MODE);
			_loc4_++;
		}
		_loc3_++;
	}
}


void ContactIsland::resolveContact(ShapeContact *param1, int param2)
{
	double _loc8_;
	Vector3 *_loc3_ = &param1->normal;
	Body *_loc4_ = param1->shape1->body;
	Body *_loc5_ = param1->shape2->body;
	Vector3 *_loc6_ = &_relativeVelocity;
	this->calculateRelativeVelocity(param1, _loc6_);
	double _loc7_ = _loc6_->getX() * _loc3_->getX() + _loc6_->getY() * _loc3_->getY() + _loc6_->getZ() * _loc3_->getZ();
	if (param2 == CONTACT_MODE)
	{
		_loc8_ = 0;
		if (_loc7_ < 0)
		{
			param1->satisfied = false;
		}
		else if (param1->satisfied)
		{
			return;
		}
	}
	else
	{
		param1->satisfied = true;
		_loc8_ = param1->collisionSpeed;
	}
	double _loc9_ = _loc6_->dot(&param1->tangent1);
	double _loc10_ = _loc6_->dot(&param1->tangent2);
	double _loc11_ = param1->tangentImpulse1 - _loc9_ / param1->tangentSpeedDelta1;
	double _loc12_ = param1->tangentImpulse2 - _loc10_ / param1->tangentSpeedDelta2;
	double _loc13_ = _loc11_ * _loc11_ + _loc12_ * _loc12_;
	double _loc14_ = param1->friction * param1->normalImpulse;
	if (_loc13_ > _loc14_ * _loc14_)
	{
		double _loc20_ = MyMath::sqrt(_loc13_);
		_loc11_ = _loc11_ * (_loc14_ / _loc20_);
		_loc12_ = _loc12_ * (_loc14_ / _loc20_);
	}
	double _loc15_ = _loc11_ - param1->tangentImpulse1;
	double _loc16_ = _loc12_ - param1->tangentImpulse2;
	param1->tangentImpulse1 = _loc11_;
	param1->tangentImpulse2 = _loc12_;
	if (_loc4_->movable)
	{
		_loc4_->applyWorldImpulseAtLocalPoint(&param1->r1, &param1->tangent1,_loc15_);
		_loc4_->applyWorldImpulseAtLocalPoint(&param1->r1, &param1->tangent2,_loc16_);
	}
	if (_loc5_->movable)
	{
		_loc5_->applyWorldImpulseAtLocalPoint(&param1->r2, &param1->tangent1,-_loc15_);
		_loc5_->applyWorldImpulseAtLocalPoint(&param1->r2, &param1->tangent2,-_loc16_);
	}
	this->calculateRelativeVelocity(param1,_loc6_);
	_loc7_ = _loc6_->getX() * _loc3_->getX() + _loc6_->getY() * _loc3_->getY() + _loc6_->getZ() * _loc3_->getZ();
	double _loc17_ = _loc8_ - _loc7_;
	double _loc18_ = param1->normalImpulse + _loc17_ / param1->normalSpeedDelta;
	if (_loc18_ < 0)
	{
		_loc18_ = 0;
	}
	double _loc19_ = _loc18_ - param1->normalImpulse;
	param1->normalImpulse = _loc18_;
	if (_loc4_->movable)
	{
		_loc4_->applyWorldImpulseAtLocalPoint(&param1->r1, &param1->normal,_loc19_);
	}
	if (_loc5_->movable)
	{
		_loc5_->applyWorldImpulseAtLocalPoint(&param1->r2, &param1->normal,-_loc19_);
	}
}


void ContactIsland::processContacts(int param1)
{
	int _loc2_ = this->allShapeContacts.count();
	int _loc3_ = 0;
	while (_loc3_ < param1)
	{
		this->shuffleContacts(&this->allShapeContacts);
		int _loc4_ = 0;
		while (_loc4_ < _loc2_)
		{
			this->resolveContact(this->allShapeContacts.at(_loc4_), CONTACT_MODE);
			_loc4_++;
		}
		_loc3_++;
	}
}


void ContactIsland::setBodiesMobility(MyList<Body*> *param1, bool param2)
{
	int _loc3_ = param1->count();
	int _loc4_ = 0;
	while (_loc4_ < _loc3_)
	{
		Body *_loc5_ = param1->at(_loc4_);
		_loc5_->movable = param2;
		_loc4_++;
	}
}


void ContactIsland::getShapeContacts(MyList<BodyContact*> *param1, MyList<ShapeContact*> *param2)
{
	int count = param1->count();
	int index = 0;
	while (index < count)
	{
		BodyContact *_loc5_ = param1->at(index);
		MyList<ShapeContact*> *_loc6_ = _loc5_->shapeContacts;
		int _loc7_ = _loc6_->count();
		int _loc8_ = 0;
		while (_loc8_ < _loc7_)
		{
			param2->append(_loc6_->at(_loc8_));
			_loc8_++;
		}
		index++;
	}
}


void ContactIsland::resolveContactsForLevel(int param1, MyList<ShapeContact*> *param2)
{
	int _loc3_ = 0;
	while (_loc3_ < param1)
	{
		this->shuffleContacts(param2);
		int _loc4_ = param2->count();
		int index = 0;
		while (index < _loc4_)
		{
			this->resolveContact(param2->at(index), CONTACT_MODE);
			index++;
		}
		_loc3_++;
	}
}


void ContactIsland::calculatePseudoVelocities(int param1, MyList<ShapeContact*> *param2)
{
	int _loc3_ = 0;
	while (_loc3_ < param1)
	{
		this->shuffleContacts(param2);
		int _loc4_ = param2->count();
		int _loc5_ = 0;
		while (_loc5_ < _loc4_)
		{
			this->resolveContactPseudoVelocity(param2->at(_loc5_));
			_loc5_++;
		}
		_loc3_++;
	}
}


void ContactIsland::shuffleContacts(MyList<ShapeContact*> *param1)
{
	int _loc2_ = param1->count();
	int index = 1;
	while (index < _loc2_)
	{
		int r = index * MyMath::random();
		ShapeContact *shapeContact = param1->at(r);
		param1->set(r, param1->at(index));
		param1->set(index, shapeContact);
		index++;
	}

}


void ContactIsland::resolveContactPseudoVelocity(ShapeContact *param1)
{
	Vector3 *_loc2_ = &_relativeVelocity;
	this->calcPseudoSeparationVelocity(param1, _loc2_);
	double _loc3_ = _loc2_->getX() * param1->normal.getX() + _loc2_->getY() * param1->normal.getY() + _loc2_->getZ() * param1->normal.getZ();
	double _loc4_ = param1->contactSeparationSpeed - _loc3_;
	double _loc5_ = _loc4_ / param1->normalSpeedDelta;
	if (param1->shape1->body->movable)
	{
		param1->shape1->body->applyWorldPseudoImpulseAtLocalPoint(&param1->r1, &param1->normal, _loc5_);
	}
	if (param1->shape2->body->movable)
	{
		param1->shape2->body->applyWorldPseudoImpulseAtLocalPoint(&param1->r2, &param1->normal, -_loc5_);
	}
}


void ContactIsland::calcPseudoSeparationVelocity(ShapeContact *param1, Vector3 *param2)
{
	Vector3 *_loc3_ = &param1->shape1->body->pseudoAngularVelocity;
	Vector3 *_loc4_ = &param1->r1;
	double _loc5_ = _loc3_->getY() * _loc4_->getZ() - _loc3_->getZ() * _loc4_->getY();
	double _loc6_ = _loc3_->getZ() * _loc4_->getX() - _loc3_->getX() * _loc4_->getZ();
	double _loc7_ = _loc3_->getX() * _loc4_->getY() - _loc3_->getY() * _loc4_->getX();
	_loc4_ = &param1->shape1->body->pseudoVelocity;
	param2->setX(_loc4_->getX() + _loc5_);
	param2->setY(_loc4_->getY() + _loc6_);
	param2->setZ(_loc4_->getZ() + _loc7_);
	_loc3_ = &param1->shape2->body->pseudoAngularVelocity;
	_loc4_ = &param1->r2;
	_loc5_ = _loc3_->getY() * _loc4_->getZ() - _loc3_->getZ() * _loc4_->getY();
	_loc6_ = _loc3_->getZ() * _loc4_->getX() - _loc3_->getX() * _loc4_->getZ();
	_loc7_ = _loc3_->getX() * _loc4_->getY() - _loc3_->getY() * _loc4_->getX();
	_loc4_ = &param1->shape2->body->pseudoVelocity;
	param2->setX(param2->getX() - (_loc4_->getX() + _loc5_));
	param2->setY(param2->getY() - (_loc4_->getY() + _loc6_));
	param2->setZ(param2->getZ() - (_loc4_->getZ() + _loc7_));
}


void ContactIsland::calculateRelativeVelocity(ShapeContact *param1, Vector3 *param2)
{
	Vector3 *_loc3_ = &param1->shape1->body->state.angularVelocity;
	Vector3 *_loc4_ = &param1->r1;
	double _loc5_ = _loc3_->getY() * _loc4_->getZ() - _loc3_->getZ() * _loc4_->getY();
	double _loc6_ = _loc3_->getZ() * _loc4_->getX() - _loc3_->getX() * _loc4_->getZ();
	double _loc7_ = _loc3_->getX() * _loc4_->getY() - _loc3_->getY() * _loc4_->getX();
	_loc4_ = &param1->shape1->body->state.velocity;
	param2->setX(_loc4_->getX() + _loc5_);
	param2->setY(_loc4_->getY() + _loc6_);
	param2->setZ(_loc4_->getZ() + _loc7_);
	_loc3_ = &param1->shape2->body->state.angularVelocity;
	_loc4_ = &param1->r2;
	_loc5_ = _loc3_->getY() * _loc4_->getZ() - _loc3_->getZ() * _loc4_->getY();
	_loc6_ = _loc3_->getZ() * _loc4_->getX() - _loc3_->getX() * _loc4_->getZ();
	_loc7_ = _loc3_->getX() * _loc4_->getY() - _loc3_->getY() * _loc4_->getX();
	_loc4_ = &param1->shape2->body->state.velocity;
	param2->setX(param2->getX() - (_loc4_->getX() + _loc5_));
	param2->setY(param2->getY() - (_loc4_->getY() + _loc6_));
	param2->setZ(param2->getZ() - (_loc4_->getZ() + _loc7_));
}


void ContactIsland::init(PhysicsScene *param1)
{
	this->allShapeContacts.clear();
	this->physicsScene = param1;
	int count = this->bodyContacts.count();
	int index = 0;
	while (index < count)
	{
		BodyContact *_loc5_ = this->bodyContacts.at(index);
		MyList<ShapeContact*> *_loc6_ = _loc5_->shapeContacts;
		int _loc7_ = _loc6_->count();
		int _loc8_ = 0;
		while (_loc8_ < _loc7_)
		{
			this->allShapeContacts.append(_loc6_->at(_loc8_));
			_loc8_++;
		}
		index++;
	}
}


ContactIsland* ContactIsland::create()
{
	if (poolSize == 0)
		return new ContactIsland();

	poolSize--;
	ContactIsland *v = pool.at(poolSize);
	pool.set(poolSize, nullptr);
	return v;
}
