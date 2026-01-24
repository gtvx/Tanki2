#include "CollisionKdTree2D.h"
#include "CollisionKdNode.h"
#include "game/alternativa/physics/collision/types/AABB.h"
#include "CollisionKdTree.h"
#include "_global.h"


CollisionKdTree2D::CollisionKdTree2D(CollisionKdTree *param1, CollisionKdNode *param2)
{
	rootNode = nullptr;
	this->parentTree = param1;
	this->parentNode = param2;
	minPrimitivesPerNode = 1;
	threshold = 0.1;
	splitAxis = 0;
	splitCoord = 0;
	splitCost = 0;
}


CollisionKdTree2D::~CollisionKdTree2D()
{
	if (rootNode != nullptr)
		delete rootNode;
}


void CollisionKdTree2D::createTree()
{
	this->rootNode = new CollisionKdNode();
	this->rootNode->boundBox.copyFrom(&this->parentNode->boundBox);
	this->rootNode->indices = new QVector<int>;
	int _loc1_ = this->parentNode->splitIndices->count();
	int _loc2_ = 0;
	while (_loc2_ < _loc1_)
	{
		this->rootNode->indices->append(this->parentNode->splitIndices->at(_loc2_));
		_loc2_++;
	}
	this->splitNode(this->rootNode);
}


void CollisionKdTree2D::splitNode(CollisionKdNode *param1)
{
	if (param1->indices->count() <= this->minPrimitivesPerNode)
		return;


	AABB nodeBoundBoxThreshold;

	QVector<int> *_loc2_ = param1->indices;
	const AABB *_loc5_ = &param1->boundBox;
	nodeBoundBoxThreshold.minX = _loc5_->minX + this->threshold;
	nodeBoundBoxThreshold.minY = _loc5_->minY + this->threshold;
	nodeBoundBoxThreshold.minZ = _loc5_->minZ + this->threshold;
	nodeBoundBoxThreshold.maxX = _loc5_->maxX - this->threshold;
	nodeBoundBoxThreshold.maxY = _loc5_->maxY - this->threshold;
	nodeBoundBoxThreshold.maxZ = _loc5_->maxZ - this->threshold;
	//double _loc6_ = this->threshold * 2;

	QVector<double> splitCoordsX;
	QVector<double> splitCoordsY;
	QVector<double> splitCoordsZ;

	MyList<AABB*> *_loc7_ = &this->parentTree->staticBoundBoxes;
	int _loc11_ = _loc2_->count();
	int _loc3_ = 0;
	while (_loc3_ < _loc11_)
	{
		AABB *_loc16_ = _loc7_->at(_loc2_->at(_loc3_));
		if (this->parentNode->axis != 0)
		{
			if (_loc16_->minX > nodeBoundBoxThreshold.minX)
			{
				splitCoordsX.append(_loc16_->minX);
			}
			if (_loc16_->maxX < nodeBoundBoxThreshold.maxX)
			{
				splitCoordsX.append(_loc16_->maxX);
			}
		}
		if (this->parentNode->axis != 1)
		{
			if (_loc16_->minY > nodeBoundBoxThreshold.minY)
			{
				splitCoordsY.append(_loc16_->minY);
			}
			if (_loc16_->maxY < nodeBoundBoxThreshold.maxY)
			{
				splitCoordsY.append(_loc16_->maxY);
			}
		}
		if (this->parentNode->axis != 2)
		{
			if (_loc16_->minZ > nodeBoundBoxThreshold.minZ)
			{
				splitCoordsZ.append(_loc16_->minZ);
			}
			if (_loc16_->maxZ < nodeBoundBoxThreshold.maxZ)
			{
				splitCoordsZ.append(_loc16_->maxZ);
			}
		}
		_loc3_++;
	}
	this->splitAxis = -1;
	this->splitCost = 1.0e308;

	double _nodeBB[6];
	_nodeBB[0] = _loc5_->minX;
	_nodeBB[1] = _loc5_->minY;
	_nodeBB[2] = _loc5_->minZ;
	_nodeBB[3] = _loc5_->maxX;
	_nodeBB[4] = _loc5_->maxY;
	_nodeBB[5] = _loc5_->maxZ;
	if (this->parentNode->axis != 0)
	{
		this->checkNodeAxis(param1, 0, splitCoordsX.count(), &splitCoordsX, _nodeBB);
	}
	if (this->parentNode->axis != 1)
	{
		this->checkNodeAxis(param1, 1, splitCoordsY.count() , &splitCoordsY, _nodeBB);
	}
	if (this->parentNode->axis != 2)
	{
		this->checkNodeAxis(param1, 2, splitCoordsZ.count(), &splitCoordsZ, _nodeBB);
	}
	if (this->splitAxis < 0)
	{
		return;
	}
	bool _loc12_ = this->splitAxis == 0;
	bool _loc13_ = this->splitAxis == 1;
	param1->axis = this->splitAxis;
	param1->coord = this->splitCoord;


	if (param1->negativeNode != nullptr)
		delete param1->negativeNode;

	if (param1->positiveNode != nullptr)
		delete param1->positiveNode;

	param1->negativeNode = new CollisionKdNode();
	param1->negativeNode->parent = param1;
	param1->negativeNode->boundBox.copyFrom(_loc5_);
	param1->positiveNode = new CollisionKdNode();
	param1->positiveNode->parent = param1;
	param1->positiveNode->boundBox.copyFrom(_loc5_);
	if (_loc12_)
	{
		param1->negativeNode->boundBox.maxX = param1->positiveNode->boundBox.minX = this->splitCoord;
	}
	else if (_loc13_)
	{
		param1->negativeNode->boundBox.maxY = param1->positiveNode->boundBox.minY = this->splitCoord;
	}
	else
	{
		param1->negativeNode->boundBox.maxZ = param1->positiveNode->boundBox.minZ = this->splitCoord;
	}
	double _loc14_ = this->splitCoord - this->threshold;
	double _loc15_ = this->splitCoord + this->threshold;
	_loc3_ = 0;
	while (_loc3_ < _loc11_)
	{
		AABB *_loc16_ = _loc7_->at(_loc2_->at(_loc3_));
		double _loc17_ = _loc12_ ? _loc16_->minX : _loc13_ ? _loc16_->minY : _loc16_->minZ;
		double _loc18_ = _loc12_ ? _loc16_->maxX : _loc13_ ? _loc16_->maxY : _loc16_->maxZ;
		if (_loc18_ <= _loc15_)
		{
			if (_loc17_ < _loc14_)
			{
				if (param1->negativeNode->indices == nullptr)
					param1->negativeNode->indices = new QVector<int>;

				param1->negativeNode->indices->append(_loc2_->at(_loc3_));
				_loc2_->replace(_loc3_, -1);
			}
		}
		else if (_loc17_ >= _loc14_)
		{
			if (_loc18_ > _loc15_)
			{
				if (param1->positiveNode->indices == nullptr)
					param1->positiveNode->indices = new QVector<int>;

				param1->positiveNode->indices->append(_loc2_->at(_loc3_));
				_loc2_->replace(_loc3_, -1);
			}
		}
		_loc3_++;
	}
	_loc3_ = 0;
	int _loc4_ = 0;
	while (_loc3_ < _loc11_)
	{
		if (_loc2_->at(_loc3_) >= 0)
		{
			_loc2_->replace(_loc4_++, _loc2_->at(_loc3_));
		}
		_loc3_++;
	}
	if (_loc4_ > 0)
	{
		_loc2_->resize(_loc4_);
	}
	else
	{
		if (param1->indices != nullptr) {
			delete param1->indices;
			param1->indices = nullptr;
		}
	}
	if (param1->negativeNode->indices != nullptr)
	{
		this->splitNode(param1->negativeNode);
	}
	if (param1->positiveNode->indices != nullptr)
	{
		this->splitNode(param1->positiveNode);
	}

}


void CollisionKdTree2D::checkNodeAxis(CollisionKdNode *param1, int param2, int count, QVector<double> *param4, double *param5)
{
	int _loc6_ = (param2 + 1) % 3;
	int _loc7_ = (param2 + 2) % 3;

	std::vector<bool> filter(count, false);

	double _loc8_ = (param5[_loc6_ + 3] - param5[_loc6_]) * (param5[_loc7_ + 3] - param5[_loc7_]);

	MyList<AABB*> *_loc9_ = &this->parentTree->staticBoundBoxes;

	int _loc10_ = 0;

	while (_loc10_ < count)
	{
		double _loc11_ = param4->at(_loc10_);
		if (!isNaN(_loc11_))
		{
			double _loc12_ = _loc11_ - this->threshold;
			double _loc13_ = _loc11_ + this->threshold;
			double _loc14_ = _loc8_ * (_loc11_ - param5[param2]);
			double _loc15_ = _loc8_ * (param5[int(param2 + 3)] - _loc11_);
			int _loc16_ = 0;
			int _loc17_ = 0;
			bool _loc18_ = false;
			int _loc19_ = param1->indices->count();
			int _loc20_ = 0;
			while (_loc20_ < _loc19_)
			{
				int s = param1->indices->at(_loc20_);
				if (s >= _loc9_->count()) {

				}
				AABB *_loc22_ = _loc9_->at(s);
				double _bb[6];
				_bb[0] = _loc22_->minX;
				_bb[1] = _loc22_->minY;
				_bb[2] = _loc22_->minZ;
				_bb[3] = _loc22_->maxX;
				_bb[4] = _loc22_->maxY;
				_bb[5] = _loc22_->maxZ;
				if (_bb[param2 + 3] <= _loc13_)
				{
					if (_bb[param2] < _loc12_)
					{
						_loc16_++;
					}
				}
				else if (_bb[param2] >= _loc12_)
				{
					_loc17_++;
				}
				else
				{
					_loc18_ = true;
					break;
				}
				_loc20_++;
			}
			double _loc21_ = _loc14_ * _loc16_ + _loc15_ * _loc17_;
			if (!_loc18_ && _loc21_ < this->splitCost && _loc16_ > 0 && _loc17_ > 0)
			{
				this->splitAxis = param2;
				this->splitCost = _loc21_;
				this->splitCoord = _loc11_;
			}
			_loc20_ = _loc10_ + 1;
			while (_loc20_ < count)
			{
				if (!filter[_loc20_] && param4->at(_loc20_) >= _loc11_ - this->threshold && param4->at(_loc20_) <= _loc11_ + this->threshold)
				{
					filter[_loc20_] = true;

					param4->replace(_loc20_, NaN());
					//param4->replace(_loc20_, 1);
				}
				_loc20_++;
			}
		}
		_loc10_++;
	}
}
