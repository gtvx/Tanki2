#include "WeaponMountProviderAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"
#include <QDebug>


WeaponMountProviderAdapt::WeaponMountProviderAdapt(IGameObject *object, std::shared_ptr<WeaponMountProvider> impl)
{
	this->object = object;
	this->impl = impl;
}


std::shared_ptr<WeaponMount> WeaponMountProviderAdapt::createWeaponMount(IGameObject *tankObject)
{
	std::shared_ptr<WeaponMount> result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->createWeaponMount(tankObject);
	}
	catch(...)
	{
		qDebug("error WeaponMountProviderAdapt::createWeaponMount");
	}

	Model::popObject();

	return result;
}
