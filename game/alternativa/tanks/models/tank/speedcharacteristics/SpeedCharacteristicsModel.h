#ifndef SPEEDCHARACTERISTICSMODEL_H
#define SPEEDCHARACTERISTICSMODEL_H

#include "game/projects/tanks/client/battlefield/models/user/speedcharacteristics/SpeedCharacteristicsModelBase.h"
#include "SpeedCharacteristics.h"

struct CurrentSpeedValues;
class Tank;

// SpeedCharacteristicsModelBase implements ISpeedCharacteristicsModelBase, ObjectLoadListener, SpeedCharacteristics

class SpeedCharacteristicsModel :
		public SpeedCharacteristicsModelBase,
		public SpeedCharacteristics
{
public:
	SpeedCharacteristicsModel();

	void setInitialTankState() override;
	double getMaxHullTurnSpeed() override;
	double getMaxTurretTurnSpeed() override;
	double getTurretRotationCoefficient() override;

private:
	CurrentSpeedValues *getCurrentValues();
	void setTankSpec(Tank *tank, double speed, double param3, double turretRotationSpeed, double acceleration, bool b);
};

#endif // SPEEDCHARACTERISTICSMODEL_H
