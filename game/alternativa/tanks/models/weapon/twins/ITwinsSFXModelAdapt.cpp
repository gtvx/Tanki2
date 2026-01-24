#include "ITwinsSFXModelAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


ITwinsSFXModelAdapt::ITwinsSFXModelAdapt(IGameObject *object, std::shared_ptr<ITwinsSFXModel> impl)
{
	this->object = object;
	this->impl = impl;
}


TwinsEffects* ITwinsSFXModelAdapt::getPlasmaWeaponEffects()
{
	TwinsEffects *result = nullptr;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getPlasmaWeaponEffects();
	}
	catch(...)
	{

	}

	Model::popObject();

	return result;
}


TwinsSFXData* ITwinsSFXModelAdapt::getSFXData()
{
	TwinsSFXData *result = nullptr;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getSFXData();
	}
	catch(...)
	{

	}

	Model::popObject();

	return result;
}
