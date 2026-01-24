#ifndef CONTACTLEVELS_H
#define CONTACTLEVELS_H

#include "MyList.h"

class BodyContact;
class Body;

class ContactLevels
{
public:
	ContactLevels();

	void init(MyList<BodyContact *> *param1);
	void clear();
	void getStaticLevel(MyList<BodyContact *> *param1, MyList<Body *> *param2);
	void getNextLevel(MyList<Body *> *param1, MyList<BodyContact *> *param2, MyList<Body *> *param3);
	bool hasContacts();

private:
	bool isStaticContact(BodyContact *param1);
	Body *getNonStaticBody(BodyContact *param1);
	void removeContact(int param1);
	bool isInContactWith(MyList<Body *> *param1, BodyContact *param2);
	Body *getNextLevelBody(BodyContact *param1, MyList<Body *> *param2);

	MyList<BodyContact*> contacts;
};

#endif // CONTACTLEVELS_H
