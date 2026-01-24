#include "BoxBoxCollider.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/math/Matrix4.h"
#include "MyMath.h"
#include "ColliderUtils.h"
#include "PolygonsIntersectionUtils.h"
#include "CollidersVertex.h"
#include "FaceSide.h"


static double getOverlapOnAxis(CollisionBox *param1, CollisionBox *param2, Vector3 *param3, Vector3 *param4)
{
	const Matrix4 *_loc5_ = &param1->transform;
	double _loc6_ = (_loc5_->m00 * param3->getX() + _loc5_->m10 * param3->getY() + _loc5_->m20 * param3->getZ()) * param1->constHS()->getX();

	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}

	double _loc7_ = _loc6_;

	_loc6_ = (_loc5_->m01 * param3->getX() + _loc5_->m11 * param3->getY() + _loc5_->m21 * param3->getZ()) * param1->constHS()->getY();
	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}
	_loc7_ += _loc6_;
	_loc6_ = (_loc5_->m02 * param3->getX() + _loc5_->m12 * param3->getY() + _loc5_->m22 * param3->getZ()) * param1->constHS()->getZ();
	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}
	_loc7_ += _loc6_;
	_loc5_ = &param2->transform;
	_loc6_ = (_loc5_->m00 * param3->getX() + _loc5_->m10 * param3->getY() + _loc5_->m20 * param3->getZ()) * param2->constHS()->getX();
	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}

	_loc7_ += _loc6_;
	_loc6_ = (_loc5_->m01 * param3->getX() + _loc5_->m11 * param3->getY() + _loc5_->m21 * param3->getZ()) * param2->constHS()->getY();
	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}

	_loc7_ += _loc6_;
	_loc6_ = (_loc5_->m02 * param3->getX() + _loc5_->m12 * param3->getY() + _loc5_->m22 * param3->getZ()) * param2->constHS()->getZ();
	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}

	_loc7_ += _loc6_;
	_loc6_ = param4->getX() * param3->getX() + param4->getY() * param3->getY() + param4->getZ() * param3->getZ();
	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}
	return _loc7_ - _loc6_;
}


static void findContacts(CollisionBox *param1, CollisionBox *param2, Vector3 *param3, MyList<ShapeContact*> *param4)
{
	const Matrix4 *_loc5_ = &param1->transform;
	const Matrix4 *_loc6_ = &param2->transform;

	Vector3 _vectorToBox;

	Vector3 *_loc7_ = &_vectorToBox;
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

	CollidersVertex faceVertices1[4];
	CollidersVertex faceVertices2[4];

	ColliderUtils::buildContactBasis(param3, _loc5_, _loc6_, &_basisMatrix);
	ColliderUtils::getBoxFaceVerticesInCCWOrder(param1 ,param3, FaceSide::BACK, faceVertices1);
	ColliderUtils::getBoxFaceVerticesInCCWOrder(param2, param3, FaceSide::FRONT, faceVertices2);
	ColliderUtils::transformFaceToReferenceSpace(&_basisMatrix, &param1->transform, faceVertices1, 4);
	ColliderUtils::transformFaceToReferenceSpace(&_basisMatrix, &param2->transform, faceVertices2, 4);
	PolygonsIntersectionUtils::findContacts(param1, faceVertices1, 4, param2, faceVertices2, 4, &_basisMatrix, param4);
}


bool BoxBoxCollider::registerOverlap(double param1, Vector3 *param2)
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


bool BoxBoxCollider::testOverlapOnMainAxis(CollisionBox *box_a, CollisionBox *box_b, Vector3 *v1, Vector3 *v2)
{
	double v = getOverlapOnAxis(box_a, box_b, v1, v2);
	return this->registerOverlap(v, v1);
}


bool BoxBoxCollider::testOverlapOnDerivedAxis(CollisionBox *param1, CollisionBox *param2, Vector3 *param3, Vector3 *param4, Vector3 *param5)
{
	Vector3 axis;

	axis.setX(param3->getY() * param4->getZ() - param3->getZ() * param4->getY());
	axis.setY(param3->getZ() * param4->getX() - param3->getX() * param4->getZ());
	axis.setZ(param3->getX() * param4->getY() - param3->getY() * param4->getX());

	double _loc7_ = axis.getX() * axis.getX() + axis.getY() * axis.getY() + axis.getZ() * axis.getZ();

	if (_loc7_ < 1.0e-10)
	{
		return true;
	}

	double _loc8_ = 1 / MyMath::sqrt(_loc7_);
	axis.setX(axis.getX() * _loc8_);
	axis.setY(axis.getY() * _loc8_);
	axis.setZ(axis.getZ() * _loc8_);
	double _loc9_ = getOverlapOnAxis(param1, param2, &axis, param5);

	return registerOverlap(_loc9_, &axis);
}


BoxBoxCollider::BoxBoxCollider(double epsilon)
{
	this->epsilon = epsilon;
	this->minOverlapAxis.reset();
}


bool BoxBoxCollider::haveCollision(CollisionShape *shape_a, CollisionShape *shape_b)
{
	this->minOverlap = 10000000000;

	CollisionBox *box_a = (CollisionBox*)shape_a;
	CollisionBox *box_b = (CollisionBox*)shape_b;

	if (box_a->hide == true || box_b->hide == true)
		return false;

	const Matrix4 *matrix_a = &box_a->transform;
	const Matrix4 *matrix_b = &box_b->transform;

	Vector3 vectorToBox;

	vectorToBox.setX(matrix_a->m03 - matrix_b->m03);
	vectorToBox.setY(matrix_a->m13 - matrix_b->m13);
	vectorToBox.setZ(matrix_a->m23 - matrix_b->m23);


	Vector3 _axis10;
	Vector3 _axis11;
	Vector3 _axis12;
	Vector3 _axis20;
	Vector3 _axis21;


	_axis10.setX(matrix_a->m00);
	_axis10.setY(matrix_a->m10);
	_axis10.setZ(matrix_a->m20);
	if (!this->testOverlapOnMainAxis(box_a, box_b, &_axis10, &vectorToBox))
	{
		return false;
	}

	_axis11.setX(matrix_a->m01);
	_axis11.setY(matrix_a->m11);
	_axis11.setZ(matrix_a->m21);
	if (!this->testOverlapOnMainAxis(box_a, box_b, &_axis11, &vectorToBox))
	{
		return false;
	}

	_axis12.setX(matrix_a->m02);
	_axis12.setY(matrix_a->m12);
	_axis12.setZ(matrix_a->m22);
	if (!this->testOverlapOnMainAxis(box_a, box_b, &_axis12, &vectorToBox))
	{
		return false;
	}

	_axis20.setX(matrix_b->m00);
	_axis20.setY(matrix_b->m10);
	_axis20.setZ(matrix_b->m20);
	if (!this->testOverlapOnMainAxis(box_a, box_b, &_axis20, &vectorToBox))
	{
		return false;
	}

	_axis21.setX(matrix_b->m01);
	_axis21.setY(matrix_b->m11);
	_axis21.setZ(matrix_b->m21);
	if (!this->testOverlapOnMainAxis(box_a, box_b, &_axis21, &vectorToBox))
	{
		return false;
	}



	Vector3 _axis22;

	_axis22.setX(matrix_b->m02);
	_axis22.setY(matrix_b->m12);
	_axis22.setZ(matrix_b->m22);


	if (!this->testOverlapOnMainAxis(box_a, box_b, &_axis22, &vectorToBox))
	{
		return false;
	}

	if (!this->testOverlapOnDerivedAxis(box_a, box_b, &_axis10, &_axis20, &vectorToBox))
	{
		return false;
	}

	if (!this->testOverlapOnDerivedAxis(box_a, box_b, &_axis10, &_axis21, &vectorToBox))
	{
		return false;
	}

	if (!this->testOverlapOnDerivedAxis(box_a, box_b, &_axis10, &_axis22, &vectorToBox))
	{
		return false;
	}

	if (!this->testOverlapOnDerivedAxis(box_a, box_b, &_axis11, &_axis20, &vectorToBox))
	{
		return false;
	}

	if (!this->testOverlapOnDerivedAxis(box_a, box_b, &_axis11,&_axis21, &vectorToBox))
	{
		return false;
	}

	if (!this->testOverlapOnDerivedAxis(box_a, box_b, &_axis11, &_axis22, &vectorToBox))
	{
		return false;
	}

	if (!this->testOverlapOnDerivedAxis(box_a, box_b, &_axis12, &_axis20, &vectorToBox))
	{
		return false;
	}

	if (!this->testOverlapOnDerivedAxis(box_a, box_b, &_axis12, &_axis21, &vectorToBox))
	{
		return false;
	}

	return this->testOverlapOnDerivedAxis(box_a, box_b, &_axis12, &_axis22, &vectorToBox);
}


void BoxBoxCollider::getContacts(CollisionShape *shape_a, CollisionShape *shape_b, MyList<ShapeContact*> *contacts)
{
	if (this->haveCollision(shape_a, shape_b))
	{
		CollisionBox *box_a = (CollisionBox*)shape_a;
		CollisionBox *box_b = (CollisionBox*)shape_b;
		findContacts(box_a, box_b, &this->minOverlapAxis, contacts);
	}
}
