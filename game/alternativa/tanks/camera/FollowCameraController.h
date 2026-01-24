#ifndef FOLLOWCAMERACONTROLLER_H
#define FOLLOWCAMERACONTROLLER_H

#include "CameraController.h"
#include "game/alternativa/tanks/services/battleinput/GameActionListener.h"
#include <memory>

class Point;
class Vector3;
class CameraPositionData;
class EllipsoidCollider;
class GameCamera;
class Object3D;
class CameraTarget;
class SettingsServiceEvent;
class Vector3D;

//CameraController, IFollowCameraController, MouseWheelListener, MouseMovementListener, GameActionListener
class FollowCameraController : virtual public CameraController, public virtual GameActionListener
{
	Point *point0;
	Point *point1;
	Point *point2;
	Point *point3;
	Point *cameraPosition;
	Vector3 *position;
	Vector3 *rotation;
	Vector3 *targetPosition;
	Vector3 *targetDirection;
	CameraPositionData *cameraPositionData;
	EllipsoidCollider *collider;
	double cameraRelativeHeight;
	double baseElevation;
	double distanceFromPivotToCamera;
	double mouseLookShift;
	int _mouseWheel;
	bool keyUpPressed;
	bool keyDownPressed;
	double yawSpeed;
	double pitchSpeed;
	std::shared_ptr<Object3D> collisionObject;
	bool locked;
	CameraTarget *target;
	double linearSpeed;
	bool pitchCorrectionEnabled;
	bool active;

public:

	static const int CAMERA_FOLLOWS_TURRET;
	static const int CAMERA_FOLLOWS_MOUSE;

	static double maxPositionError;
	static double camSpeedThreshold;


	static int getFollowCameraMode();
	static double getFollowCameraDirection();

	static void setFollowCameraMode(int);
	static void setFollowCameraDirection(double);


	FollowCameraController();

	void setCollisionObject(std::shared_ptr<Object3D>);

	void deactivate() override;
	void activate(GameCamera*) override;
	void update(GameCamera*, int, int) override;

	void setTarget(CameraTarget*);

	void setCurrentState(const Vector3 *position, const Vector3 *direction);
	void onGameAction(GameActionEnum e, bool down);
	void setLocked(bool);
	void setCameraPosition();

	bool inputLocked;

private:
	void setCameraRelativeHeight(double);
	void updateCameraHeight(double);
	void getCameraPositionData(const Vector3*, const Vector3*, CameraPositionData*);
	void getCollisionPoint(const Vector3*, const Vector3*, double, Vector3*);
	double getPitchAngle(CameraPositionData*);
	void configAlternateCamera(SettingsServiceEvent* = nullptr);
	double getAngularSpeed(double, double);
};

#endif // FOLLOWCAMERACONTROLLER_H
