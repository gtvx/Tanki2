#include "VerticalAnglesAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


VerticalAnglesAdapt::VerticalAnglesAdapt(IGameObject *object, std::shared_ptr<VerticalAngles> impl)
{
	this->object = object;
	this->impl = impl;

}


double VerticalAnglesAdapt::getAngleUp()
{
	double result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getAngleUp();
	}
	catch (...)
	{
		result = 0;
	}

	Model::popObject();

	return result;
}


double VerticalAnglesAdapt::getAngleDown()
{
	double result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getAngleDown();
	}
	catch (...)
	{
		result = 0;
	}

	Model::popObject();

	return result;
}
