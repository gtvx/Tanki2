#ifndef IROTATINGTURRETMODELADAPT_H
#define IROTATINGTURRETMODELADAPT_H

#include "IRotatingTurretModel.h"
#include <memory>

class IGameObject;

class IRotatingTurretModelAdapt : public IRotatingTurretModel
{
	IGameObject *object;
	std::shared_ptr<IRotatingTurretModel> impl;

public:
	IRotatingTurretModelAdapt(IGameObject *object, std::shared_ptr<IRotatingTurretModel> impl);

private:
	LocalTurretController* getLocalTurretController() override;
	Turret* getTurret() override;
};

#endif // IROTATINGTURRETMODELADAPT_H
