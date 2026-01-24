#ifndef TANKSTATE_H
#define TANKSTATE_H

#include "game/projects/tanks/client/battlefield/types/Vector3d.h"

class TankState
{
	Vector3d _angularVelocity;
	int _chassisControl;
	int _chassisTurnSpeedNumber;
	Vector3d _linearVelocity;
	Vector3d _orientation;
	Vector3d _position;

public:
	TankState(const Vector3d *angularVelocity = nullptr, int chassisControl = 0, int hassisTurnSpeedNumber = 0,
			  const Vector3d *linearVelocity = nullptr, const Vector3d *orientation = nullptr, const Vector3d *position = nullptr);

	void set_orientation(const Vector3d *orientation);
	Vector3d *get_orientation();
	void set_position(const Vector3d *position);
	Vector3d *get_position();
	void set_chassisControl(int chassisControl);
	int get_chassisControl();
	void set_chassisTurnSpeedNumberl(int chassisTurnSpeedNumber);
	int get_chassisTurnSpeedNumber();
};

#endif // TANKSTATE_H
