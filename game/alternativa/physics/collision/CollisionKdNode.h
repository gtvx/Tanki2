#ifndef COLLISIONKDNODE_H
#define COLLISIONKDNODE_H

#include <QVector>
#include "game/alternativa/physics/collision/types/AABB.h"

class CollisionKdTree2D;

class CollisionKdNode
{
public:
	CollisionKdNode();
	~CollisionKdNode();

	QVector<int> *indices;
	int axis;
	CollisionKdNode *negativeNode;
	CollisionKdNode *positiveNode;
	CollisionKdTree2D *splitTree;
	double coord;
	AABB boundBox;
	CollisionKdNode *parent;
	QVector<int> *splitIndices;
};

#endif // COLLISIONKDNODE_H
