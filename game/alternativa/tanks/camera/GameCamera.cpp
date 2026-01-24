#include "GameCamera.h"
#include "game/alternativa/math/Vector3.h"
#include "MyMath.h"


std::shared_ptr<GameCamera> new_GameCamera()
{
	return std::make_shared<GameCamera>();
}


GameCamera::GameCamera()
{
	position.reset();
	xAxis.reset();
	yAxis.reset();
	zAxis.reset();
}


void GameCamera::setPosition(const Vector3 *v)
{
	x = v->getX();
	y = v->getY();
	z = v->getZ();
}


void GameCamera::setRotation(const Vector3 *v)
{
	rotationX = v->getX();
	rotationY = v->getY();
	rotationZ = v->getZ();
}


void GameCamera::readPosition(Vector3 *v) const
{
	v->setX(x);
	v->setY(y);
	v->setZ(z);
}


void GameCamera::readRotation(Vector3 *v) const
{
	v->setX(rotationX);
	v->setY(rotationY);
	v->setZ(rotationZ);
}


void GameCamera::calculateAdditionalData()
{
	double _local_1 = MyMath::cos(rotationX);
	double _local_2 = MyMath::sin(rotationX);
	double _local_3 = MyMath::cos(rotationY);
	double _local_4 = MyMath::sin(rotationY);
	double _local_5 = MyMath::cos(rotationZ);
	double _local_6 = MyMath::sin(rotationZ);
	double _local_7 = (_local_5 * _local_4);
	double _local_8 = (_local_6 * _local_4);

	this->xAxis.x = (_local_5 * _local_3);
	this->yAxis.x = ((_local_7 * _local_2) - (_local_6 * _local_1));
	this->zAxis.x = ((_local_7 * _local_1) + (_local_6 * _local_2));

	this->xAxis.y = (_local_6 * _local_3);
	this->yAxis.y = ((_local_8 * _local_2) + (_local_5 * _local_1));
	this->zAxis.y = ((_local_8 * _local_1) - (_local_5 * _local_2));

	this->xAxis.z = -(_local_4);
	this->yAxis.z = (_local_3 * _local_2);
	this->zAxis.z = (_local_3 * _local_1);

	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}
