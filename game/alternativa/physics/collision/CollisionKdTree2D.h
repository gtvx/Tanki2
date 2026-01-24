#ifndef COLLISIONKDTREE2D_H
#define COLLISIONKDTREE2D_H

#include <QVector>

class CollisionKdNode;
class CollisionKdTree;

class CollisionKdTree2D
{
	double splitCoord;
	double splitCost;

public:
	CollisionKdTree2D(CollisionKdTree*, CollisionKdNode*);
	~CollisionKdTree2D();
	void createTree();

	CollisionKdNode *rootNode;
	CollisionKdTree *parentTree;
	CollisionKdNode *parentNode;
	int minPrimitivesPerNode;
	double threshold;
	int splitAxis;

private:
	void splitNode(CollisionKdNode*);
	void checkNodeAxis(CollisionKdNode *param1, int param2, int param3, QVector<double> *param4, double *param5);
};

#endif // COLLISIONKDTREE2D_H
