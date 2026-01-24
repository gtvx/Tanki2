#ifndef CONTACTISLAND_H
#define CONTACTISLAND_H

#include "MyList.h"
#include "ContactLevels.h"

class ShapeContact;
class Body;
class PhysicsScene;
class Vector3;

class ContactIsland
{
public:
	MyList<BodyContact*> bodyContacts;

	ContactIsland();
	~ContactIsland();

	void contactPhase(int param1);
	void collisionPhase(int param1);
	void dispose();
	void init(PhysicsScene *param1);
	static ContactIsland *create();

private:

	ContactLevels contactLevels;
	MyList<BodyContact*> levelBodyContacts;
	MyList<Body*> *prevLevelBodies;
	MyList<Body*> *levelBodies;
	MyList<ShapeContact*> levelShapeContacts;
	PhysicsScene *physicsScene;
	MyList<ShapeContact*> allShapeContacts;

	void resolveContacts(int param1);
	void resolveCollisions(int param1);
	void resolveContact(ShapeContact *param1, int param2);
	void processContacts(int param1);
	void getShapeContacts(MyList<BodyContact *> *param1, MyList<ShapeContact *> *param2);
	void resolveContactsForLevel(int param1, MyList<ShapeContact *> *param2);
	void calculatePseudoVelocities(int param1, MyList<ShapeContact *> *param2);
	void setBodiesMobility(MyList<Body *> *param1, bool param2);
	void shuffleContacts(MyList<ShapeContact *> *param1);
	void resolveContactPseudoVelocity(ShapeContact *param1);
	void calcPseudoSeparationVelocity(ShapeContact *param1, Vector3 *param2);
	void calculateRelativeVelocity(ShapeContact *param1, Vector3 *param2);
};

#endif // CONTACTISLAND_H
