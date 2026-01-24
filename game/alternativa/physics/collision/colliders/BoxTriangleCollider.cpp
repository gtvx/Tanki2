#include "BoxTriangleCollider.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/physics/collision/primitives/CollisionTriangle.h"
#include "game/alternativa/math/Matrix4.h"
#include "MyMath.h"
#include "FaceSide.h"
#include "ColliderUtils.h"
#include "PolygonsIntersectionUtils.h"
#include "game/alternativa/physics/ShapeContact.h"
#include "CollidersVertex.h"


BoxTriangleCollider::BoxTriangleCollider(double epsilon)
{
	this->epsilon = epsilon;
	this->minOverlapAxis.reset();
}


bool BoxTriangleCollider::haveCollision(CollisionShape *shape1, CollisionShape *shape2)
{
	CollisionTriangle *triangle;
	CollisionBox *box;

	if (shape1->type == CollisionShape::Type::BOX) {
		box = (CollisionBox*)shape1;
		triangle = (CollisionTriangle*)shape2;
	} else {
		box = (CollisionBox*)shape2;
		triangle = (CollisionTriangle*)shape1;
	}

	const Matrix4 *box_matrix = &box->transform;
	const Matrix4 *triangle_matrix = &triangle->transform;

	Vector3 toBox;
	toBox.setX(box_matrix->m03 - triangle_matrix->m03);
	toBox.setY(box_matrix->m13 - triangle_matrix->m13);
	toBox.setZ(box_matrix->m23 - triangle_matrix->m23);
	this->minOverlap = 10000000000;


	Vector3 axis;
	Vector3 axis10;
	Vector3 axis11;
	Vector3 axis12;
	Vector3 axis20;
	Vector3 axis21;

	axis.setX(triangle_matrix->m02);
	axis.setY(triangle_matrix->m12);
	axis.setZ(triangle_matrix->m22);
	if (!this->testOverlapOnMainAxis(box, triangle, &axis, &toBox))
		return false;

	axis10.setX(box_matrix->m00);
	axis10.setY(box_matrix->m10);
	axis10.setZ(box_matrix->m20);
	if (!this->testOverlapOnMainAxis(box, triangle, &axis10, &toBox))
		return false;

	axis11.setX(box_matrix->m01);
	axis11.setY(box_matrix->m11);
	axis11.setZ(box_matrix->m21);
	if (!this->testOverlapOnMainAxis(box, triangle, &axis11, &toBox))
		return false;

	axis12.setX(box_matrix->m02);
	axis12.setY(box_matrix->m12);
	axis12.setZ(box_matrix->m22);
	if (!this->testOverlapOnMainAxis(box, triangle, &axis12, &toBox))
		return false;

	Vector3 *_loc7_ = &triangle->e0;
	axis20.setX(triangle_matrix->m00 * _loc7_->getX() + triangle_matrix->m01 * _loc7_->getY() + triangle_matrix->m02 * _loc7_->getZ());
	axis20.setY(triangle_matrix->m10 * _loc7_->getX() + triangle_matrix->m11 * _loc7_->getY() + triangle_matrix->m12 * _loc7_->getZ());
	axis20.setZ(triangle_matrix->m20 * _loc7_->getX() + triangle_matrix->m21 * _loc7_->getY() + triangle_matrix->m22 * _loc7_->getZ());

	if (!this->testOverlapOnDerivedAxis(box, triangle, &axis10, &axis20, &toBox))
		return false;

	if (!this->testOverlapOnDerivedAxis(box,triangle, &axis11, &axis20, &toBox))
		return false;

	if (!this->testOverlapOnDerivedAxis(box,triangle, &axis12, &axis20, &toBox))
		return false;

	_loc7_ = &triangle->e1;

	axis21.setX(triangle_matrix->m00 * _loc7_->getX() + triangle_matrix->m01 * _loc7_->getY() + triangle_matrix->m02 * _loc7_->getZ());
	axis21.setY(triangle_matrix->m10 * _loc7_->getX() + triangle_matrix->m11 * _loc7_->getY() + triangle_matrix->m12 * _loc7_->getZ());
	axis21.setZ(triangle_matrix->m20 * _loc7_->getX() + triangle_matrix->m21 * _loc7_->getY() + triangle_matrix->m22 * _loc7_->getZ());

	if (!this->testOverlapOnDerivedAxis(box,triangle, &axis10, &axis21,&toBox))
		return false;

	if (!this->testOverlapOnDerivedAxis(box, triangle, &axis11, &axis21, &toBox))
		return false;

	if (!this->testOverlapOnDerivedAxis(box, triangle, &axis12, &axis21, &toBox))
		return false;

	_loc7_ = &triangle->e2;

	Vector3 axis22;
	axis22.setX(triangle_matrix->m00 * _loc7_->getX() + triangle_matrix->m01 * _loc7_->getY() + triangle_matrix->m02 * _loc7_->getZ());
	axis22.setY(triangle_matrix->m10 * _loc7_->getX() + triangle_matrix->m11 * _loc7_->getY() + triangle_matrix->m12 * _loc7_->getZ());
	axis22.setZ(triangle_matrix->m20 * _loc7_->getX() + triangle_matrix->m21 * _loc7_->getY() + triangle_matrix->m22 * _loc7_->getZ());

	if (!this->testOverlapOnDerivedAxis(box,triangle, &axis10, &axis22, &toBox))
		return false;

	if (!this->testOverlapOnDerivedAxis(box, triangle, &axis11, &axis22, &toBox))
		return false;

	return this->testOverlapOnDerivedAxis(box, triangle, &axis12, &axis22, &toBox);
}


static double getOverlapOnAxis(CollisionBox *param1, CollisionTriangle *param2, Vector3 *param3, Vector3 *param4)
{
	const Matrix4 *_loc5_ = &param1->transform;
	const Vector3 *_loc6_ = param1->constHS();
	double _loc7_ = 0;
	double _loc8_ = (_loc5_->m00 * param3->getX() + _loc5_->m10 * param3->getY() + _loc5_->m20 * param3->getZ()) * _loc6_->getX();
	if (_loc8_ < 0)
	{
		_loc7_ = _loc7_ - _loc8_;
	}
	else
	{
		_loc7_ = _loc7_ + _loc8_;
	}
	_loc8_ = (_loc5_->m01 * param3->getX() + _loc5_->m11 * param3->getY() + _loc5_->m21 * param3->getZ()) * _loc6_->getY();
	if (_loc8_ < 0)
	{
		_loc7_ = _loc7_ - _loc8_;
	}
	else
	{
		_loc7_ = _loc7_ + _loc8_;
	}
	_loc8_ = (_loc5_->m02 * param3->getX() + _loc5_->m12 * param3->getY() + _loc5_->m22 * param3->getZ()) * _loc6_->getZ();
	if (_loc8_ < 0)
	{
		_loc7_ = _loc7_ - _loc8_;
	}
	else
	{
		_loc7_ = _loc7_ + _loc8_;
	}
	double _loc9_ = param4->getX() * param3->getX() + param4->getY() * param3->getY() + param4->getZ() * param3->getZ();
	const Matrix4 *_loc10_ = &param2->transform;
	double _loc11_ = _loc10_->m00 * param3->getX() + _loc10_->m10 * param3->getY() + _loc10_->m20 * param3->getZ();
	double _loc12_ = _loc10_->m01 * param3->getX() + _loc10_->m11 * param3->getY() + _loc10_->m21 * param3->getZ();
	double _loc13_ = _loc10_->m02 * param3->getX() + _loc10_->m12 * param3->getY() + _loc10_->m22 * param3->getZ();
	double _loc14_ = 0;
	Vector3 *_loc15_ = &param2->v0;
	Vector3 *_loc16_ = &param2->v1;
	Vector3 *_loc17_ = &param2->v2;
	if (_loc9_ < 0)
	{
		_loc9_ = -_loc9_;
		_loc8_ = _loc15_->getX() * _loc11_ + _loc15_->getY() * _loc12_ + _loc15_->getZ() * _loc13_;
		if (_loc8_ < _loc14_)
		{
			_loc14_ = _loc8_;
		}
		_loc8_ = _loc16_->getX() * _loc11_ + _loc16_->getY() * _loc12_ + _loc16_->getZ() * _loc13_;
		if (_loc8_ < _loc14_)
		{
			_loc14_ = _loc8_;
		}
		_loc8_ = _loc17_->getX() * _loc11_ + _loc17_->getY() * _loc12_ + _loc17_->getZ() * _loc13_;
		if (_loc8_ < _loc14_)
		{
			_loc14_ = _loc8_;
		}
		_loc14_ = -_loc14_;
	}
	else
	{
		_loc8_ = _loc15_->getX() * _loc11_ + _loc15_->getY() * _loc12_ + _loc15_->getZ() * _loc13_;
		if (_loc8_ > _loc14_)
		{
			_loc14_ = _loc8_;
		}
		_loc8_ = _loc16_->getX() * _loc11_ + _loc16_->getY() * _loc12_ + _loc16_->getZ() * _loc13_;
		if (_loc8_ > _loc14_)
		{
			_loc14_ = _loc8_;
		}
		_loc8_ = _loc17_->getX() * _loc11_ + _loc17_->getY() * _loc12_ + _loc17_->getZ() * _loc13_;
		if (_loc8_ > _loc14_)
		{
			_loc14_ = _loc8_;
		}
	}
	return _loc7_ + _loc14_ - _loc9_;
}


void BoxTriangleCollider::getContacts(CollisionShape *shape1, CollisionShape *shape2, MyList<ShapeContact *> *list)
{
	if (!this->haveCollision(shape1, shape2))
		return;

	CollisionTriangle *triangle;
	CollisionBox *box;

	if (shape1->type == CollisionShape::Type::BOX)
	{
		box = (CollisionBox*)shape1;
		triangle = (CollisionTriangle*)shape2;
	}
	else
	{
		box = (CollisionBox*)shape2;
		triangle = (CollisionTriangle*)shape1;
	}
	this->findContacts(box, triangle, &this->minOverlapAxis, list);
}


bool BoxTriangleCollider::testOverlapOnMainAxis(CollisionBox *box, CollisionTriangle *triangle, Vector3 *param3, Vector3 *param4)
{
	double v = getOverlapOnAxis(box, triangle, param3, param4);
	return this->registerOverlap(v, param3);
}


bool BoxTriangleCollider::testOverlapOnDerivedAxis(CollisionBox *box, CollisionTriangle *triangle, Vector3 *param3, Vector3 *param4, Vector3 *param5)
{
	Vector3 axis;
	axis.setX(param3->getY() * param4->getZ() - param3->getZ() * param4->getY());
	axis.setY(param3->getZ() * param4->getX() - param3->getX() * param4->getZ());
	axis.setZ(param3->getX() * param4->getY() - param3->getY() * param4->getX());
	double _loc6_ = (axis.getX() * axis.getX()) + (axis.getY() * axis.getY()) + (axis.getZ() * axis.getZ());
	if (_loc6_ < 1.0e-10)
		return true;

	double _loc7_ = 1 / MyMath::sqrt(_loc6_);
	axis.setX(axis.getX() * _loc7_);
	axis.setY(axis.getY() * _loc7_);
	axis.setZ(axis.getZ() * _loc7_);
	double _loc8_ = getOverlapOnAxis(box, triangle, &axis, param5);
	return this->registerOverlap(_loc8_, &axis);
}


bool BoxTriangleCollider::registerOverlap(double param1, Vector3 *param2)
{
	if (param1 < this->epsilon)
	{
		return false;
	}

	if (param1 + this->epsilon < this->minOverlap)
	{
		this->minOverlap = param1;
		this->minOverlapAxis.setX(param2->getX());
		this->minOverlapAxis.setY(param2->getY());
		this->minOverlapAxis.setZ(param2->getZ());
	}

	return true;
}


void BoxTriangleCollider::findContacts(CollisionBox *param1, CollisionTriangle *param2, Vector3 *param3, MyList<ShapeContact*> *param4)
{
	const Matrix4 *_loc5_ = &param1->transform;
	const Matrix4 *_loc6_ = &param2->transform;


	Vector3 toBox;
	Vector3 *_loc7_ = &toBox;
	_loc7_->setX(_loc5_->m03 - _loc6_->m03);
	_loc7_->setY(_loc5_->m13 - _loc6_->m13);
	_loc7_->setZ(_loc5_->m23 - _loc6_->m23);
	if (param3->getX() * _loc7_->getX() + param3->getY() * _loc7_->getY() + param3->getZ() * _loc7_->getZ() < 0)
	{
		param3->setX(-param3->getX());
		param3->setY(-param3->getY());
		param3->setZ(-param3->getZ());
	}

	Matrix4 _basisMatrix;
	_basisMatrix.m03 = 0;
	_basisMatrix.m13 = 0;
	_basisMatrix.m23 = 0;

	Matrix4 *_loc8_ = &_basisMatrix;
	ColliderUtils::buildContactBasis(param3, _loc5_, _loc6_, _loc8_);

	CollidersVertex boxFaceVertices[4];
	CollidersVertex triFaceVertices[3];

	ColliderUtils::getBoxFaceVerticesInCCWOrder(param1, param3, FaceSide::BACK, boxFaceVertices);
	ColliderUtils::getTriangleFaceInCCWOrder(param2, param3, triFaceVertices);
	ColliderUtils::transformFaceToReferenceSpace(_loc8_, _loc5_, boxFaceVertices, 4);
	ColliderUtils::transformFaceToReferenceSpace(_loc8_, _loc6_, triFaceVertices, 3);
	int _loc9_ = param4->count();
	PolygonsIntersectionUtils::findContacts(param1, boxFaceVertices, 4, param2, triFaceVertices, 3, _loc8_, param4);
	int _loc10_ = param4->count();
	int _loc11_ = _loc9_;
	while (_loc11_ < _loc10_)
	{
		ShapeContact *_loc12_ = param4->at(_loc11_);
		Vector3 *_loc13_ = &_loc12_->normal;
		double _loc14_ = _loc6_->m02;
		double _loc15_ = _loc6_->m12;
		double _loc16_ = _loc6_->m22;
		if (_loc13_->getX() * _loc14_ + _loc13_->getY() * _loc15_ + _loc13_->getZ() * _loc16_ < 0)
		{
			_loc12_->dispose();
			_loc10_--;
			param4->set(_loc11_, param4->at(_loc10_));
			param4->set(_loc10_, nullptr);
			_loc11_--;
		}
		_loc11_++;
	}
	if (_loc10_ < param4->count())
	{
		param4->resize(_loc10_);
	}
}
