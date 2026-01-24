#ifndef WEAPONFORCES_H
#define WEAPONFORCES_H

class EncryptedNumber;

class WeaponForces
{
	EncryptedNumber *impactForce;
	EncryptedNumber *recoilForce;

public:
	WeaponForces(double impactForce, double recoilForce);
	double getImpactForce() const;
	double getRecoilForce() const;
	void setRecoilForce(double value);
};

#endif // WEAPONFORCES_H
