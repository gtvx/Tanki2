#include "WeaponVerticalAnglesModel.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/angles/verticals/verticalangles/VerticalAnglesCC.h"


WeaponVerticalAnglesModel::WeaponVerticalAnglesModel()
{

}


double WeaponVerticalAnglesModel::getAngleUp()
{
	return getInitParam()->angleUp;
}


double WeaponVerticalAnglesModel::getAngleDown()
{
	return getInitParam()->angleDown;
}
