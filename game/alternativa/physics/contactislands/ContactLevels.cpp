#include "ContactLevels.h"
#include "game/alternativa/physics/BodyContact.h"
#include "game/alternativa/physics/Body.h"


ContactLevels::ContactLevels()
{

}


void ContactLevels::init(MyList<BodyContact*> *param1)
{
	this->contacts.clear();
	int count = param1->count();
	this->contacts.resize(count);
	int index = 0;
	while (index < count)
	{
		this->contacts.set(index, param1->at(index));
		index++;
	}
}


void ContactLevels::clear()
{
	this->contacts.clear();
}


void ContactLevels::getStaticLevel(MyList<BodyContact*> *param1, MyList<Body*> *param2)
{
	int index = 0;
	while (index < this->contacts.count())
	{
		BodyContact *_loc4_ = this->contacts.at(index);
		if (this->isStaticContact(_loc4_))
		{
			param1->append(_loc4_);
			param2->append(this->getNonStaticBody(_loc4_));
			this->removeContact(index);
			index--;
		}
		index++;
	}
	index = 0;
	while (index < this->contacts.count())
	{
		BodyContact *_loc4_ = this->contacts.at(index);
		if (param2->indexOf(_loc4_->body1) >= 0 && param2->indexOf(_loc4_->body2) >= 0)
		{
			param1->append(_loc4_);
			this->removeContact(index);
			index--;
		}
		index++;
	}
}



bool ContactLevels::isStaticContact(BodyContact *param1)
{
	return !(param1->body1->movable && param1->body2->movable);
}


Body* ContactLevels::getNonStaticBody(BodyContact *param1)
{
	if (param1->body1->movable) {
		return param1->body1;
	}
	return param1->body2;
}


void ContactLevels::removeContact(int param1)
{
	int _loc2_ = this->contacts.count() - 1;
	this->contacts.set(param1, this->contacts.at(_loc2_));
	this->contacts.resize(_loc2_);
}


void ContactLevels::getNextLevel(MyList<Body*> *param1, MyList<BodyContact*> *param2, MyList<Body*> *param3)
{
	int index = 0;
	while (index < this->contacts.count())
	{
		BodyContact *_loc5_ = this->contacts.at(index);
		if (this->isInContactWith(param1,_loc5_))
		{
			param2->append(_loc5_);
			param3->append(this->getNextLevelBody(_loc5_,param1));
			this->removeContact(index);
			index--;
		}
		index++;
	}
	index = 0;
	while (index < this->contacts.count())
	{
		BodyContact *_loc5_ = this->contacts.at(index);
		if (param3->indexOf(_loc5_->body1) >= 0 && param3->indexOf(_loc5_->body2) >= 0)
		{
			param2->append(_loc5_);
			this->removeContact(index);
			index--;
		}
		index++;
	}
}


bool ContactLevels::isInContactWith(MyList<Body*> *param1, BodyContact *param2)
{
	return param1->indexOf(param2->body1) >= 0 || param1->indexOf(param2->body2) >= 0;
}


Body* ContactLevels::getNextLevelBody(BodyContact *param1, MyList<Body*> *param2)
{
	if (param2->indexOf(param1->body1) < 0) {
		return param1->body1;
	}
	return param1->body2;
}


bool ContactLevels::hasContacts()
{
	return this->contacts.count() > 0;
}
