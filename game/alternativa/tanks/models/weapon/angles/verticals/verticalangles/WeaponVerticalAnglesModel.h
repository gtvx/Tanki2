#ifndef WEAPONVERTICALANGLESMODEL_H
#define WEAPONVERTICALANGLESMODEL_H

#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/angles/verticals/verticalangles/WeaponVerticalAnglesModelBase.h"
#include "VerticalAngles.h"

//extends WeaponVerticalAnglesModelBase implements IWeaponVerticalAnglesModelBase, VerticalAngles

class WeaponVerticalAnglesModel : public WeaponVerticalAnglesModelBase, public VerticalAngles
{
public:
	WeaponVerticalAnglesModel();

	double getAngleUp() override;
	double getAngleDown() override;
};

#endif // WEAPONVERTICALANGLESMODEL_H
