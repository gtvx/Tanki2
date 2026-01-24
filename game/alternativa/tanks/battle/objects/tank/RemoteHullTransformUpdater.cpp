#include "RemoteHullTransformUpdater.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/tanks/utils/EncryptedNumberImpl.h"
#include "MyMath.h"
#include "game/alternativa/tanks/physics/TankBody.h"
#include "game/alternativa/physics/BodyState.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankSkin.h"
#include "game/alternativa/physics/Body.h"


static Vector3 position;
static Matrix3 m3;
static Vector3 smoothedEulerAngles;
static const EncryptedNumberImpl SMOOTHING_COEFF(MyMath::PI / 10.4719);



RemoteHullTransformUpdater::RemoteHullTransformUpdater(Tank *tank)
{
	this->tank = tank;
	this->smoothedOrientation.reset();
}


double RemoteHullTransformUpdater::smoothValue(double _arg_1, double _arg_2, double _arg_3, double _arg_4, double _arg_5)
{
	_arg_1 = (_arg_1 + (_arg_2 * _arg_3));
	return (_arg_1 + ((_arg_4 - _arg_1) * _arg_5));
}


void RemoteHullTransformUpdater::reset()
{
	BodyState *bodyState = &this->tank->getBody()->prevState;
	this->smoothedPosition.copy(&bodyState->position);
	this->smoothedOrientation.copy(&bodyState->orientation);
}


void RemoteHullTransformUpdater::update(double a)
{
	Body *body = this->tank->getBody();
	BodyState *bodyState = &body->prevState;
	Vector3 *_local_4 = &bodyState->velocity;
	Vector3 *_local_5 = &this->tank->interpolatedPosition;
	double _local_6 = SMOOTHING_COEFF.getNumber();
	this->smoothedPosition.setX(smoothValue(this->smoothedPosition.getX(), _local_4->getX(), a, _local_5->getX(), _local_6));
	this->smoothedPosition.setY(smoothValue(this->smoothedPosition.getY(), _local_4->getY(), a, _local_5->getY(), _local_6));
	this->smoothedPosition.setZ(smoothValue(this->smoothedPosition.getZ(), _local_4->getZ(), a, _local_5->getZ(), _local_6));
	Vector3 *_local_7 = &bodyState->angularVelocity;
	this->smoothedOrientation.addScaledVector(_local_7, a);
	this->smoothedOrientation.slerp(&this->smoothedOrientation, &this->tank->interpolatedOrientation, _local_6);
	this->smoothedOrientation.getEulerAngles(&smoothedEulerAngles);
	this->smoothedOrientation.toMatrix3(&m3);
	position.copy(&this->tank->skinCenterOffset);
	position.transform3(&m3);
	position.add(&this->smoothedPosition);
	this->tank->getSkin()->updateHullTransform(&position, &smoothedEulerAngles);
}
