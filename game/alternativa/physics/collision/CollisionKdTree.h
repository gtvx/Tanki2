#ifndef COLLISIONKDTREE_H
#define COLLISIONKDTREE_H

#include "MyList.h"
#include <QVector>

class CollisionKdNode;
class CollisionShape;
class AABB;

class CollisionKdTree
{
public:
	CollisionKdTree();
	CollisionKdNode *rootNode;
	MyList<CollisionShape*> staticChildren;
	int numStaticChildren;
	MyList<AABB*> staticBoundBoxes;
	int minPrimitivesPerNode;
	double threshold;
	double splitCost;

	void createTree(MyList<CollisionShape *> *param1);
	void reset();

private:

	int splitAxis;
	double splitCoord;

	void splitNode(CollisionKdNode *param1);
	void checkNodeAxis(const CollisionKdNode *param1, int param2, int param3, QVector<double> *param4, const double *param5);
};

#endif // COLLISIONKDTREE_H
