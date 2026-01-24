#include "FollowCameraController.h"
#include "flash/geom/Point.h"
#include "game/alternativa/math/Vector3.h"
#include "CameraPositionData.h"
#include "hardware/alternativa/engine3d/core/EllipsoidCollider.h"
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "MyMath.h"
#include "GameCamera.h"
#include "game/alternativa/math/Matrix3.h"
#include "CameraTarget.h"
#include "flash/geom/Vector3D.h"
#include "game/alternativa/tanks/services/battleinput/BattleInputServiceImpl.h"
#include "flash/geom/Vector3D.h"


extern BattleInputService *battleInputService;

static const double COLLIDER_RADIUS = 50; //50
static const double VERTICAL_CAMERA_SPEED = 0.7; //07
static const double MIN_DISTANCE = 5 * 60; //300
static const double PITCH_CORRECTION_COEFF = 1;//1
static const double FIXED_PITCH = 10 * (MyMath::PI / 180); //0.1745329251994329478
static const double MIN_CAMERA_ANGLE = 5 * (MyMath::PI / 180); //0.087266462599716473902
static const double MIN_CAMERA_ROTATE_SPEED = 3;
static const double MAX_CAMERA_ROTATE_SPEED = 9;
static double maxAngleError = (MyMath::PI / 180);
static int followCameraMode = 0;
const int FollowCameraController::CAMERA_FOLLOWS_TURRET = 0;//0
double FollowCameraController::maxPositionError = 10; //10
double FollowCameraController::camSpeedThreshold = 10; //10
const int FollowCameraController::CAMERA_FOLLOWS_MOUSE = 1;//1
static double followCameraDirection = 0;
static const double maxCameraMoveSpeed = 5;


FollowCameraController::FollowCameraController()
{
	this->mouseLookShift = 0;
	this->_mouseWheel = 0;
	this->keyUpPressed = false;
	this->keyDownPressed = false;
	this->inputLocked = false;
	this->yawSpeed = 5;
	this->pitchSpeed = 0;
	this->baseElevation = 0;
	this->collisionObject = nullptr;
	//this->collisionPoint3D = new Vector3D;
	this->locked = false;
	this->target = nullptr;
	this->linearSpeed = 0;
	this->pitchCorrectionEnabled = false;
	this->active = false;

	this->baseElevation = 0.26003128958213211908;

	this->position = new Vector3();
	this->position->reset();
	this->rotation = new Vector3();
	this->rotation->reset();
	this->targetPosition = new Vector3();
	this->targetPosition->reset();
	this->targetDirection = new Vector3();
	this->targetDirection->reset();
	this->cameraPositionData = new CameraPositionData();
	this->cameraPositionData->extraPitch = 0;
	this->cameraPositionData->t = 0;
	this->cameraPositionData->position.reset();
	this->cameraPosition = new Point(0, 0);

	this->point0 = new Point(145, 545);
	this->point1 = new Point(930, 1395);
	this->point2 = new Point(2245, 1565);
	this->point3 = new Point(3105, 760);
	this->collider = new EllipsoidCollider(COLLIDER_RADIUS, COLLIDER_RADIUS, COLLIDER_RADIUS);

	this->setCameraRelativeHeight(0.13);
}


static double bezier(double param1, double param2, double param3, double param4, double param5)
{
	double _loc6_ = 3 * (param3 - param2);
	double _loc7_ = 3 * param2 - 6 * param3 + 3 * param4;
	double _loc8_ = -param2 + 3 * param3 - 3 * param4 + param5;
	return param2 + param1 * _loc6_ + param1 * param1 * _loc7_ + param1 * param1 * param1 * _loc8_;
}


static void vector3To3D(const Vector3 *v1, Vector3D *v2)
{
	v2->setX(v1->getX());
	v2->setY(v1->getY());
	v2->setZ(v1->getZ());
}


static double getLinearSpeed(double v)
{
	return maxCameraMoveSpeed * v;
}


void FollowCameraController::setCameraRelativeHeight(double value)
{
	this->cameraRelativeHeight = MathUtils::clamp(value, 0, 1);
	double _loc2_ = MathUtils::clamp(this->cameraRelativeHeight + this->mouseLookShift * 0.1, 0, 1);
	this->cameraPosition->x = bezier(_loc2_, this->point0->x, this->point1->x, this->point2->x, this->point3->x);
	this->cameraPosition->y = bezier(_loc2_, this->point0->y, this->point1->y, this->point2->y, this->point3->y);
	this->baseElevation = MyMath::atan2(this->cameraPosition->x, this->cameraPosition->y);
	this->distanceFromPivotToCamera = this->cameraPosition->length();
}


void FollowCameraController::updateCameraHeight(double param1)
{
	//this->keyUpPressed = true;
	//this->keyDownPressed = true;

	int _loc2_ = 0;
	if (this->_mouseWheel < 0)
	{
		this->keyUpPressed = true;
		this->keyDownPressed = false;
		this->_mouseWheel++;
		if (this->_mouseWheel == 0)
		{
			this->keyUpPressed = false;
		}
	}
	else if (this->_mouseWheel > 0)
	{
		this->keyUpPressed = false;
		this->keyDownPressed = true;
		this->_mouseWheel--;
		if (this->_mouseWheel == 0)
		{
			this->keyDownPressed = false;
		}
	}
	if (!this->inputLocked && this->keyUpPressed != this->keyDownPressed)
	{
		_loc2_ = this->keyUpPressed ? 1 : -1;
		this->setCameraRelativeHeight(this->cameraRelativeHeight + ((_loc2_ * VERTICAL_CAMERA_SPEED) * param1));
	}
	else
	{
		this->setCameraRelativeHeight(this->cameraRelativeHeight);
	}
}


void FollowCameraController::setCameraPosition()
{
	this->target->getCameraParams(this->targetPosition, this->targetDirection);
	this->getCameraPositionData(this->targetPosition, this->targetDirection, this->cameraPositionData);
	this->position->copy(&this->cameraPositionData->position);
	double _loc7_ = this->getPitchAngle(this->cameraPositionData);
	double _loc8_ = MyMath::atan2(-this->targetDirection->getX(), this->targetDirection->getY());
	this->rotation->setX(_loc7_ - 1.58825); //UD
	this->rotation->setZ(_loc8_);
	this->yawSpeed = 0;
}


void FollowCameraController::update(GameCamera *param1, int, int param3)
{
	double _loc4_ = param3 * 0.001;
	if (_loc4_ > 0.1)
		_loc4_ = 0.1;

	this->updateCameraHeight(_loc4_);
	if (((!(this->locked)) && (!(this->target == nullptr))))
		this->target->getCameraParams(this->targetPosition, this->targetDirection);

	this->getCameraPositionData(this->targetPosition, this->targetDirection, this->cameraPositionData);

	Vector3 positionDelta;
	positionDelta.diff(&this->cameraPositionData->position, this->position);

	double _loc5_ = positionDelta.length();
	if (_loc5_ > maxPositionError)
	{
		this->linearSpeed = getLinearSpeed(_loc5_ - maxPositionError);
	}
	double _loc6_ = this->linearSpeed * _loc4_;
	if (_loc6_ > _loc5_)
	{
		_loc6_ = _loc5_;
	}
	positionDelta.normalize();
	positionDelta.scale(_loc6_);

	double _loc7_ = this->getPitchAngle(this->cameraPositionData);
	double _loc8_ = MyMath::atan2(-this->targetDirection->getX(), this->targetDirection->getY());
	double _loc9_ = MathUtils::clampAngle(this->rotation->getX() + (0.5 * MyMath::PI));
	double _loc10_ = MathUtils::clampAngle(this->rotation->getZ());
	double _loc11_ = MathUtils::clampAngle(_loc7_ - _loc9_);
	this->pitchSpeed = this->getAngularSpeed(_loc11_, this->pitchSpeed);
	double _loc12_ = this->pitchSpeed * _loc4_;
	if ((((_loc11_ > 0) && (_loc12_ > _loc11_)) || ((_loc11_ < 0) && (_loc12_ < _loc11_))))
		_loc12_ = _loc11_;

	double _loc13_ = MathUtils::clampAngle(_loc8_ - _loc10_);
	this->yawSpeed = this->getAngularSpeed(_loc13_, this->yawSpeed);
	double _loc14_ = this->yawSpeed * _loc4_;
	if ((((_loc13_ > 0) && (_loc14_ > _loc13_)) || ((_loc13_ < 0) && (_loc14_ < _loc13_))))
		_loc14_ = _loc13_;

	this->linearSpeed = MathUtils::snap(this->linearSpeed, 0, camSpeedThreshold); //линейный
	this->pitchSpeed = MathUtils::snap(this->pitchSpeed, 0, camSpeedThreshold); //подача
	this->yawSpeed = MathUtils::snap(this->yawSpeed, 0, camSpeedThreshold); //рыскание
	this->position->add(&positionDelta);
	this->rotation->addX(_loc12_); //UD
	this->rotation->setY(MathUtils::moveValueTowards(this->rotation->getY(), 0, _loc4_));
	this->rotation->addZ(_loc14_); //LR

	param1->setPosition(this->position);
	param1->setRotation(this->rotation);
}


void FollowCameraController::getCameraPositionData(const Vector3 *position, const Vector3 *direction, CameraPositionData *param3)
{
	Vector3 flatDirection;
	flatDirection.setZ(0);

	double _loc4_ = this->baseElevation;
	double _loc5_ = MyMath::sqrt(direction->getX() * direction->getX() + direction->getY() * direction->getY());
	if (_loc5_ < 0.00001)
	{
		flatDirection.setX(1);
		flatDirection.setY(0);
	}
	else
	{
		flatDirection.setX(direction->getX() / _loc5_);
		flatDirection.setY(direction->getY() / _loc5_);
	}
	param3->extraPitch = 0;
	param3->t = 1;

	Vector3 rayOrigin;
	rayOrigin.copy(position);

	Vector3 axis;
	axis.setX(flatDirection.getY());
	axis.setY(-flatDirection.getX());
	axis.setZ(0);

	flatDirection.reverse();


	Matrix3 rotationMatrix;

	rotationMatrix.fromAxisAngle(&axis, -_loc4_);


	Vector3 rayDirection;
	Vector3 collisionPoint;

	rotationMatrix.transformVector(&flatDirection, &rayDirection);
	this->getCollisionPoint(&rayOrigin, &rayDirection, this->distanceFromPivotToCamera, &collisionPoint);

	Vector3 _v;
	_v.copy(&rayOrigin);
	_v.subtract(&collisionPoint);
	double _loc6_ = _v.length();

	param3->t = _loc6_ / this->distanceFromPivotToCamera;

	if (_loc6_ < MIN_DISTANCE)
	{
		rayOrigin.copy(&collisionPoint);
		double _loc7_ = MIN_DISTANCE - _loc6_;
		this->getCollisionPoint(&rayOrigin, Vector3::Z_AXIS, _loc7_, &collisionPoint);
	}
	param3->position.copy(&collisionPoint);
}


void FollowCameraController::getCollisionPoint(const Vector3 *param1, const Vector3 *param2, double param3, Vector3 *param4)
{
	Vector3D rayOrigin3D;
	vector3To3D(param1, &rayOrigin3D);
	Vector3D displacement;
	Vector3D collisionPoint3D;
	Vector3D collisionNormal3D;
	displacement.setX(param3 * param2->getX());
	displacement.setY(param3 * param2->getY());
	displacement.setZ(param3 * param2->getZ());

	if (this->collider->getCollision(&rayOrigin3D, &displacement, &collisionPoint3D, &collisionNormal3D, this->collisionObject.get()))
	{
		double v = COLLIDER_RADIUS + 0.1;

		param4->setX(collisionPoint3D.getX() + v * collisionNormal3D.getX());
		param4->setY(collisionPoint3D.getY() + v * collisionNormal3D.getY());
		param4->setZ(collisionPoint3D.getZ() + v * collisionNormal3D.getZ());
	}
	else
	{
		param4->copy(param1);
		param4->addScaled(param3, param2);
	}
}


void FollowCameraController::setCollisionObject(std::shared_ptr<Object3D> o)
{
	this->collisionObject = o;
}


void FollowCameraController::deactivate()
{
	if (this->active)
	{
		this->active = false;
		battleInputService->removeGameActionListener(this);
		//battleInputService.removeMouseMoveListener(this);
		//battleInputService.removeMouseWheelListener(this);
		//settings.removeEventListener(SettingsServiceEvent.SETTINGS_CHANGED,this.configAlternateCamera);
		//storageService.getStorage().data["cameraT"] = this.cameraRelativeHeight;
		this->keyUpPressed = false;
		this->keyDownPressed = false;
	}
}


void FollowCameraController::activate(GameCamera *param1)
{
	if (!this->active)
	{
		this->active = true;
		this->configAlternateCamera();
		//settings.addEventListener(SettingsServiceEvent.SETTINGS_CHANGED, this.configAlternateCamera);
		param1->readPosition(this->position);
		param1->readRotation(this->rotation);
		battleInputService->addGameActionListener(this);
		//battleInputService.addMouseMoveListener(this);
		//battleInputService.addMouseWheelListener(this);
	}
}


void FollowCameraController::configAlternateCamera(SettingsServiceEvent*)
{
	/*
   if (param1 == nullptr || param1->getSetting() == SettingEnum.ALTERNATE_CAMERA)
   {
	  this->pitchCorrectionEnabled = !settings->alternateCamera;
   }
   */
	this->pitchCorrectionEnabled = false;
}


double FollowCameraController::getPitchAngle(CameraPositionData *_arg_1)
{
	double _local_2 = (this->baseElevation - FIXED_PITCH);
	if (_local_2 < 0)
	{
		_local_2 = 0;
	}
	double _local_3 = _arg_1->t;
	if ((((_local_3 >= 1) || (_local_2 < MIN_CAMERA_ANGLE)) || (!(this->pitchCorrectionEnabled))))
	{
		return (_arg_1->extraPitch - _local_2);
	}
	double _local_4 = this->cameraPosition->x;
	double d = ((PITCH_CORRECTION_COEFF * _local_4) * ((1 / MyMath::tan(_local_2)) - ((1 - _local_3) / MyMath::tan(this->baseElevation))));
	return (_arg_1->extraPitch - MyMath::atan2((_local_3 * _local_4),  d));

}


double FollowCameraController::getAngularSpeed(double param1, double param2)
{
	double v = (followCameraMode == CAMERA_FOLLOWS_TURRET) ? MIN_CAMERA_ROTATE_SPEED : MAX_CAMERA_ROTATE_SPEED;
	if (param1 < -maxAngleError)
	{
		return v * (param1 + maxAngleError);
	}
	if (param1 > maxAngleError)
	{
		return v * (param1 - maxAngleError);
	}
	return param2;
}


void FollowCameraController::setTarget(CameraTarget *cameraTarget)
{
	this->target = cameraTarget;
}


int FollowCameraController::getFollowCameraMode()
{
	return followCameraMode;
}


double FollowCameraController::getFollowCameraDirection()
{
	return followCameraDirection;
}


void FollowCameraController::setFollowCameraMode(int value)
{
	followCameraMode = value;
}


void FollowCameraController::setFollowCameraDirection(double value)
{
	followCameraDirection = value;
}


void FollowCameraController::setCurrentState(const Vector3 *position, const Vector3 *direction)
{
	this->targetPosition->copy(position);
	this->targetDirection->copy(direction);
	this->getCameraPositionData(position, direction, this->cameraPositionData);
	this->position->copy(&this->cameraPositionData->position);
	this->rotation->setX(this->getPitchAngle(this->cameraPositionData) - (0.5 * MyMath::PI));
	this->rotation->setY(0);
	this->rotation->setZ(MyMath::atan2(-(direction->getX()), direction->getY()));
}


void FollowCameraController::onGameAction(GameActionEnum e, bool down)
{
	switch (e)
	{
	case GameActionEnum::FOLLOW_CAMERA_UP:
		this->keyUpPressed = down;
		return;
	case GameActionEnum::FOLLOW_CAMERA_DOWN:
		this->keyDownPressed = down;
		return;
	default:
		return;
	}
}


void FollowCameraController::setLocked(bool locked)
{
	this->locked = locked;
	if (locked)
		this->_mouseWheel = 0;
}
