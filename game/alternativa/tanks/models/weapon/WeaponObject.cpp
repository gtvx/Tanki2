#include "WeaponObject.h"
#include "game/alternativa/tanks/models/weapon/shared/shot/IShotModel.h"
#include "game/alternativa/tanks/models/weapon/weakening/IWeaponWeakeningModel.h"
#include "game/alternativa/tanks/models/weapon/common/IWeaponCommonModel.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "Error.h"


WeaponObject::WeaponObject(IGameObject *object)
{
	this->remote = false;
	this->object = object;
}


VerticalAutoAiming* WeaponObject::verticalAutoAiming()
{
	VerticalAutoAiming *result = object->adapt<VerticalAutoAiming>().get();

	if (result == nullptr)
	{
		throw Error("WeaponObject::verticalAutoAiming");
	}

	return result;
}


int WeaponObject::getReloadTimeMS()
{
	IShotModel *result = object->adapt<IShotModel>().get();

	if (result == nullptr)
	{
		throw Error("WeaponObject::getReloadTimeMS");
	}

	return result->getReloadMS();
}


WeaponCommonData* WeaponObject::commonData()
{
	return this->weaponCommon()->getCommonData().get();
}


IWeaponCommonModel* WeaponObject::weaponCommon()
{
	IWeaponCommonModel *result = object->adapt<IWeaponCommonModel>().get();

	if (result == nullptr)
	{
		throw Error("WeaponObject::weaponCommon");
	}

	return result;
}


void WeaponObject::distanceWeakening(DistanceWeakening *distanceWeakening)
{
	object->adapt<IWeaponWeakeningModel>()->getDistanceWeakening(distanceWeakening);
}


bool WeaponObject::isLocal()
{
	return !this->remote;
}


void WeaponObject::markAsRemote()
{
	this->remote = true;
}
