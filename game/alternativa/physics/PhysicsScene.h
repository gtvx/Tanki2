#ifndef PHYSICSSCENE_H
#define PHYSICSSCENE_H

class Vector3;
class CollisionDetector;
class Body;
class IslandsGenerator;
class ContactIsland;
class BodyContact;
class ShapeContact;
#include <QVector>
#include "MyList.h"

class PhysicsScene
{
public:
	PhysicsScene();
	void update(int);
	Vector3 *gravity;
	int collisionIterations;
	int contactIterations;
	double allowedPenetration;
	int time;
	CollisionDetector *collisionDetector;
	MyList<Body*> bodies;
	double linSpeedFreezeLimit;
	int freezeSteps;
	double angSpeedFreezeLimit;
	int timeStamp;
	double dt;
	MyList<BodyContact*> bodyContacts;
	double penetrationErrorCorrection;
	double maxCorrectablePenetration;
	void addBody(Body *body);

	void removeBody(Body *body);
private:
	void postPhysics();
	void applyForces();
	void calculateBodiesDerivedData();
	void prepareBodyContacts(MyList<BodyContact *>*, double);
	void detectCollisions();
	IslandsGenerator *islandsGenerator;
	void resolveCollisions(MyList<ContactIsland *>*);
	void intergateVelocities(double dt);
	void resolveContacts(MyList<ContactIsland *>*);
	void disposeBodyContacts(MyList<BodyContact *>*);
	void integratePositions(double);
	void prepareShapeContacts(MyList<ShapeContact *>*, double);
};

#endif // PHYSICSSCENE_H
