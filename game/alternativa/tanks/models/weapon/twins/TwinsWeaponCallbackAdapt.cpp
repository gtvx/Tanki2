#include "TwinsWeaponCallbackAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"


TwinsWeaponCallbackAdapt::TwinsWeaponCallbackAdapt(IGameObject *object, std::shared_ptr<TwinsWeaponCallback> impl)
{
	this->object = object;
	this->impl = impl;
}


void TwinsWeaponCallbackAdapt::onShot(int time, int shotId, int currentBarrel, const Vector3 *shotDirection)
{
	try
	{
		Model::set_object(this->object);
		this->impl->onShot(time, shotId, currentBarrel, shotDirection);
	}
	catch (...)
	{

	}
}


void TwinsWeaponCallbackAdapt::onDummyShot(int time, int currentBarrel)
{
	try
	{
		Model::set_object(this->object);
		this->impl->onDummyShot(time, currentBarrel);
	}
	catch (...)
	{

	}
}


void TwinsWeaponCallbackAdapt::onTargetHit(int shotId, Body *targetBody, const Vector3 *hitPoint)
{
	try
	{
		Model::set_object(this->object);
		this->impl->onTargetHit(shotId, targetBody, hitPoint);
	}
	catch (...)
	{

	}
}


void TwinsWeaponCallbackAdapt::onStaticHit(int shotId, const Vector3 *position)
{
	try
	{
		Model::set_object(this->object);
		this->impl->onStaticHit(shotId, position);
	}
	catch (...)
	{

	}
}
