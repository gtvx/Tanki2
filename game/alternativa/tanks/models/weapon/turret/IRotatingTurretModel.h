#ifndef IROTATINGTURRETMODEL_H
#define IROTATINGTURRETMODEL_H

class LocalTurretController;
class Turret;

class IRotatingTurretModel
{
public:
	virtual LocalTurretController* getLocalTurretController() = 0;
	virtual Turret* getTurret() = 0;
};

#endif // IROTATINGTURRETMODEL_H
