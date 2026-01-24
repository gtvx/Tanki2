#include "TankConfigurationAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


TankConfigurationAdapt::TankConfigurationAdapt(IGameObject *object, std::shared_ptr<TankConfiguration> impl)
{
	this->object = object;
	this->impl = impl;
}


IGameObject* TankConfigurationAdapt::getHullObject()
{
	IGameObject *result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getHullObject();
	}
	catch (...)
	{
		result = nullptr;
	}

	Model::popObject();

	return result;
}


IGameObject* TankConfigurationAdapt::getWeaponObject()
{

	IGameObject *result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getWeaponObject();
	}
	catch (...)
	{
		result = nullptr;
	}

	Model::popObject();

	return result;
}


IGameObject* TankConfigurationAdapt::getColoringObject()
{
	IGameObject *result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getColoringObject();
	}
	catch (...)
	{
		result = nullptr;
	}

	Model::popObject();

	return result;
}
