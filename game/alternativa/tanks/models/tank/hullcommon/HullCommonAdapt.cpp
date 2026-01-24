#include "HullCommonAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


HullCommonAdapt::HullCommonAdapt(IGameObject *object, std::shared_ptr<HullCommon> impl)
{
	this->object = object;
	this->impl = impl;
}


double HullCommonAdapt::getMass()
{
	double result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getMass();
	}
	catch (...)
	{
		result = 0;
	}

	Model::popObject();

	return result;
}
