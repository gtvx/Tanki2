#ifndef TANKCONFIGURATIONMODEL_H
#define TANKCONFIGURATIONMODEL_H

#include "game/projects/tanks/client/battlefield/models/user/configuration/TankConfigurationModelBase.h"
#include "TankConfiguration.h"

//extends TankConfigurationModelBase implements ITankConfigurationModelBase, TankConfiguration

class TankConfigurationModel : public TankConfigurationModelBase, public TankConfiguration
{
public:
	TankConfigurationModel();
	IGameObject* getHullObject();
	IGameObject* getWeaponObject();
	IGameObject* getColoringObject();
};

#endif // TANKCONFIGURATIONMODEL_H
