#include "RemoteSmokyWeapon.h"
#include "Error.h"


RemoteSmokyWeapon::RemoteSmokyWeapon(WeaponForces *weaponForces, ISmokyEffects *effects)
{
	this->weaponPlatform = nullptr;
	this->weaponForces = weaponForces;
	this->effects = effects;
}


void RemoteSmokyWeapon::init(WeaponPlatform *weaponPlatform)
{
	 this->weaponPlatform = weaponPlatform;
}


void RemoteSmokyWeapon::destroy()
{

}


void RemoteSmokyWeapon::activate()
{

}


void RemoteSmokyWeapon::deactivate()
{

}


void RemoteSmokyWeapon::enable()
{

}


void RemoteSmokyWeapon::disable(bool)
{

}

void RemoteSmokyWeapon::stun()
{

}


void RemoteSmokyWeapon::calm(int)
{

}


void RemoteSmokyWeapon::reset()
{

}

double RemoteSmokyWeapon::getStatus()
{
	return 0;
}


ItemProperty *RemoteSmokyWeapon::getResistanceProperty()
{
	throw Error(180);
}


void RemoteSmokyWeapon::updateRecoilForce(double)
{
	throw Error(181);
}


void RemoteSmokyWeapon::fullyRecharge()
{
	throw Error(182);
}


void RemoteSmokyWeapon::weaponReloadTimeChanged(int, int)
{

}

SimpleWeaponController *RemoteSmokyWeapon::getController()
{
	return nullptr;
}


DistanceWeakening *RemoteSmokyWeapon::getWeakening()
{
	return nullptr;
}


bool RemoteSmokyWeapon::check(Tank *)
{
	return false;
}
