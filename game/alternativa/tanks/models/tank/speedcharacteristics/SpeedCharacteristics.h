#ifndef SPEEDCHARACTERISTICS_H
#define SPEEDCHARACTERISTICS_H

class SpeedCharacteristics
{
public:
	virtual void setInitialTankState() = 0;
	virtual double getMaxHullTurnSpeed() = 0;
	virtual double getMaxTurretTurnSpeed() = 0;
	virtual double getTurretRotationCoefficient() = 0;
};

#endif // SPEEDCHARACTERISTICS_H
