#include "SpeedCharacteristicsAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


SpeedCharacteristicsAdapt::SpeedCharacteristicsAdapt(IGameObject *object,
													 std::shared_ptr<SpeedCharacteristics> impl)
{
	this->object = object;
	this->impl = impl;
}


void SpeedCharacteristicsAdapt::setInitialTankState()
{
	try
	{
		Model::set_object(this->object);
		this->impl->setInitialTankState();
	}
	catch (...)
	{

	}

	Model::popObject();
}


double SpeedCharacteristicsAdapt::getMaxHullTurnSpeed()
{
	double result = 0;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getMaxHullTurnSpeed();
	}
	catch (...)
	{

	}

	Model::popObject();

	return result;
}


double SpeedCharacteristicsAdapt::getMaxTurretTurnSpeed()
{
	double result = 0;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getMaxTurretTurnSpeed();
	}
	catch (...)
	{

	}

	Model::popObject();

	return result = 0;
}


double SpeedCharacteristicsAdapt::getTurretRotationCoefficient()
{
	double result = 0;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getTurretRotationCoefficient();
	}
	catch (...)
	{

	}

	Model::popObject();

	return result;
}
