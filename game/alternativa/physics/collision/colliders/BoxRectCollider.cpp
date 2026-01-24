#include "BoxRectCollider.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/physics/collision/primitives/CollisionRect.h"
#include "MyMath.h"
#include "game/alternativa/math/Matrix4.h"
#include "game/alternativa/physics/ShapeContact.h"
#include "ColliderUtils.h"
#include "CollidersVertex.h"
#include "PolygonsIntersectionUtils.h"
#include "FaceSide.h"


BoxRectCollider::BoxRectCollider(double epsilon)
{
	this->epsilon = epsilon;
	this->minOverlapAxis.reset();
}


bool BoxRectCollider::haveCollision(CollisionShape *param1, CollisionShape *param2)
{
	CollisionBox *box;
	CollisionRect *rect;
	this->minOverlap = 10000000000;
	if (param1->type ==  CollisionShape::Type::BOX)
	{
		box = (CollisionBox*)param1;
		rect = (CollisionRect*)param2;
	}
	else
	{
		box = (CollisionBox*)param2;
		rect = (CollisionRect*)param1;
	}


	const Matrix4 *_loc5_ = &box->transform;
	const Matrix4 *_loc6_ = &rect->transform;
	Vector3 _vectorToBox;
	_vectorToBox.setX(_loc5_->m03 - _loc6_->m03);
	_vectorToBox.setY(_loc5_->m13 - _loc6_->m13);
	_vectorToBox.setZ(_loc5_->m23 - _loc6_->m23);

	Vector3 axis22;



	axis22.setX(_loc6_->m02);
	axis22.setY(_loc6_->m12);
	axis22.setZ(_loc6_->m22);

	if (!this->testMainAxis(box, rect, &axis22, &_vectorToBox))
	{
		return false;
	}

	Vector3 axis10;
	Vector3 axis11;
	Vector3 axis12;
	Vector3 axis20;
	Vector3 axis21;

	axis10.setX(_loc5_->m00);
	axis10.setY(_loc5_->m10);
	axis10.setZ(_loc5_->m20);

	if (!this->testMainAxis(box, rect, &axis10, &_vectorToBox))
	{
		return false;
	}

	axis11.setX(_loc5_->m01);
	axis11.setY(_loc5_->m11);
	axis11.setZ(_loc5_->m21);

	if (!this->testMainAxis(box, rect, &axis11, &_vectorToBox))
	{
		return false;
	}

	axis12.setX(_loc5_->m02);
	axis12.setY(_loc5_->m12);
	axis12.setZ(_loc5_->m22);

	if (!this->testMainAxis(box, rect, &axis12, &_vectorToBox))
	{
		return false;
	}

	axis20.setX(_loc6_->m00);
	axis20.setY(_loc6_->m10);
	axis20.setZ(_loc6_->m20);
	axis21.setX(_loc6_->m01);
	axis21.setY(_loc6_->m11);
	axis21.setZ(_loc6_->m21);

	if (!this->testDerivedAxis(box, rect, &axis10, &axis20, &_vectorToBox))
	{
		return false;
	}
	if (!this->testDerivedAxis(box, rect, &axis10, &axis21, &_vectorToBox))
	{
		return false;
	}
	if (!this->testDerivedAxis(box, rect, &axis11, &axis20, &_vectorToBox))
	{
		return false;
	}
	if (!this->testDerivedAxis(box, rect, &axis11, &axis21, &_vectorToBox))
	{
		return false;
	}
	if (!this->testDerivedAxis(box, rect, &axis12, &axis20, &_vectorToBox))
	{
		return false;
	}
	if (!this->testDerivedAxis(box, rect, &axis12, &axis21, &_vectorToBox))
	{
		return false;
	}

	return true;
}


static double getOverlapOnAxis(CollisionBox *param1, CollisionRect *param2, Vector3 *param3, Vector3 *param4)
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
	_loc7_ = _loc7_ + _loc6_;
	_loc6_ = (_loc5_->m02 * param3->getX() + _loc5_->m12 * param3->getY() + _loc5_->m22 * param3->getZ()) * param1->constHS()->getZ();
	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}
	_loc7_ = _loc7_ + _loc6_;
	_loc5_ = &param2->transform;
	_loc6_ = (_loc5_->m00 * param3->getX() + _loc5_->m10 * param3->getY() + _loc5_->m20 * param3->getZ()) * param2->hs.getX();
	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}
	_loc7_ = _loc7_ + _loc6_;
	_loc6_ = (_loc5_->m01 * param3->getX() + _loc5_->m11 * param3->getY() + _loc5_->m21 * param3->getZ()) * param2->hs.getY();
	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}
	_loc7_ = _loc7_ + _loc6_;
	_loc6_ = param4->getX() * param3->getX() + param4->getY() * param3->getY() + param4->getZ() * param3->getZ();
	if (_loc6_ < 0)
	{
		_loc6_ = -_loc6_;
	}
	return _loc7_ - _loc6_;
}




static void findContacts(CollisionBox *param1, CollisionRect *param2, Vector3 *param3, MyList<ShapeContact*> *param4)
{
	const Matrix4 *_loc5_ = &param1->transform;
	const Matrix4 *_loc6_ = &param2->transform;
	Vector3 _vectorToBox;
	_vectorToBox.setX(_loc5_->m03 - _loc6_->m03);
	_vectorToBox.setY(_loc5_->m13 - _loc6_->m13);
	_vectorToBox.setZ(_loc5_->m23 - _loc6_->m23);
	if ((param3->getX() * _vectorToBox.getX()) + (param3->getY() * _vectorToBox.getY()) + (param3->getZ() * _vectorToBox.getZ()) < 0.)
	{
		param3->setX(-param3->getX());
		param3->setY(-param3->getY());
		param3->setZ(-param3->getZ());
	}

	CollidersVertex rectFaceVertices[4];
	CollidersVertex boxFaceVertices[4];
	Matrix4 _basisMatrix;
	_basisMatrix.m03 = 0;
	_basisMatrix.m13 = 0;
	_basisMatrix.m23 = 0;


	///////////////////////////////////in//////in//////in/////out///
	ColliderUtils::buildContactBasis(param3, _loc5_, _loc6_, &_basisMatrix);

	ColliderUtils::getBoxFaceVerticesInCCWOrder(param1, param3, FaceSide::BACK, boxFaceVertices);
	ColliderUtils::getRectFaceInCCWOrder(param2, param3, rectFaceVertices);

	ColliderUtils::transformFaceToReferenceSpace(&_basisMatrix, _loc5_, boxFaceVertices, 4);
	ColliderUtils::transformFaceToReferenceSpace(&_basisMatrix, _loc6_, rectFaceVertices, 4);
	int _loc9_ = param4->count();



	PolygonsIntersectionUtils::findContacts(param1, boxFaceVertices, 4, param2, rectFaceVertices, 4, &_basisMatrix, param4);

	int _loc10_ = param4->count();
	int _loc11_ = _loc9_;
	while (_loc11_ < _loc10_)
	{
		ShapeContact *shapeContact = param4->at(_loc11_);
		Vector3 *normal = &shapeContact->normal;
		double X = _loc6_->m02;
		double Y = _loc6_->m12;
		double Z = _loc6_->m22;
		if (((normal->getX() * X) + (normal->getY() * Y) + (normal->getZ() * Z)) < 0)
		{
			shapeContact->dispose();
			_loc10_--;
			param4->set(_loc11_, param4->at(_loc10_));
			param4->set(_loc10_, nullptr);
			_loc11_--;
		}
		else if (MyMath::abs(_loc6_->m22) > 0.999)
		{
			normal->setX(X);
			normal->setY(Y);
			normal->setZ(Z);
		}
		_loc11_++;
	}


	if (_loc10_ < param4->count())
	{
		param4->resize(_loc10_);
	}
}


void BoxRectCollider::getContacts(CollisionShape *param1, CollisionShape *param2, MyList<ShapeContact *> *param3)
{
	if (this->haveCollision(param1, param2))
	{
		CollisionRect *rect;
		CollisionBox *box;

		if (param1->type == CollisionShape::Type::RECT)
		{
			rect = (CollisionRect*)param1;
			box = (CollisionBox*)param2;
		}
		else
		{
			rect = (CollisionRect*)param2;
			box = (CollisionBox*)param1;
		}

		findContacts(box, rect, &this->minOverlapAxis, param3);
	}
}


bool BoxRectCollider::testDerivedAxis(CollisionBox *param1, CollisionRect *param2, Vector3 *param3, Vector3 *param4, Vector3 *param5)
{
	Vector3 _axis;
	_axis.setX(param3->getY() * param4->getZ() - param3->getZ() * param4->getY());
	_axis.setY(param3->getZ() * param4->getX() - param3->getX() * param4->getZ());
	_axis.setZ(param3->getX() * param4->getY() - param3->getY() * param4->getX());
	double _loc6_ = _axis.getX() * _axis.getX() + _axis.getY() * _axis.getY() + _axis.getZ() * _axis.getZ();
	if (_loc6_ < 1.0e-10)
	{
		return true;
	}
	double _loc7_ = 1. / MyMath::sqrt(_loc6_);
	_axis.setX(_axis.getX() * _loc7_);
	_axis.setY(_axis.getY() * _loc7_);
	_axis.setZ(_axis.getZ() * _loc7_);
	double _loc8_ = getOverlapOnAxis(param1, param2, &_axis, param5);
	return this->registerOverlap(_loc8_, &_axis);
}


bool BoxRectCollider::testMainAxis(CollisionBox *box, CollisionRect *rect, Vector3 *param3, Vector3 *param4)
{
	double v = getOverlapOnAxis(box, rect, param3, param4);
	return this->registerOverlap(v, param3);
}


bool BoxRectCollider::registerOverlap(double param1, Vector3 *param2)
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

