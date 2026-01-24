#ifndef WEAPONCOMMONDATA_H
#define WEAPONCOMMONDATA_H

#include "game/alternativa/tanks/utils/EncryptedNumber.h"

class WeaponCommonData
{
	EncryptedNumber *maxTurretRotationSpeed;
	EncryptedNumber *turretRotationAcceleration;
	EncryptedNumber *impactForce;
	EncryptedNumber *recoilForce;

public:
	WeaponCommonData(double maxTurretRotationSpeed, double turretRotationAcceleration, double impactForce, double recoilForce);
	double getMaxTurretRotationSpeed() const;
	double getTurretRotationAcceleration() const;
	double getImpactForce() const;
	double getRecoilForce() const;
	void setRecoilForce(double value);
};

#endif // WEAPONCOMMONDATA_H
