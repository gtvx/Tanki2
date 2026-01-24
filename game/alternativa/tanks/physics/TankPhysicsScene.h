#ifndef TANKPHYSICSSCENE_H
#define TANKPHYSICSSCENE_H

#include "MyList.h"

class TanksCollisionDetector;
class Vector3Validator;
class PhysicsScene;
class BattleEventDispatcher;
class TankBody;
class CollisionShape;

class TankPhysicsScene
{
public:
	TankPhysicsScene();
	TankPhysicsScene(int param1, double gravity, BattleEventDispatcher *param3);

	void update(int param1);
	int getPhysicsTime();
	void addBody(TankBody *tankbody);
	void initStaticGeometry(MyList<CollisionShape *> *param1);
	void removeBody(TankBody *tankBody);
	TanksCollisionDetector *getCollisionDetector();

private:
	void postPhysicsTankProcessor();

	TanksCollisionDetector *collisionDetector;
	Vector3Validator *gravityValidator;
	PhysicsScene *physicsScene;
};

#endif // TANKPHYSICSSCENE_H
