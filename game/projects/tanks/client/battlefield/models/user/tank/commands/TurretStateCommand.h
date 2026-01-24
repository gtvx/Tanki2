#ifndef TURRETSTATECOMMAND_H
#define TURRETSTATECOMMAND_H

#include "TurretControlType.h"

class TurretStateCommand
{
public:
	TurretStateCommand();
	double controlInput;
	TurretControlType controlType;
	double direction;
	int rotationSpeedNumber;
};

#endif // TURRETSTATECOMMAND_H
