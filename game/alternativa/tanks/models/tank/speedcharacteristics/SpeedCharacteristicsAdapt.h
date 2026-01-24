#ifndef SPEEDCHARACTERISTICSADAPT_H
#define SPEEDCHARACTERISTICSADAPT_H

#include "SpeedCharacteristics.h"
#include <memory>

class IGameObject;

class SpeedCharacteristicsAdapt : public SpeedCharacteristics
{
	IGameObject *object;
	std::shared_ptr<SpeedCharacteristics> impl;

public:
	SpeedCharacteristicsAdapt(IGameObject *object, std::shared_ptr<SpeedCharacteristics> impl);

private:
	void setInitialTankState() override;
	double getMaxHullTurnSpeed() override;
	double getMaxTurretTurnSpeed() override;
	double getTurretRotationCoefficient() override;
};

#endif // SPEEDCHARACTERISTICSADAPT_H
