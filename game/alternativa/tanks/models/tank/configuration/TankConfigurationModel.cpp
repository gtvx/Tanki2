#include "TankConfigurationModel.h"
#include "Error.h"
#include "game/projects/tanks/client/battlefield/models/user/configuration/TankConfigurationCC.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "entrance/platform/client/fp10/core/type/ISpace.h"


TankConfigurationModel::TankConfigurationModel()
{

}


IGameObject* TankConfigurationModel::getHullObject()
{
	auto cc = getInitParam();

	if (cc == nullptr)
	{
		throw Error("TankConfigurationModel::getHullObjec");
	}

	return object()->space()->getObject(cc->hullId);
}


IGameObject* TankConfigurationModel::getWeaponObject()
{
	auto cc = getInitParam();

	if (cc == nullptr)
	{
		throw Error("TankConfigurationModel::getWeaponObject");
	}

	return object()->space()->getObject(cc->weaponId);
}


IGameObject* TankConfigurationModel::getColoringObject()
{
	auto cc = getInitParam();

	if (cc == nullptr)
	{
		throw Error("TankConfigurationModel::getColoringObject");
	}

	return object()->space()->getObject(cc->coloringId);
}

