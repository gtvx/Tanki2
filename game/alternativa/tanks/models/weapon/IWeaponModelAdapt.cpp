#include "IWeaponModelAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"
#include <QDebug>


IWeaponModelAdapt::IWeaponModelAdapt(IGameObject *object, std::shared_ptr<IWeaponModel> impl)
{
	this->object = object;
	this->s_impl = impl;
	this->impl = impl.get();
}


Weapon* IWeaponModelAdapt::createLocalWeapon(IGameObject *user)
{
	Weapon *result = nullptr;

	try
	{
		Model::set_object(this->object);
		result = this->impl->createLocalWeapon(user);
	}
	catch(...)
	{
		qDebug() << "error IWeaponModelAdapt::createLocalWeapon";
	}

	Model::popObject();

	return result;
}


Weapon* IWeaponModelAdapt::createRemoteWeapon(IGameObject *user)
{
	Weapon *result = nullptr;

	try
	{
		Model::set_object(this->object);
		result = this->impl->createRemoteWeapon(user);
	}
	catch(...)
	{
		qDebug() << "error IWeaponModelAdapt::createRemoteWeapon";
	}

	Model::popObject();

	return result;
}
