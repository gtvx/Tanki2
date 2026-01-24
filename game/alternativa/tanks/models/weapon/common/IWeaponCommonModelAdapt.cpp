#include "IWeaponCommonModelAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


IWeaponCommonModelAdapt::IWeaponCommonModelAdapt(IGameObject *object, std::shared_ptr<IWeaponCommonModel> impl)
{
	this->object = object;
	this->impl = impl;
}


std::shared_ptr<WeaponCommonData> IWeaponCommonModelAdapt::getCommonData()
{
	std::shared_ptr<WeaponCommonData> result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getCommonData();
	}
	catch (...)
	{

	}

	Model::popObject();

	return result;
}


void IWeaponCommonModelAdapt::storeTank(std::shared_ptr<Tank> tank)
{
	try
	{
		Model::set_object(this->object);
		this->impl->storeTank(tank);
	}

	catch (...)
	{

	}

	Model::popObject();
}


Tank* IWeaponCommonModelAdapt::getTank()
{
	Tank *result = nullptr;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getTank();
	}
	catch (...)
	{

	}

	Model::popObject();

	return result;
}


void IWeaponCommonModelAdapt::getGunParams(AllGlobalGunParams *allGunParams, int barrelIndex)
{
	try
	{
		Model::set_object(this->object);
		this->impl->getGunParams(allGunParams, barrelIndex);
	}
	catch(...)
	{

	}

	Model::popObject();
}
