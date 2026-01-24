#include "ColliderUtils.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/math/Matrix4.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include <QVector>
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "FaceSide.h"
#include "MyMath.h"
#include "CollidersVertex.h"
#include "Error.h"
#include "game/alternativa/physics/collision/primitives/CollisionRect.h"
#include "game/alternativa/physics/collision/primitives/CollisionTriangle.h"



void ColliderUtils::buildContactBasis(const Vector3 *param1, const Matrix4*, const Matrix4*, Matrix4 *param4)
{
	Vector3 basisAxisX;
	if (MyMath::abs(param1->getX()) < MyMath::abs(param1->getY()))
	{
		basisAxisX.setX(0);
		basisAxisX.setY(param1->getZ());
		basisAxisX.setZ(-param1->getY());
	}
	else
	{
		basisAxisX.setX(-param1->getZ());
		basisAxisX.setY(0);
		basisAxisX.setZ(param1->getX());
	}
	basisAxisX.normalize();

	Vector3 basisAxisY;
	basisAxisY.setX(param1->getY() * basisAxisX.getZ() - param1->getZ() * basisAxisX.getY());
	basisAxisY.setY(param1->getZ() * basisAxisX.getX() - param1->getX() * basisAxisX.getZ());
	basisAxisY.setZ(param1->getX() * basisAxisX.getY() - param1->getY() * basisAxisX.getX());
	param4->m00 = basisAxisX.getX();
	param4->m10 = basisAxisX.getY();
	param4->m20 = basisAxisX.getZ();
	param4->m01 = basisAxisY.getX();
	param4->m11 = basisAxisY.getY();
	param4->m21 = basisAxisY.getZ();
	param4->m02 = param1->getX();
	param4->m12 = param1->getY();
	param4->m22 = param1->getZ();
}


void ColliderUtils::getBoxFaceVerticesInCCWOrder(const CollisionBox *param1, const Vector3 *param2, const FaceSide param3, CollidersVertex *param4)
{
	const Matrix4 *_loc5_ = &param1->transform;
	int _loc6_ = 0;
	double _loc11_ = _loc5_->m00;
	double _loc12_ = _loc5_->m10;
	double _loc13_ = _loc5_->m20;
	double _loc7_ = _loc11_ * param2->getX() + _loc12_ * param2->getY() + _loc13_ * param2->getZ();
	double _loc8_ = MyMath::abs(_loc7_);
	_loc11_ = _loc5_->m01;
	_loc12_ = _loc5_->m11;
	_loc13_ = _loc5_->m21;
	double _loc10_ = _loc11_ * param2->getX() + _loc12_ * param2->getY() + _loc13_ * param2->getZ();
	double _loc9_ = MyMath::abs(_loc10_);
	if (_loc9_ > _loc8_)
	{
		_loc8_ = _loc9_;
		_loc7_ = _loc10_;
		_loc6_ = 1;
	}
	_loc11_ = _loc5_->m02;
	_loc12_ = _loc5_->m12;
	_loc13_ = _loc5_->m22;
	_loc10_ = _loc11_ * param2->getX() + _loc12_ * param2->getY() + _loc13_ * param2->getZ();
	_loc9_ = MyMath::abs(_loc10_);
	if (_loc9_ > _loc8_)
	{
		_loc7_ = _loc10_;
		_loc6_ = 2;
	}
	double _loc14_ = (_loc7_ > 0.) ? 1. : -1.;
	if (param3 == FaceSide::BACK)
	{
		_loc14_ = -_loc14_;
	}

	Vector3 *_loc15_ = nullptr;
	const Vector3 *hs = param1->constHS();
	switch(_loc6_)
	{
	case 0:
		_loc15_ = &param4[0].local;
		_loc15_->setX(_loc14_ * hs->getX());
		_loc15_->setY(hs->getY());
		_loc15_->setZ(hs->getZ());

		_loc15_ = &param4[1].local;
		_loc15_->setX(_loc14_ * hs->getX());
		_loc15_->setY(-hs->getY());
		_loc15_->setZ(hs->getZ());

		_loc15_ = &param4[2].local;
		_loc15_->setX(_loc14_ * hs->getX());
		_loc15_->setY(-hs->getY());
		_loc15_->setZ(-hs->getZ());

		_loc15_ = &param4[3].local;
		_loc15_->setX(_loc14_ * hs->getX());
		_loc15_->setY(hs->getY());
		_loc15_->setZ(-hs->getZ());
		break;
	case 1:
		_loc15_ = &param4[0].local;
		_loc15_->setX(hs->getX());
		_loc15_->setY(_loc14_ * hs->getY());
		_loc15_->setZ(hs->getZ());

		_loc15_ = &param4[1].local;
		_loc15_->setX(hs->getX());
		_loc15_->setY(_loc14_ * hs->getY());
		_loc15_->setZ(-hs->getZ());

		_loc15_ = &param4[2].local;
		_loc15_->setX(-hs->getX());
		_loc15_->setY(_loc14_ * hs->getY());
		_loc15_->setZ(-hs->getZ());

		_loc15_ = &param4[3].local;
		_loc15_->setX(-hs->getX());
		_loc15_->setY(_loc14_ * hs->getY());
		_loc15_->setZ(hs->getZ());
		break;
	case 2:
		_loc15_ = &param4[0].local;
		_loc15_->setX(hs->getX());
		_loc15_->setY(hs->getY());
		_loc15_->setZ(_loc14_ * hs->getZ());

		_loc15_ = &param4[1].local;
		_loc15_->setX(-hs->getX());
		_loc15_->setY(hs->getY());
		_loc15_->setZ(_loc14_ * hs->getZ());

		_loc15_ = &param4[2].local;
		_loc15_->setX(-hs->getX());
		_loc15_->setY(-hs->getY());
		_loc15_->setZ(_loc14_ * hs->getZ());

		_loc15_ = &param4[3].local;
		_loc15_->setX(hs->getX());
		_loc15_->setY(-hs->getY());
		_loc15_->setZ(_loc14_ * hs->getZ());
		break;
	default:
		throw Error(1);
	}
	if (_loc7_ < 0.)
	{
		CollidersVertex _loc17_ = param4[0];
		param4[0] = param4[3];
		param4[3] = _loc17_;
		_loc17_ = param4[1];
		param4[1] = param4[2];
		param4[2] = _loc17_;
	}
}


void ColliderUtils::transformFaceToReferenceSpace(const Matrix4 *param1, const Matrix4 *param2, CollidersVertex *param3, int count)
{
	int index = 0;
	while (index < count)
	{
		CollidersVertex *collidersVertex = &param3[index];
		param2->transformVector(&collidersVertex->local, &collidersVertex->global);
		param1->transformVectorInverse(&collidersVertex->global, &collidersVertex->transformed);
		index++;
	}
}


void ColliderUtils::getRectFaceInCCWOrder(const CollisionRect *param1, const Vector3 *param2, CollidersVertex *param3)
{
	const Vector3 *_loc4_ = &param1->hs;
	CollidersVertex *_loc5_ = &param3[0];
	_loc5_->local.setX(_loc4_->getX());
	_loc5_->local.setY(_loc4_->getY());
	_loc5_->local.setZ(0);
	_loc5_ = &param3[1];
	_loc5_->local.setX(-_loc4_->getX());
	_loc5_->local.setY(_loc4_->getY());
	_loc5_->local.setZ(0);
	_loc5_ = &param3[2];
	_loc5_->local.setX(-_loc4_->getX());
	_loc5_->local.setY(-_loc4_->getY());
	_loc5_->local.setZ(0);
	_loc5_ = &param3[3];
	_loc5_->local.setX(_loc4_->getX());
	_loc5_->local.setY(-_loc4_->getY());
	_loc5_->local.setZ(0);
	const Matrix4 *_loc6_ = &param1->transform;
	double _loc7_ = param2->getX() * _loc6_->m02 + param2->getY() * _loc6_->m12 + param2->getZ() * _loc6_->m22;
	if (_loc7_ < 0)
	{
		CollidersVertex _loc5_ = param3[0];
		param3[0] = param3[3];
		param3[3] = _loc5_;
		_loc5_ = param3[1];
		param3[1] = param3[2];
		param3[2] = _loc5_;
	}
}


void ColliderUtils::getTriangleFaceInCCWOrder(CollisionTriangle *param1, Vector3 *param2, CollidersVertex *param3)
{
	CollidersVertex *vertex = &param3[0];
	vertex->local.setX(param1->v0.getX());
	vertex->local.setY(param1->v0.getY());
	vertex->local.setZ(0);
	vertex = &param3[1];
	vertex->local.setX(param1->v1.getX());
	vertex->local.setY(param1->v1.getY());
	vertex->local.setZ(0);
	vertex = &param3[2];
	vertex->local.setX(param1->v2.getX());
	vertex->local.setY(param1->v2.getY());
	vertex->local.setZ(0);
	Matrix4 *_loc5_ = &param1->transform;
	double v = param2->getX() * _loc5_->m02 + param2->getY() * _loc5_->m12 + param2->getZ() * _loc5_->m22;
	if (v < 0)
	{
		CollidersVertex vertex = param3[0];
		param3[0] = param3[2];
		param3[2] = vertex;
	}
}
