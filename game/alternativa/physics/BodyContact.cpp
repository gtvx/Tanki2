#include "BodyContact.h"
#include "ShapeContact.h"

static BodyContact *poolTop = nullptr;


BodyContact::BodyContact()
{
	nextInPool = nullptr;
	this->shapeContacts = new MyList<ShapeContact*>;
}


void BodyContact::dispose()
{
	this->body1 = nullptr;
	this->body2 = nullptr;
	int count = this->shapeContacts->count();
	int index = 0;
	while (index < count)
	{
		ShapeContact *v = this->shapeContacts->at(index);
		v->dispose();
		index++;
	}
	this->shapeContacts->clear();
	this->nextInPool = poolTop;
	poolTop = this;
}


BodyContact* BodyContact::create()
{
	if (poolTop == nullptr)
		return new BodyContact();

	BodyContact *p = poolTop;
	poolTop = poolTop->nextInPool;
	p->nextInPool = nullptr;
	return p;
}


void BodyContact::setShapeContacts(MyList<ShapeContact*> *param1)
{
	int count = param1->count();
	int index = 0;
	while (index < count)
	{
		this->shapeContacts->append(param1->at(index));
		index++;
	}
}
