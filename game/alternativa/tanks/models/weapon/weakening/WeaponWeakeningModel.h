#ifndef WEAPONWEAKENINGMODEL_H
#define WEAPONWEAKENINGMODEL_H

#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/weakening/WeaponWeakeningModelBase.h"
#include "IWeaponWeakeningModel.h"

class WeaponWeakeningModel :
		public WeaponWeakeningModelBase,
		public IWeaponWeakeningModel
{
public:

	void objectLoaded();

	void getDistanceWeakening(DistanceWeakening *distanceWeakening) override;
};

#endif // WEAPONWEAKENINGMODEL_H
