#ifndef SHELLSTATE_H
#define SHELLSTATE_H

#include "game/projects/tanks/client/battlefield/types/Vector3d.h"

class ShellState
{
	int _clientTime;
	Vector3d _direction;
	Vector3d _position;

public:
	ShellState(int clientTime, const Vector3d &direction, const Vector3d &position)
	{
		_clientTime = clientTime;
		_direction = direction;
		_position = position;
	}

};

#endif // SHELLSTATE_H
