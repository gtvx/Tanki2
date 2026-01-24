#include "RemoteTwinsWeapon.h"
#include "ITwinsSFXModel.h"
#include "TwinsAmmunition.h"
#include "game/alternativa/tanks/models/weapon/WeaponObject.h"
#include "game/alternativa/tanks/models/weapon/common/WeaponCommonData.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include <QDebug>


RemoteTwinsWeapon::RemoteTwinsWeapon(IGameObject *object, TwinsCC *cc)
{
	WeaponObject *weaponObject = new WeaponObject(object);
	WeaponCommonData *weaponCommonData = weaponObject->commonData();
	ITwinsSFXModel *sfx= object->adapt<ITwinsSFXModel>().get();
	TwinsAmmunition *_local_6 = new TwinsAmmunition(weaponObject, cc, sfx->getSFXData());
	this->recoilForce = weaponCommonData->getRecoilForce();
	this->ammunition = _local_6;
	this->effects = sfx->getPlasmaWeaponEffects();
}


void RemoteTwinsWeapon::init(WeaponPlatform *weaponPlatform)
{
	this->weaponPlatform = weaponPlatform;
}


void RemoteTwinsWeapon::destroy()
{

}


void RemoteTwinsWeapon::activate()
{

}

void RemoteTwinsWeapon::deactivate()
{

}


void RemoteTwinsWeapon::enable()
{

}


void RemoteTwinsWeapon::disable(bool)
{

}


void RemoteTwinsWeapon::stun()
{

}


void RemoteTwinsWeapon::calm(int)
{

}


void RemoteTwinsWeapon::reset()
{

}


double RemoteTwinsWeapon::getStatus()
{
	return 0;
}


ItemProperty* RemoteTwinsWeapon::getResistanceProperty()
{
	qDebug("used RemoteTwinsWeapon::getResistanceProperty");
	return nullptr;
	//return ItemProperty::TWINS_RESISTANCE;
}


void RemoteTwinsWeapon::updateRecoilForce(double)
{

}


void RemoteTwinsWeapon::fullyRecharge()
{

}


void RemoteTwinsWeapon::weaponReloadTimeChanged(int, int)
{

}


bool RemoteTwinsWeapon::check(Tank *)
{
	return false;
}


SimpleWeaponController* RemoteTwinsWeapon::getController()
{
	qDebug("used RemoteTwinsWeapon::getController");
	return nullptr;
}


DistanceWeakening* RemoteTwinsWeapon::getWeakening()
{
	qDebug("used RemoteTwinsWeapon::getWeakening");
	return nullptr;
}
