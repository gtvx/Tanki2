#include "IObject3DSAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


IObject3DSAdapt::IObject3DSAdapt(IGameObject *object, std::shared_ptr<IObject3DS> impl)
{
	this->object = object;
	this->impl = impl;
}


Tanks3DSResource* IObject3DSAdapt::getResource3DS()
{
	Tanks3DSResource *result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getResource3DS();
	}
	catch (...)
	{
		result = nullptr;
	}

	Model::popObject();

	return result;
}
