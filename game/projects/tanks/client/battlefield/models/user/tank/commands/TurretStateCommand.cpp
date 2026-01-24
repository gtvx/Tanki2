#include "TurretStateCommand.h"

TurretStateCommand::TurretStateCommand()
{
	controlInput = 0;
	controlType = TurretControlType::TARGET_ANGLE_LOCAL;
	direction = 0;
	rotationSpeedNumber = 0;
}
