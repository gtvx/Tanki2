#include "CollisionKdNode.h"
#include "_global.h"
#include "CollisionKdTree2D.h"


CollisionKdNode::CollisionKdNode()
{
	axis = -1;
	indices = nullptr;
	negativeNode = nullptr;
	positiveNode = nullptr;
	splitTree = nullptr;
	coord = NaN();
	//boundBox = nullptr;
	parent = nullptr;
	splitIndices = nullptr;
}


CollisionKdNode::~CollisionKdNode()
{
	if (indices != nullptr)
		delete indices;
	if (negativeNode != nullptr)
		delete negativeNode;
	if (positiveNode != nullptr)
		delete positiveNode;
	if (splitTree != nullptr)
		delete splitTree;
	if (splitIndices != nullptr)
		delete splitIndices;
}
