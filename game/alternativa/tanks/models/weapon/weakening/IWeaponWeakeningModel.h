#ifndef IWEAPONWEAKENINGMODEL_H
#define IWEAPONWEAKENINGMODEL_H

class DistanceWeakening;

class IWeaponWeakeningModel
{
public:
	virtual void getDistanceWeakening(DistanceWeakening*) = 0;
};

#endif // IWEAPONWEAKENINGMODEL_H
