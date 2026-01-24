#ifndef TANKCONFIGURATIONADAPT_H
#define TANKCONFIGURATIONADAPT_H

#include "TankConfiguration.h"
#include <memory>

class IGameObject;

class TankConfigurationAdapt : public TankConfiguration
{
	IGameObject *object;
	std::shared_ptr<TankConfiguration> impl;

public:
	TankConfigurationAdapt(IGameObject *object, std::shared_ptr<TankConfiguration> impl);

private:
	IGameObject *getHullObject() override;
	IGameObject *getWeaponObject() override;
	IGameObject *getColoringObject() override;
};

#endif // TANKCONFIGURATIONADAPT_H
