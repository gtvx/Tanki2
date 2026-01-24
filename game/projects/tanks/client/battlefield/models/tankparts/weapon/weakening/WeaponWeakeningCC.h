#ifndef WEAPONWEAKENINGCC_H
#define WEAPONWEAKENINGCC_H

class WeaponWeakeningCC
{
public:

	double maximumDamageRadius;
	double minimumDamagePercent;
	double minimumDamageRadius;

	WeaponWeakeningCC(double maximumDamageRadius,
					double minimumDamagePercent,
					double minimumDamageRadius)
	{
		this->maximumDamageRadius = maximumDamageRadius;
		this->minimumDamagePercent = minimumDamagePercent;
		this->minimumDamageRadius = minimumDamageRadius;
	}
};

#endif // WEAPONWEAKENINGCC_H
