#include "IShotModelAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


IShotModelAdapt::IShotModelAdapt(IGameObject *object, std::shared_ptr<IShotModel> impl)
{
	this->object = object;
	this->impl = impl;
}


int IShotModelAdapt::getReloadMS()
{
	int result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getReloadMS();
	}
	catch (...)
	{
		result = 0;
	}

	Model::popObject();

	return result;
}
