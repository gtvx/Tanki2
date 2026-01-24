#ifndef ISLANDSGENERATOR_H
#define ISLANDSGENERATOR_H

#include "game/alternativa/physics/QuickUnionFind.h"
#include "MyList.h"
#include <QHash>

class BodyContact;
class ContactIsland;
class PhysicsScene;

class IslandsGenerator
{
	QuickUnionFind uf;
	PhysicsScene *physicsScene;

public:
	IslandsGenerator(PhysicsScene *physicsScene);
	void generate(MyList<BodyContact *> *param1, int param2);	
	void clear();
	void createUnions(MyList<BodyContact *> *param1, int param2);
	void createIslands(MyList<BodyContact *> *param1);

	MyList<ContactIsland*> contactIslands;
	QHash<int, ContactIsland*> contactIslandsByRootId;
};

#endif // ISLANDSGENERATOR_H
