#include "TanksCollisionDetector.h"
#include "TankBody.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/physics/BodyContact.h"
#include "game/alternativa/physics/collision/CollisionKdTree.h"
#include "_global.h"
#include "game/alternativa/physics/collision/CollisionKdNode.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/physics/collision/types/AABB.h"
#include "game/alternativa/physics/collision/CollisionKdTree2D.h"
#include "game/alternativa/physics/collision/Collider.h"
#include "TankBody.h"
#include "game/alternativa/physics/ShapeContact.h"
#include "game/alternativa/physics/collision/BodyCollisionFilter.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/physics/collision/IRayCollisionFilter.h"
#include "_global.h"
#include "game/alternativa/physics/collision/colliders/BoxBoxCollider.h"
#include "game/alternativa/physics/collision/colliders/BoxRectCollider.h"
#include "game/alternativa/physics/collision/colliders/BoxTriangleCollider.h"
#include "game/alternativa/physics/collision/colliders/BoxSphereCollider.h"
#include "error.h"
#include "MinMax.h"
#include <QDebug>
#include "Error.h"


static const double AABB_INTERSECTION_EPSILON = 0.01;


//getBodyContacts:
//	getContactsWithStatic:
//		getShapeNodeCollisions
//  getContactsWithOtherBodies :
//		getContacts


//getShapeNodeCollisions:
//	getContacts
//	getShapeNodeCollisions


//getShapeContactsWithStatic:
//	getShapeNodeCollisions



//raycast:
//	raycastStatic:
//		getRayBoundBoxIntersection
//		testRayAgainstNode:
//			getRayNodeIntersection
//			testRayAgainstNode

//	raycastDynamic




TanksCollisionDetector::TanksCollisionDetector()
{
	this->threshold = 1.0E-4;
	this->tree = new CollisionKdTree();
	this->setCollider(CollisionShape::Type::BOX, CollisionShape::Type::BOX, new BoxBoxCollider(1.0e-6));
	this->setCollider(CollisionShape::Type::BOX, CollisionShape::Type::RECT, new BoxRectCollider(1.0e-6));
	this->setCollider(CollisionShape::Type::BOX, CollisionShape::Type::TRIANGLE, new BoxTriangleCollider(1.0e-6));
	this->setCollider(CollisionShape::Type::BOX, CollisionShape::Type::SPHERE, new BoxSphereCollider());
	this->createStaticBody();
}


void TanksCollisionDetector::setCollider(CollisionShape::Type v1, CollisionShape::Type v2, Collider *collider)
{
	this->colliders[int(v1) | int(v2)] = collider;
}


MyList<TankBody*>* TanksCollisionDetector::getTankBodies()
{
	return &this->tankBodies;
}


void TanksCollisionDetector::getBodyContacts(MyList<BodyContact *> *list)
{
	const int count = this->tankBodies.count();

	int index = 0;

	while (index < count)
	{
		TankBody *tankBody = this->tankBodies.at(index);
		tankBody->wasContactWithStatic = tankBody->hasContactsWithStatic;
		Tank *tank = tankBody->body->tank;
		tankBody->hasContactsWithStatic = (tank == nullptr) ? false : tank->hasTracksContactsWithStatic();
		tankBody->penetratedBodies.clear();
		index++;
	}

	index = 0;
	while (index < count)
	{
		TankBody *tankBody = this->tankBodies.at(index);
		const int count = list->count();
		this->getContactsWithStatic(tankBody, list);
		if (count != list->count())
			tankBody->hasContactsWithStatic = true;

		index++;
		this->getContactsWithOtherBodies(tankBody, index, list);
	}
}


void TanksCollisionDetector::getContactsWithStatic(TankBody *tankBody, MyList<BodyContact*> *list)
{
	if (!tankBody->body->frozen)
	{
		int count = tankBody->staticShapes.count();

		for (int i = 0; i < count; i++)
		{
			this->getShapeNodeCollisions(this->tree->rootNode, tankBody->staticShapes.at(i), &this->shapeContacts);
		}

		if (this->shapeContacts.count() > 0)
		{
			BodyContact *bodyContact = BodyContact::create();
			bodyContact->body1 = tankBody->body;
			bodyContact->body2 = this->staticBody;
			bodyContact->setShapeContacts(&this->shapeContacts);
			this->shapeContacts.clear();
			list->append(bodyContact);
		}
	}
}


void TanksCollisionDetector::getContactsWithOtherBodies(TankBody *tankBody, int param2, MyList<BodyContact*> *param3)
{
	int _loc4_ = this->tankBodies.count();
	int _loc5_ = param2;
	while (_loc5_ < _loc4_)
	{
		TankBody *_loc6_ = this->tankBodies.at(_loc5_);
		Body *_loc7_ = tankBody->body;
		Body *_loc8_ = _loc6_->body;
		if (!(_loc7_->frozen && _loc8_->frozen) && _loc7_->aabb.intersects(&_loc8_->aabb, AABB_INTERSECTION_EPSILON))
		{

			this->getContacts(tankBody->tankCollisionBox, _loc6_->tankCollisionBox, &this->shapeContacts);

			int _loc9_ = this->shapeContacts.count();
			if (_loc9_ > 0)
			{

				bool _loc10_ = _loc7_->postCollisionFilter == nullptr || _loc7_->postCollisionFilter->considerBodies(_loc7_, _loc8_);
				bool _loc11_ = _loc8_->postCollisionFilter == nullptr || _loc8_->postCollisionFilter->considerBodies(_loc8_, _loc7_);
				if (_loc10_ && _loc11_)
				{
					BodyContact *_loc12_ = BodyContact::create();
					_loc12_->body1 = _loc7_;
					_loc12_->body2 = _loc8_;
					_loc12_->setShapeContacts(&this->shapeContacts);
					param3->append(_loc12_);

					tankBody->penetratedBodies.append(_loc8_);
					_loc6_->penetratedBodies.append(_loc7_);
				}
				else
				{
					int _loc13_ = 0;
					while (_loc13_ < _loc9_)
					{
						ShapeContact *_loc14_ = this->shapeContacts.at(_loc13_);
						_loc14_->dispose();
						delete _loc14_;
						_loc13_++;
					}
				}
				this->shapeContacts.clear();
			}
		}
		_loc5_++;
	}
}


void TanksCollisionDetector::getShapeNodeCollisions(CollisionKdNode *node, CollisionShape *shape, MyList<ShapeContact*> *shape_contact_list)
{
	if (node->indices != nullptr)
	{
		MyList<CollisionShape*> *list = &this->tree->staticChildren;
		QVector<int> *indices = node->indices;
		int count = indices->count();

		for (int i = 0; i < count; i++)
		{
			this->getContacts(shape, list->at(indices->at(i)), shape_contact_list);
		}
	}

	if (node->axis == -1)
		return;

	double min;
	double max;

	switch (node->axis)
	{
		case 0:
			min = shape->aabb.minX;
			max = shape->aabb.maxX;
			break;
		case 1:
			min = shape->aabb.minY;
			max = shape->aabb.maxY;
			break;
		case 2:
			min = shape->aabb.minZ;
			max = shape->aabb.maxZ;
			break;
		default:
			throw Error(5);
	}

	if (min < node->coord)
	{
		this->getShapeNodeCollisions(node->negativeNode, shape, shape_contact_list);
	}

	if (max > node->coord)
	{
		this->getShapeNodeCollisions(node->positiveNode, shape, shape_contact_list);
	}

	if (node->splitTree != nullptr && min < node->coord && max > node->coord)
	{
		this->getShapeNodeCollisions(node->splitTree->rootNode, shape, shape_contact_list);
	}
}


void TanksCollisionDetector::getContacts(CollisionShape *param1, CollisionShape *param2, MyList<ShapeContact*> *param3)
{
	if ((param1->collisionGroup & param2->collisionGroup) == 0)
	{
		return;
	}

	if (param1->body == param2->body)
	{
		return;
	}


	if (!param1->aabb.intersects(&param2->aabb, AABB_INTERSECTION_EPSILON))
	{
		return;
	}


	Collider *collider = this->colliders[uchar(param1->type) | uchar(param2->type)];
	collider->getContacts(param1, param2, param3);
}


void TanksCollisionDetector::addTankBody(TankBody *tankbody)
{
	tankbody->hasContactsWithStatic = false;
	this->tankBodies.append(tankbody);
}


bool TanksCollisionDetector::raycast(const Vector3 *pos, const Vector3 *dir, int collisionGroup, double param4, IRayCollisionFilter *filter, RayHit *rayHit)
{
	bool _static = this->raycastStatic(pos, dir, collisionGroup, param4, filter, rayHit);
	bool _dynamic = this->raycastDynamic(pos, dir, collisionGroup, param4, filter, &this->_dynamicRayHit);

	if (!(_dynamic || _static))
	{
		return false;
	}

	if (_dynamic && _static)
	{
		if (rayHit->t > this->_dynamicRayHit.t)
		{
			rayHit->copy(&this->_dynamicRayHit);
		}
		this->_dynamicRayHit.clear();
		return true;
	}

	if (_static)
	{
		this->_dynamicRayHit.clear();
		return true;
	}

	rayHit->copy(&this->_dynamicRayHit);
	this->_dynamicRayHit.clear();

	return true;
}


bool TanksCollisionDetector::raycastStatic(const Vector3 *pos, const Vector3 *dir, int collisionGroup, double time, IRayCollisionFilter *filter, RayHit *rayHit)
{
	const AABB *aabb = &this->tree->rootNode->boundBox;

	MinMax _time;

	if (!this->getRayBoundBoxIntersection(pos, dir, aabb, &_time))
		return false;

	if (_time.max < 0 || _time.min > time)
		return false;

	Vector3 _o;

	if (_time.min <= 0) {
		_time.min = 0;
		_o.setX(pos->getX());
		_o.setY(pos->getY());
		_o.setZ(pos->getZ());
	} else {
		_o.setX(pos->getX() + _time.min * dir->getX());
		_o.setY(pos->getY() + _time.min * dir->getY());
		_o.setZ(pos->getZ() + _time.min * dir->getZ());
	}

	if (_time.max > time)
		_time.max = time;

	const bool b = this->testRayAgainstNode(this->tree->rootNode, pos, &_o, dir, collisionGroup, _time.min, _time.max, filter, rayHit);

	return b ? (rayHit->t <= time) : false;
}


bool TanksCollisionDetector::raycastDynamic(const Vector3 *pos, const Vector3 *dir, int collisionGroup, double time, IRayCollisionFilter *param5, RayHit *param6)
{
	double X = pos->getX() + dir->getX() * time;
	double Y = pos->getY() + dir->getY() * time;
	double Z = pos->getZ() + dir->getZ() * time;

	AABB _rayAABB;

	if (X < pos->getX())
	{
		_rayAABB.minX = X;
		_rayAABB.maxX = pos->getX();
	}
	else
	{
		_rayAABB.minX = pos->getX();
		_rayAABB.maxX = X;
	}

	if (Y < pos->getY())
	{
		_rayAABB.minY = Y;
		_rayAABB.maxY = pos->getY();
	}
	else
	{
		_rayAABB.minY = pos->getY();
		_rayAABB.maxY = Y;
	}

	if (Z < pos->getZ())
	{
		_rayAABB.minZ = Z;
		_rayAABB.maxZ = pos->getZ();
	}
	else
	{
		_rayAABB.minZ = pos->getZ();
		_rayAABB.maxZ = Z;
	}

	double _loc10_ = time + 1;
	int count = this->tankBodies.count();

	for (int i = 0; i < count; i++)
	{
		TankBody *_loc13_ = this->tankBodies.at(i);
		Body *_loc14_ = _loc13_->body;
		AABB *_loc15_ = &_loc14_->aabb;
		if (!(_rayAABB.maxX < _loc15_->minX || _rayAABB.minX > _loc15_->maxX || _rayAABB.maxY < _loc15_->minY || _rayAABB.minY > _loc15_->maxY || _rayAABB.maxZ < _loc15_->minZ || _rayAABB.minZ > _loc15_->maxZ))
		{
			for (int i = 0; i < _loc14_->numCollisionShapes; i++)
			{
				CollisionShape *_loc17_ = _loc14_->collisionShapes->at(i);
				if ((_loc17_->collisionGroup & collisionGroup) != 0)
				{
					AABB *_loc15_ = &_loc17_->aabb;
					if (!(_rayAABB.maxX < _loc15_->minX || _rayAABB.minX > _loc15_->maxX || _rayAABB.maxY < _loc15_->minY || _rayAABB.minY > _loc15_->maxY || _rayAABB.maxZ < _loc15_->minZ || _rayAABB.minZ > _loc15_->maxZ))
					{
						if (!(param5 != nullptr && !param5->considerBody(_loc14_)))
						{
							double _loc18_ = _loc17_->raycast(pos, dir, this->threshold, &this->_normal);

							if (_loc18_ >= 0 && _loc18_ < _loc10_)
							{
								_loc10_ = _loc18_;
								param6->shape = _loc17_;
								param6->normal.setX(this->_normal.getX());
								param6->normal.setY(this->_normal.getY());
								param6->normal.setZ(this->_normal.getZ());
							}
						}
					}
				}
			}
		}
	}

	if (_loc10_ > time)
	{
		return false;
	}

	param6->position.setX(pos->getX() + dir->getX() * _loc10_);
	param6->position.setY(pos->getY() + dir->getY() * _loc10_);
	param6->position.setZ(pos->getZ() + dir->getZ() * _loc10_);

	param6->t = _loc10_;

	return true;
}


bool TanksCollisionDetector::getRayBoundBoxIntersection(const Vector3 *pos, const Vector3 *dir, const AABB *aabb, MinMax *minmax)
{
	minmax->min = -1;
	minmax->max = 1.0e308;

	for (int i = 0; i < 3; i++)
	{
		double v1;
		double v2;

		switch (i)
		{
			case 0:
				if (dir->getX() < this->threshold && dir->getX() > -this->threshold)
				{
					if (pos->getX() < aabb->minX || pos->getX() > aabb->maxX)
					{
						return false;
					}
					continue;
				}
				v1 = (aabb->minX - pos->getX()) / dir->getX();
				v2 = (aabb->maxX - pos->getX()) / dir->getX();
				break;
			case 1:
				if (dir->getY() < this->threshold && dir->getY() > -this->threshold)
				{
					if (pos->getY() < aabb->minY || pos->getY() > aabb->maxY)
					{
						return false;
					}
					continue;
				}
				v1 = (aabb->minY - pos->getY()) / dir->getY();
				v2 = (aabb->maxY - pos->getY()) / dir->getY();
				break;
			case 2:
				if (dir->getZ() < this->threshold && dir->getZ() > -this->threshold)
				{
					if (pos->getZ() < aabb->minZ || pos->getZ() > aabb->maxZ)
					{
						return false;
					}
					continue;
				}
				v1 = (aabb->minZ - pos->getZ()) / dir->getZ();
				v2 = (aabb->maxZ - pos->getZ()) / dir->getZ();
				break;
			default:
				throw Error(3);
		}

		if (v1 < v2)
		{
			if (v1 > minmax->min)
			{
				minmax->min = v1;
			}
			if (v2 < minmax->max)
			{
				minmax->max = v2;
			}
		}
		else
		{
			if (v2 > minmax->min)
			{
				minmax->min = v2;
			}
			if (v1 < minmax->max)
			{
				minmax->max = v1;
			}
		}
		if (minmax->max < minmax->min)
		{
			return false;
		}
	}

	return true;
}


bool TanksCollisionDetector::testRayAgainstNode(CollisionKdNode *param1, const Vector3 *param2,
												Vector3 *param3, const Vector3 *param4, int collisionGroup,
												double param6, double param7, IRayCollisionFilter *param8, RayHit *param9)
{
	if (param1->indices != nullptr && this->getRayNodeIntersection(param2,
																   param4,
																   collisionGroup,
																   &this->tree->staticChildren,
																   param1->indices,
																   param8,
																   param9))
	{
		return true;
	}

	if (param1->axis == -1)
		return false;

	double _loc10_ = NaN();

	CollisionKdNode *_loc11_ = nullptr;

	switch (param1->axis)
	{
		case 0:
			if (param4->getX() > -this->threshold && param4->getX() < this->threshold)
			{
				_loc10_ = param7 + 1;
			}
			else
			{
				_loc10_ = (param1->coord - param2->getX()) / param4->getX();
			}
			_loc11_ = param3->getX() < param1->coord ? param1->negativeNode : param1->positiveNode;
			break;

		case 1:
			if (param4->getY() > -this->threshold && param4->getY() < this->threshold)
			{
				_loc10_ = param7 + 1;
			}
			else
			{
				_loc10_ = (param1->coord - param2->getY()) / param4->getY();
			}
			_loc11_ = param3->getY() < param1->coord ? param1->negativeNode : param1->positiveNode;
			break;

		case 2:
			if (param4->getZ() > -this->threshold && param4->getZ() < this->threshold)
			{
				_loc10_ = param7 + 1;
			}
			else
			{
				_loc10_ = (param1->coord - param2->getZ()) / param4->getZ();
			}
			_loc11_ = param3->getZ() < param1->coord ? param1->negativeNode : param1->positiveNode;
			break;

		default:
			throw Error(4);
	}

	if (_loc10_ < param6 || _loc10_ > param7)
	{
		return this->testRayAgainstNode(_loc11_,param2,param3,param4,collisionGroup,param6,param7,param8,param9);
	}

	bool _loc12_ = this->testRayAgainstNode(_loc11_,param2,param3,param4,collisionGroup,param6,_loc10_,param8,param9);
	if (_loc12_)
	{
		return true;
	}

	Vector3 _o;

	_o.setX(param2->getX() + _loc10_ * param4->getX());
	_o.setY(param2->getY() + _loc10_ * param4->getY());
	_o.setZ(param2->getZ() + _loc10_ * param4->getZ());

	if (param1->splitTree != nullptr)
	{
		CollisionKdNode *collisionKdNode = param1->splitTree->rootNode;
		while (collisionKdNode != nullptr && collisionKdNode->axis != -1)
		{
			switch(collisionKdNode->axis)
			{
				case 0:
					collisionKdNode = _o.getX() < collisionKdNode->coord ? collisionKdNode->negativeNode : collisionKdNode->positiveNode;
					continue;
				case 1:
					collisionKdNode = _o.getY() < collisionKdNode->coord ? collisionKdNode->negativeNode : collisionKdNode->positiveNode;
					continue;
				case 2:
					collisionKdNode = _o.getZ() < collisionKdNode->coord ? collisionKdNode->negativeNode : collisionKdNode->positiveNode;
					continue;
				default:
					continue;
			}
		}
		if (collisionKdNode != nullptr && collisionKdNode->indices != nullptr)
		{
			const int count = collisionKdNode->indices->count();
			int index = 0;
			while (index < count)
			{
				CollisionShape *_loc16_ = this->tree->staticChildren.at(collisionKdNode->indices->at(index));
				if ((_loc16_->collisionGroup & collisionGroup) != 0)
				{
					if (!(param8 != nullptr && !param8->considerBody(_loc16_->body)))
					{
						param9->t = _loc16_->raycast(param2, param4, this->threshold, &param9->normal);
						if (param9->t >= 0)
						{
							param9->position.copy(&_o);
							param9->shape = _loc16_;
							return true;
						}
					}
				}
				index++;
			}
		}
	}
	return this->testRayAgainstNode(_loc11_ == param1->negativeNode ? param1->positiveNode : param1->negativeNode,
									param2, &_o, param4, collisionGroup, _loc10_, param7, param8, param9);
}



bool TanksCollisionDetector::getRayNodeIntersection(const Vector3 *param1, const Vector3 *param2, int collisionGroup,
													MyList<CollisionShape*> *param4, QVector<int> *param5,
													IRayCollisionFilter *param6, RayHit *param7)
{
	int _loc8_ = param5->count();
	double _loc9_ = 1.0e308;
	int _loc10_ = 0;
	while (_loc10_ < _loc8_)
	{
		CollisionShape *_loc11_ = param4->at(param5->at(_loc10_));
		if ((_loc11_->collisionGroup & collisionGroup) != 0)
		{
			if (!(param6 != nullptr && !param6->considerBody(_loc11_->body)))
			{
				double _loc12_ = _loc11_->raycast(param1, param2, this->threshold, &this->_normal);
				if (_loc12_ > 0 && _loc12_ < _loc9_)
				{
					_loc9_ = _loc12_;
					param7->shape = _loc11_;
					param7->normal.setX(this->_normal.getX());
					param7->normal.setY(this->_normal.getY());
					param7->normal.setZ(this->_normal.getZ());
				}
			}
		}
		_loc10_++;
	}
	if (_loc9_ == 1.0e308)
		return false;

	param7->position.setX(param1->getX() + param2->getX() * _loc9_);
	param7->position.setY(param1->getY() + param2->getY() * _loc9_);
	param7->position.setZ(param1->getZ() + param2->getZ() * _loc9_);
	param7->t = _loc9_;
	return true;
}


void TanksCollisionDetector::buildKdTree(MyList<CollisionShape*> *vector)
{
	for (int i = 0; i < vector->count(); i++)
		vector->at(i)->body = this->staticBody;
	this->tree->createTree(vector);
}


void TanksCollisionDetector::getShapeContactsWithStatic(CollisionShape *param1, MyList<ShapeContact*> *param2)
{
	return this->getShapeNodeCollisions(this->tree->rootNode, param1, param2);
}


void TanksCollisionDetector::createStaticBody()
{
	Matrix3 m;
	m.setDefault();
	this->staticBody = new Body(1, &m, 10000000000);
	this->staticBody->movable = false;
}


bool TanksCollisionDetector::testStaticCollision(CollisionShape *collisionShape)
{
	return this->testShapeNodeCollision(collisionShape, this->tree->rootNode);
}


bool TanksCollisionDetector::testShapeNodeCollision(CollisionShape *collisionShape, CollisionKdNode *node)
{
	double _local_3;
	double _local_4;
	if (node->indices != nullptr)
	{
		MyList<CollisionShape*>* list = &this->tree->staticChildren;
		QVector<int>* _local_6 = node->indices;
		int _local_7 = _local_6->count();
		int _local_8 = 0;
		while (_local_8 < _local_7)
		{
			if (this->testCollision(collisionShape, list->at(_local_6->at(_local_8))))
			{
				return true;
			}
			_local_8++;
		}
	}
	if (node->axis == -1)
	{
		return false;
	}
	switch (node->axis)
	{
		case 0:
			_local_3 = collisionShape->aabb.minX;
			_local_4 = collisionShape->aabb.maxX;
			break;
		case 1:
			_local_3 = collisionShape->aabb.minY;
			_local_4 = collisionShape->aabb.maxY;
			break;
		case 2:
			_local_3 = collisionShape->aabb.minZ;
			_local_4 = collisionShape->aabb.maxZ;
			break;
		default:
			throw Error(524);
	}
	if (((!(node->splitTree == nullptr)) && (_local_3 < node->coord)) && (_local_4 > node->coord))
	{
		if (this->testShapeNodeCollision(collisionShape, node->splitTree->rootNode))
		{
			return true;
		}
	}
	if (_local_3 < node->coord)
	{
		if (this->testShapeNodeCollision(collisionShape, node->negativeNode))
		{
			return true;
		}
	}
	if (_local_4 > node->coord)
	{
		if (this->testShapeNodeCollision(collisionShape, node->positiveNode))
		{
			return true;
		}
	}
	return false;
}


bool TanksCollisionDetector::testCollision(CollisionShape *a, CollisionShape *b)
{
	if ((a->collisionGroup & b->collisionGroup) == 0)
		return false;

	if (a->body != nullptr &&  a->body == b->body)
		return false;

	if (!a->aabb.intersects(&b->aabb, AABB_INTERSECTION_EPSILON))
		return false;



	return this->colliders[(uchar(a->type) | uchar(b->type))]->haveCollision(a, b);
}


void TanksCollisionDetector::removeTankBody(TankBody *tankBody)
{
	this->tankBodies.removeAt(tankBody);
}
