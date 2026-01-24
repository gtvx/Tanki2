#include "IRotatingTurretModelAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


IRotatingTurretModelAdapt::IRotatingTurretModelAdapt(IGameObject *object, std::shared_ptr<IRotatingTurretModel> impl)
{
	this->object = object;
	this->impl = impl;
}


LocalTurretController* IRotatingTurretModelAdapt::getLocalTurretController()
{
	LocalTurretController *result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getLocalTurretController();
	}
	catch (...)
	{
		result = nullptr;
	}

	Model::popObject();

	return result;
}


Turret* IRotatingTurretModelAdapt::getTurret()
{
	Turret* result;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getTurret();
	}
	catch (...)
	{
		result = nullptr;
	}

	Model::popObject();

	return result;
}
