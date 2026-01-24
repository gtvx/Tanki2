#include "IslandsGenerator.h"
#include "ContactIsland.h"
#include "game/alternativa/physics/BodyContact.h"
#include "game/alternativa/physics/Body.h"


IslandsGenerator::IslandsGenerator(PhysicsScene *physicsScene)
{
	this->physicsScene = physicsScene;
}


void IslandsGenerator::generate(MyList<BodyContact*> *param1, int param2)
{
	this->createUnions(param1,param2);
	this->createIslands(param1);
}


void IslandsGenerator::clear()
{
	int count = this->contactIslands.count();
	int index = 0;
	while (index < count)
	{
		this->contactIslands.at(index)->dispose();
		index++;
	}
	this->contactIslands.clear();
}


void IslandsGenerator::createUnions(MyList<BodyContact*> *vector, int param2)
{
	this->uf.init(param2);
	int count = vector->count();
	int index = 0;
	while (index < count)
	{
		BodyContact *bodyContact = vector->at(index);
		if (bodyContact->body1->movable && bodyContact->body2->movable)
		{
			this->uf._union(bodyContact->body1->id, bodyContact->body2->id);
		}
		index++;
	}
}


void IslandsGenerator::createIslands(MyList<BodyContact*> *param1)
{
	const int count = param1->count();
	int index = 0;
	while (index < count)
	{
		int _loc7_;
		BodyContact *bodyContact = param1->at(index);
		if (bodyContact->body1->movable)
		{
			_loc7_ = this->uf.root(bodyContact->body1->id);
		}
		else
		{
			_loc7_ = this->uf.root(bodyContact->body2->id);
		}

		ContactIsland *_loc8_;
		if (this->contactIslandsByRootId.contains(_loc7_) == false)
		{
			_loc8_ = ContactIsland::create();
			this->contactIslands.append(_loc8_);
			this->contactIslandsByRootId[_loc7_] = _loc8_;
		} else {
			 _loc8_ = this->contactIslandsByRootId[_loc7_];
		}
		_loc8_->bodyContacts.append(bodyContact);
		index++;
	}
	int _loc4_ = this->contactIslands.count();
	int _loc5_ = 0;
	while (_loc5_ < _loc4_)
	{
		ContactIsland *_loc8_ = this->contactIslands.at(_loc5_);
		_loc8_->init(this->physicsScene);
		_loc5_++;
	}

	this->contactIslandsByRootId.clear();
}
