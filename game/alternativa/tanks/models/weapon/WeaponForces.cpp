#include "WeaponForces.h"
#include "game/alternativa/tanks/utils/EncryptedNumberImpl.h"


WeaponForces::WeaponForces(double impactForce, double recoilForce)
{
	this->impactForce = new EncryptedNumberImpl(impactForce);
	this->recoilForce = new EncryptedNumberImpl(recoilForce);
}


double WeaponForces::getImpactForce() const
{
   return this->impactForce->getNumber();
}


double WeaponForces::getRecoilForce() const
{
   return this->recoilForce->getNumber();
}


void WeaponForces::setRecoilForce(double value)
{
   this->recoilForce->setNumber(value);
}
