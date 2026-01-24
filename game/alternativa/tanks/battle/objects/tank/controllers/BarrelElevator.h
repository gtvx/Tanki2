#ifndef BARRELELEVATOR_H
#define BARRELELEVATOR_H

#include "game/projects/tanks/client/battlefield/models/user/tank/commands/TurretControlType.h"

class BitMask;
class ValueSmoother;
class Body;
class Matrix3;

class BarrelElevator
{
	ValueSmoother *maxTurnSpeedSmoother;
	double maxTurnSpeed;
	double turnAcceleration;
	double turnSpeed;
	double defaultElevation;
	double minElevation;
	double maxElevation;
	double prevElevation;
	double currElevation;
	double interpolatedElevation;
	BitMask *locks;
	int userControl;
	int realControl;
	int elevationDirection;

public:
	BarrelElevator(double defaultElevation, double minElevation, double maxElevation, double maxTurnSpeed, double turnAcceleration);

	int getRealControl();
	double getBarrelPhysicsElevation();
	void reset();
	void setUserControl(int);
	void rotate(double, Matrix3 *);
	void interpolate(double, int);
	bool isRotating();
	double getBarrelInterpolatedElevation();
	void setBarrelElevation(double);
	void setMaxTurnSpeed(double, bool);
	double getTurnAcceleration();
	void setTurnAcceleration(double);
	void lock(int);
	void unlock(int);
	double getTurretInterpolatedDirection();
	double getTurretPhysicsDirection();
	void updatePhysics(Body*);
	void setTurretPhysicsDirection(double);
	TurretControlType getTurretRealControlType();
	double getTurretRealControlInput();
	int getTurretTurnSpeedNumber();
	void setTurretControlState(TurretControlType, double, int);
	void setGyroscopePower(double);


	enum {
		STOP = 0,
		UP = 1,
		DOWN = 2,
		CENTER = 3,
	};

private:
	void stop();
	void rotateInDirection(int param1, double param2);
	void center(double param1);
	bool isLocked();
	bool isNotLocked();
	void setRealControl(int param1);
};

#endif // BARRELELEVATOR_H
