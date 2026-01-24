#include "WeaponCommonData.h"
#include "game/alternativa/tanks/utils/EncryptedNumberImpl.h"


WeaponCommonData::WeaponCommonData(double maxTurretRotationSpeed, double turretRotationAcceleration, double impactForce, double recoilForce)
{
	this->maxTurretRotationSpeed = new EncryptedNumberImpl(maxTurretRotationSpeed);
	this->turretRotationAcceleration = new EncryptedNumberImpl(turretRotationAcceleration);
	this->impactForce = new EncryptedNumberImpl(impactForce);
	this->recoilForce = new EncryptedNumberImpl(recoilForce);
}


double WeaponCommonData::getMaxTurretRotationSpeed() const
{
	return this->maxTurretRotationSpeed->getNumber();
}


double WeaponCommonData::getTurretRotationAcceleration() const
{
	return this->turretRotationAcceleration->getNumber();
}


double WeaponCommonData::getImpactForce() const
{
	return this->impactForce->getNumber();
}


double WeaponCommonData::getRecoilForce() const
{
	return this->recoilForce->getNumber();
}


void WeaponCommonData::setRecoilForce(double value)
{
	this->recoilForce->setNumber(value);
}
