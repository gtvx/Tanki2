#ifndef BODYCONTACT_H
#define BODYCONTACT_H

#include "MyList.h"

class ShapeContact;
class Body;

class BodyContact
{
	BodyContact *nextInPool;

public:
	BodyContact();
	MyList<ShapeContact*> *shapeContacts;
	void dispose();
	Body *body1;
	Body *body2;
	static BodyContact *create();
	void setShapeContacts(MyList<ShapeContact *> *param1);
};

#endif // BODYCONTACT_H
