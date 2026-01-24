#include "CollisionKdTree.h"
#include "game/alternativa/physics/collision/types/AABB.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "CollisionKdNode.h"
#include "CollisionKdTree2D.h"
#include "_global.h"
#include "game/alternativa/math/Vector3.h"


CollisionKdTree::CollisionKdTree()
{
	rootNode = nullptr;
	numStaticChildren = 0;
	minPrimitivesPerNode = 1;
	splitCoord = 0;
	threshold = 0.1;
	splitCost = 0;
}


void CollisionKdTree::createTree(MyList<CollisionShape*> *param1)
{
	this->staticChildren.copy(param1);
	this->numStaticChildren = this->staticChildren.count();

	this->reset();
}


void CollisionKdTree::reset()
{
	if (this->rootNode != nullptr) {
		delete this->rootNode;
	}

	this->rootNode = new CollisionKdNode();
	this->rootNode->indices = new QVector<int>;

	this->staticBoundBoxes.resize(this->numStaticChildren);
	this->rootNode->indices->resize(this->numStaticChildren);

	AABB _aabb;

	for (int i = 0; i < this->numStaticChildren; i++)
	{
		CollisionShape *collisionShape = this->staticChildren.at(i);
		AABB *aabb = collisionShape->calculateAABB();
		this->staticBoundBoxes.set(i, aabb);
		_aabb.addBoundBox(aabb);
		this->rootNode->indices->replace(i, i);
	}

	this->rootNode->boundBox.copyFrom(&_aabb);
	this->splitNode(this->rootNode);
}


void CollisionKdTree::splitNode(CollisionKdNode *collisionKdNode)
{
	QVector<int> *_loc2_ = collisionKdNode->indices;
	int _loc3_ = _loc2_->count();
	if (_loc3_ <= this->minPrimitivesPerNode)
		return;

	AABB nodeBoundBoxThreshold;

	const AABB *_loc4_ = &collisionKdNode->boundBox;
	nodeBoundBoxThreshold.minX = _loc4_->minX + this->threshold;
	nodeBoundBoxThreshold.minY = _loc4_->minY + this->threshold;
	nodeBoundBoxThreshold.minZ = _loc4_->minZ + this->threshold;
	nodeBoundBoxThreshold.maxX = _loc4_->maxX - this->threshold;
	nodeBoundBoxThreshold.maxY = _loc4_->maxY - this->threshold;
	nodeBoundBoxThreshold.maxZ = _loc4_->maxZ - this->threshold;


	QVector<double> splitCoordsX;
	QVector<double> splitCoordsY;
	QVector<double> splitCoordsZ;

	splitCoordsX.reserve(1000);
	splitCoordsY.reserve(1000);
	splitCoordsZ.reserve(1000);

	double _loc5_ = this->threshold * 2;
	int _loc6_ = 0;

	while (_loc6_ < _loc3_)
	{
		const AABB *_loc15_ = this->staticBoundBoxes.at( _loc2_->at(_loc6_) );
		if (_loc15_->maxX - _loc15_->minX <= _loc5_)
		{
			if (_loc15_->minX <= nodeBoundBoxThreshold.minX)
			{
				splitCoordsX.append(_loc4_->minX);
			}
			else if (_loc15_->maxX >= nodeBoundBoxThreshold.maxX)
			{
				splitCoordsX.append(_loc4_->maxX);
			}
			else
			{
				splitCoordsX.append((_loc15_->minX + _loc15_->maxX) * 0.5);
			}
		}
		else
		{
			if (_loc15_->minX > nodeBoundBoxThreshold.minX)
			{
				splitCoordsX.append(_loc15_->minX);
				//splitCoordsX[_loc8_++] = _loc15_->minX;
			}
			if (_loc15_->maxX < nodeBoundBoxThreshold.maxX)
			{
				splitCoordsX.append(_loc15_->maxX);
			}
		}
		if ((_loc15_->maxY - _loc15_->minY) <= _loc5_)
		{
			if (_loc15_->minY <= nodeBoundBoxThreshold.minY)
			{
				splitCoordsY.append(_loc4_->minY);
			}
			else if (_loc15_->maxY >= nodeBoundBoxThreshold.maxY)
			{
				splitCoordsY.append(_loc4_->maxY);
			}
			else
			{
				splitCoordsY.append((_loc15_->minY + _loc15_->maxY) * 0.5);
			}
		}
		else
		{
			if (_loc15_->minY > nodeBoundBoxThreshold.minY)
			{
				splitCoordsY.append(_loc15_->minY);
			}
			if (_loc15_->maxY < nodeBoundBoxThreshold.maxY)
			{
				splitCoordsY.append(_loc15_->maxY);
			}
		}
		if (_loc15_->maxZ - _loc15_->minZ <= _loc5_)
		{
			if (_loc15_->minZ <= nodeBoundBoxThreshold.minZ)
			{
				splitCoordsZ.append(_loc4_->minZ);
			}
			else if (_loc15_->maxZ >= nodeBoundBoxThreshold.maxZ)
			{
				splitCoordsZ.append(_loc4_->maxZ);
			}
			else
			{
				splitCoordsZ.append((_loc15_->minZ + _loc15_->maxZ) * 0.5);
			}
		}
		else
		{
			if (_loc15_->minZ > nodeBoundBoxThreshold.minZ)
			{
				splitCoordsZ.append(_loc15_->minZ);
			}
			if (_loc15_->maxZ < nodeBoundBoxThreshold.maxZ)
			{
				splitCoordsZ.append(_loc15_->maxZ);
			}
		}
		_loc6_++;
	}

	this->splitAxis = -1;
	this->splitCost = 1.0e308;

	double _nodeBB[6];
	_nodeBB[0] = _loc4_->minX;
	_nodeBB[1] = _loc4_->minY;
	_nodeBB[2] = _loc4_->minZ;
	_nodeBB[3] = _loc4_->maxX;
	_nodeBB[4] = _loc4_->maxY;
	_nodeBB[5] = _loc4_->maxZ;

	this->checkNodeAxis(collisionKdNode, 0, splitCoordsX.count(), &splitCoordsX, _nodeBB);
	this->checkNodeAxis(collisionKdNode, 1, splitCoordsY.count(), &splitCoordsY, _nodeBB);
	this->checkNodeAxis(collisionKdNode, 2, splitCoordsZ.count(), &splitCoordsZ, _nodeBB);

	if (this->splitAxis < 0)
		return;

	bool _loc11_ = this->splitAxis == 0;
	bool _loc12_ = this->splitAxis == 1;
	collisionKdNode->axis = this->splitAxis;
	collisionKdNode->coord = this->splitCoord;

	if (collisionKdNode->negativeNode == nullptr)
		delete collisionKdNode->negativeNode;
	if (collisionKdNode->positiveNode != nullptr)
		delete collisionKdNode->positiveNode;

	collisionKdNode->negativeNode = new CollisionKdNode();
	collisionKdNode->negativeNode->parent = collisionKdNode;
	collisionKdNode->negativeNode->boundBox.copyFrom(_loc4_);
	collisionKdNode->positiveNode = new CollisionKdNode();
	collisionKdNode->positiveNode->parent = collisionKdNode;
	collisionKdNode->positiveNode->boundBox.copyFrom(_loc4_);

	if (_loc11_)
	{
		collisionKdNode->negativeNode->boundBox.maxX = collisionKdNode->positiveNode->boundBox.minX = this->splitCoord;
	}
	else if (_loc12_)
	{
		collisionKdNode->negativeNode->boundBox.maxY = collisionKdNode->positiveNode->boundBox.minY = this->splitCoord;
	}
	else
	{
		collisionKdNode->negativeNode->boundBox.maxZ = collisionKdNode->positiveNode->boundBox.minZ = this->splitCoord;
	}

	double _loc13_ = this->splitCoord - this->threshold;
	double _loc14_ = this->splitCoord + this->threshold;
	_loc6_ = 0;
	while (_loc6_ < _loc3_)
	{
		AABB *_loc15_ = this->staticBoundBoxes.at(_loc2_->at(_loc6_));
		double _loc16_ = _loc11_ ? _loc15_->minX : (_loc12_ ? _loc15_->minY : _loc15_->minZ);
		double _loc17_ = _loc11_ ? _loc15_->maxX : (_loc12_ ? _loc15_->maxY : _loc15_->maxZ);
		if (_loc17_ <= _loc14_)
		{
			if (_loc16_ < _loc13_)
			{
				if (collisionKdNode->negativeNode->indices == nullptr) {
					collisionKdNode->negativeNode->indices = new QVector<int>();
					collisionKdNode->negativeNode->indices->reserve(100);
				}

				collisionKdNode->negativeNode->indices->append(_loc2_->at(_loc6_));
				_loc2_->replace(_loc6_, -1);
			}
			else
			{
				if (collisionKdNode->splitIndices == nullptr) {
					collisionKdNode->splitIndices = new QVector<int>();
					collisionKdNode->splitIndices->reserve(100);
				}

				collisionKdNode->splitIndices->append(_loc2_->at(_loc6_));
				_loc2_->replace(_loc6_, -1);
			}
		}
		else if (_loc16_ >= _loc13_)
		{
			if (collisionKdNode->positiveNode->indices == nullptr) {
				collisionKdNode->positiveNode->indices = new QVector<int>();
				collisionKdNode->positiveNode->indices->reserve(100);
			}

			collisionKdNode->positiveNode->indices->append(_loc2_->at(_loc6_));
			_loc2_->replace(_loc6_, -1);
		}
		_loc6_++;
	}
	_loc6_ = 0;
	int _loc7_ = 0;
	while (_loc6_ < _loc3_)
	{
		if (_loc2_->at(_loc6_) >= 0)
		{
			_loc2_->replace(_loc7_++, _loc2_->at(_loc6_));
		}
		_loc6_++;
	}
	if (_loc7_ > 0)
	{
		_loc2_->resize(_loc7_);
	}
	else
	{
		if (collisionKdNode->indices != nullptr)
			delete collisionKdNode->indices;
		collisionKdNode->indices = nullptr;
	}

	if (collisionKdNode->splitIndices != nullptr)
	{
		if (collisionKdNode->splitTree != nullptr)
			delete collisionKdNode->splitTree;
		collisionKdNode->splitTree = new CollisionKdTree2D(this, collisionKdNode);
		collisionKdNode->splitTree->createTree();
	}

	if (collisionKdNode->negativeNode->indices != nullptr)
	{
		this->splitNode(collisionKdNode->negativeNode);
	}

	if (collisionKdNode->positiveNode->indices != nullptr)
	{
		this->splitNode(collisionKdNode->positiveNode);
	}
}


void CollisionKdTree::checkNodeAxis(const CollisionKdNode *collisionKdNode, int param2, int count, QVector<double> *param4, const double *param5)
{
	int _loc6_ = (param2 + 1) % 3;
	int _loc7_ = (param2 + 2) % 3;

	std::vector<bool> filter(count, false);

	double _loc8_ = (param5[_loc6_ + 3] - param5[_loc6_]) * (param5[_loc7_ + 3] - param5[_loc7_]);
	int _loc9_ = 0;
	while (_loc9_ < count)
	{
		double _loc10_ = param4->at(_loc9_);
		if (!isNaN(_loc10_))
		{
			double _loc11_ = _loc10_ - this->threshold;
			double _loc12_ = _loc10_ + this->threshold;
			double _loc13_ = _loc8_ * (_loc10_ - param5[param2]);
			double _loc14_ = _loc8_ * (param5[int(param2 + 3)] - _loc10_);
			int _loc15_ = 0;
			int _loc16_ = 0;
			bool _loc17_ = false;
			int _loc18_ = collisionKdNode->indices->count();
			int _loc19_ = 0;
			while (_loc19_ < _loc18_)
			{
				double _bb[6];
				AABB *_loc21_ = this->staticBoundBoxes.at(collisionKdNode->indices->at(_loc19_));
				_bb[0] = _loc21_->minX;
				_bb[1] = _loc21_->minY;
				_bb[2] = _loc21_->minZ;
				_bb[3] = _loc21_->maxX;
				_bb[4] = _loc21_->maxY;
				_bb[5] = _loc21_->maxZ;
				if (_bb[param2 + 3] <= _loc12_)
				{
					if (_bb[param2] < _loc11_)
					{
						_loc15_++;
					}
				}
				else if (_bb[param2] >= _loc11_)
				{
					_loc16_++;
				}
				else
				{
					_loc17_ = true;
					break;
				}
				_loc19_++;
			}
			double _loc20_ = _loc13_ * _loc15_ + _loc14_ * _loc16_;

			if (!_loc17_ && _loc20_ < this->splitCost && _loc15_ > 0 && _loc16_ > 0)
			{
				this->splitAxis = param2;
				this->splitCost = _loc20_;
				this->splitCoord = _loc10_;
			}
			_loc19_ = _loc9_ + 1;
			while (_loc19_ < count)
			{
				if (!filter[_loc19_] && param4->at(_loc19_) >= (_loc10_ - this->threshold) && param4->at(_loc19_) <= (_loc10_ + this->threshold))
				{
					filter[_loc19_] = true;

					param4->replace(_loc19_, NaN());
					//param4->replace(_loc19_, 1);
				}
				_loc19_++;
			}
		}
		_loc9_++;
	}
}
