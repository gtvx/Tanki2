#include "VerticalAutoAimingAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


VerticalAutoAimingAdapt::VerticalAutoAimingAdapt(IGameObject *object, std::shared_ptr<VerticalAutoAiming> impl)
{
	this->object = object;
	this->impl = impl;
}


double VerticalAutoAimingAdapt::getElevationAngleUp()
{
	double result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getElevationAngleUp();
	}
	catch (...)
	{
		result = 0;
	}

	Model::popObject();

	return result;
}


int VerticalAutoAimingAdapt::getNumRaysUp()
{
	int result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getNumRaysUp();
	}
	catch (...)
	{
		result = 0;
	}

	Model::popObject();

	return result;
}


double VerticalAutoAimingAdapt::getElevationAngleDown()
{
	double result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getElevationAngleDown();
	}
	catch (...)
	{
		result = 0;
	}

	Model::popObject();

	return result;
}


int VerticalAutoAimingAdapt::getNumRaysDown()
{
	int result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getNumRaysDown();
	}
	catch (...)
	{
		result = 0;
	}

	Model::popObject();

	return result;
}


double VerticalAutoAimingAdapt::getMaxAngle()
{
	double result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getMaxAngle();
	}
	catch (...)
	{
		result = 0;
	}

	Model::popObject();

	return result;
}
