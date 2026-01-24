#include "IWeaponWeakeningModelAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"
#include <QDebug>


IWeaponWeakeningModelAdapt::IWeaponWeakeningModelAdapt(IGameObject *object, std::shared_ptr<IWeaponWeakeningModel> impl)
{
	this->object = object;
	this->impl = impl;
}


void IWeaponWeakeningModelAdapt::getDistanceWeakening(DistanceWeakening *distanceWeakening)
{
	try
	{
		Model::set_object(this->object);
		this->impl->getDistanceWeakening(distanceWeakening);
	}
	catch (...)
	{
		qDebug("error IWeaponWeakeningModelAdapt::getDistanceWeakening");
	}

	Model::popObject();
}
