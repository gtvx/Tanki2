#ifndef ROTATINGTURRETCC_H
#define ROTATINGTURRETCC_H

#include <memory>

class TurretStateCommand;

class RotatingTurretCC
{
public:
	std::shared_ptr<TurretStateCommand> turretState;
};

#endif // ROTATINGTURRETCC_H
