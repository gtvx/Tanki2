#include "InitTankPartAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


InitTankPartAdapt::InitTankPartAdapt(IGameObject *object, std::shared_ptr<InitTankPart> impl)
{
	this->object = object;
	this->impl = impl;
}


void InitTankPartAdapt::initTankPart(std::shared_ptr<Tank> tank)
{
	try
	{
		Model::set_object(this->object);
		this->impl->initTankPart(tank);
	}
	catch (...)
	{

	}

	Model::popObject();
}


