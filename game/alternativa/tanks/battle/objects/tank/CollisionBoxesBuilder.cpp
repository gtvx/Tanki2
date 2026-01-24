#include "CollisionBoxesBuilder.h"
#include "game/alternativa/physics/PhysicsMaterial.h"
#include "MyMath.h"
#include "game/alternativa/math/Matrix4.h"
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/tanks/physics/TankBody.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/physics/CollisionGroup.h"
#include <QDebug>


static const PhysicsMaterial NORMAL_FRICTION_MATERIAL(0, 1);
static const PhysicsMaterial LOW_FRICTION_MATERIAL(0, 0.2);


void CollisionBoxesBuilder::createStaticCollisionBoxes(Vector3 *_arg_1, double _arg_2, TankBody *tankBody)
{
	double _local_4 = (_arg_2 - 10);
	double _local_5 = _arg_1->getY();
	double _local_6 = (_local_4 / 2);
	double _local_7 = 0.82;
	double _local_8 = (1 - (((((1 - _local_7) * (1 - _local_7)) * _local_5) * _local_5) / (_local_6 * _local_6)));
	if (_local_8 > 0) {
		_local_8 = MyMath::sqrt(_local_8);
	} else {
		_local_8 = (1 - (((1 - _local_7) * _local_5) / _local_6));
	}

	double _local_9 = (((_local_7 - 1) * _local_5) / ((1 + _local_8) * _local_6));
	double _local_10 = ((_local_8 * _local_6) - (_local_9 * _local_5));
	double _local_11 = (((1 - _local_7) * _local_5) / ((1 + _local_8) * _local_6));
	double _local_12 = (((1 - _local_8) * _local_6) / ((1 - _local_7) * _local_5));

	double _local_13;
	double _local_14;

	if (((MathUtils::numbersEqual(_local_11, _local_12, 1E-5)) || (_local_11 < _local_12))) {
		_local_13 = (_local_7 * _local_5);
		_local_14 = ((_local_9 * _local_13) + _local_10);
	} else {
		_local_13 = ((((1 - _local_8) * _local_6) / _local_9) + _local_5);
		_local_14 = _local_6;
	}

	double _local_15 = (_local_14 - (_local_8 * _local_6));
	double _local_16 = (_local_5 - _local_13);
	double _local_17 = (MyMath::sqrt(((_local_15 * _local_15) + (_local_16 * _local_16))) / 2);
	_local_15 = ((1 + _local_8) * _local_6);
	_local_16 = ((1 - _local_7) * _local_5);
	double _local_18 = (MyMath::sqrt(((_local_15 * _local_15) + (_local_16 * _local_16))) / 2);

	const double _local_21 = MyMath::atan(_local_11);

	double _local_22 = (((_local_7 * _local_5) + (_local_18 * MyMath::sin(_local_21))) - (_local_17 * MyMath::cos(_local_21)));
	double _local_23 = (((-(_local_6) + (_local_18 * MyMath::cos(_local_21))) + (_local_17 * MyMath::sin(_local_21))) - (_arg_1->getZ() - (_local_4 / 2)));



	{
		//front box
		const Vector3 hs(_arg_1->getX(), _local_17, _local_18 + 0); //200
		CollisionBox *box = new CollisionBox(&hs, CollisionGroup::STATIC, &LOW_FRICTION_MATERIAL);
		Matrix4 matrix;
		matrix.setRotationMatrix((-(_local_21)) - .0, 0, 0);
		const Vector3 position(0, _local_22 + 0 , _local_23);//1300
		matrix.setPosition(&position);
		tankBody->body->addCollisionShape(box, &matrix);
		tankBody->staticShapes.append(box);
	}


	{
		//rear box
		const Vector3 hs(_arg_1->getX(), _local_17, _local_18);
		CollisionBox *box = new CollisionBox(&hs, CollisionGroup::STATIC, &LOW_FRICTION_MATERIAL);
		Matrix4 matrix;
		matrix.setRotationMatrix(_local_21, 0, 0);
		const Vector3 position(0, -(_local_22), _local_23);
		matrix.setPosition(&position);
		tankBody->body->addCollisionShape(box, &matrix);
		tankBody->staticShapes.append(box);
	}


	{
		//lower box
		double _local_24 = ((_arg_2 * 3) / 4);
		const Vector3 hs(_arg_1->getX(), (_arg_1->getY() * _local_7), (_local_24 / 2));
		Matrix4 matrix;
		matrix.null();
		matrix.m23 = (hs.getZ() - _arg_1->getZ());
		CollisionBox *box = new CollisionBox(&hs, CollisionGroup::STATIC, &LOW_FRICTION_MATERIAL);
		tankBody->body->addCollisionShape(box, &matrix);
		tankBody->staticShapes.append(box);
	}


	{
		//upper box
		double _local_28 = ((_arg_2 * 3) / 4);
		const Vector3 hs(_arg_1->getX(), (_arg_1->getY() * _local_7), (_local_28 / 2));
		Matrix4 matrix;
		matrix.null();
		matrix.m23 = ((_arg_2 - hs.getZ()) - _arg_1->getZ());
		CollisionBox *box = new CollisionBox(&hs, CollisionGroup::STATIC, &NORMAL_FRICTION_MATERIAL);
		tankBody->body->addCollisionShape(box, &matrix);
		tankBody->staticShapes.append(box);
	}
}


void CollisionBoxesBuilder::createTankCollisionBox(Vector3 *_arg_1, double _arg_2, TankBody *tankBody)
{
	//box
	const Vector3 hs(_arg_1->getX(), _arg_1->getY(), (_arg_2 / 2));
	CollisionBox *box = new CollisionBox(&hs, 0, &LOW_FRICTION_MATERIAL);
	Matrix4 matrix;
	matrix.null();
	matrix.m23 = ((_arg_2 / 2) - _arg_1->getZ());
	tankBody->body->addCollisionShape(box, &matrix);
	tankBody->tankCollisionBox = box;
}
