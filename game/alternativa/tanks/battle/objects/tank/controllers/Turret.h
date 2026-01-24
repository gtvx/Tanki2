#ifndef TURRET_H
#define TURRET_H

#include "game/alternativa/tanks/utils/BitMask.h"
#include "game/projects/tanks/client/battlefield/models/user/tank/commands/TurretControlType.h"
#include "game/alternativa/tanks/battle/objects/tank/WeaponMount.h"
#include "game/alternativa/tanks/battle/utils/DampedSpring.h"

class ValueSmoother;

class Turret : public WeaponMount
{
	double maxTurnSpeed;
	double turnAcceleration;
	BitMask lockMask;
	TurretControlType realControlType;
	TurretControlType controlType;
	ValueSmoother *maxTurnSpeedSmoother;
	double currDirection;
	double prevDirection;
	double turnSpeed;
	double controlInput;
	double interpolatedDirection;
	double turnSpeedNumber;
	DampedSpring spring;
	int prevTurnDirection;
	double realControlInput;
	double barrelElevation;
	double gyroscopePower;

public:

	static const int TURN_SPEED_COUNT;


	Turret(double maxTurnSpeed, double turnAcceleration);

	void setTurretPhysicsDirection(double);
	void setTurretControlState(TurretControlType, double, int);
	void setRemoteDirection(double value);

	void setMaxTurnSpeed(double, bool) override;
	double getTurretPhysicsDirection() override;
	void rotate(double, Matrix3*) override;
	void interpolate(double, int) override;
	double getTurretInterpolatedDirection() override;
	double getBarrelInterpolatedElevation() override;
	void setBarrelElevation(double value) override;
	virtual void updatePhysics(Body*) override;
	virtual void reset() override;

private:
	bool isLocked();
	bool isNotLocked();
	void setRealControlState(TurretControlType realControlType, double realControlInput);
	void rotateToLocalDirection(double, double);
	double calculateTurnSpeed(double, double, double);
	double getLocalDirectionFromWorldDirection(double, Matrix3*);
	void updateDirectionalRotation(double);
	void applyGyroscopeEffect(Body *body);
};

#endif // TURRET_H
