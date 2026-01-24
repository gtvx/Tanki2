#include "VerticalAutoAimingModel.h"
#include "MyMath.h"
#include "../verticalangles/VerticalAngles.h"
#include "game/alternativa/tanks/models/weapon/weakening/DistanceWeakening.h"
#include "game/alternativa/tanks/models/weapon/weakening/IWeaponWeakeningModel.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "Error.h"


static const double MAXIMUM_RAYS_PER_RADIAN = ((8 * 180) / MyMath::PI); //458.366236104659
static const double MIN_TANK_SIZE = 90;


VerticalAutoAimingModel::VerticalAutoAimingModel()
{

}


int VerticalAutoAimingModel::getNumRays(double num)
{
	double result;

	if (object()->hasModel<IWeaponWeakeningModel>())
	{
		DistanceWeakening distanceWeakening;
		object()->adapt<IWeaponWeakeningModel>()->getDistanceWeakening(&distanceWeakening);
		result = calculateRaysPerRadian(distanceWeakening.getFullDamageDistance());
	}
	else
	{
		result = MAXIMUM_RAYS_PER_RADIAN;
	}

	return MyMath::ceil(num * result);
}


double VerticalAutoAimingModel::calculateRaysPerRadian(double value)
{
	return MyMath::min(MAXIMUM_RAYS_PER_RADIAN, (1. / (2. * MyMath::atan((MIN_TANK_SIZE / (2 * value))))));
}


double VerticalAutoAimingModel::getElevationAngleUp()
{
	return this->verticalAngles()->getAngleUp();
}


int VerticalAutoAimingModel::getNumRaysUp()
{
	return getNumRays(this->getElevationAngleUp());
}

double VerticalAutoAimingModel::getElevationAngleDown()
{
	return this->verticalAngles()->getAngleDown();
}


int VerticalAutoAimingModel::getNumRaysDown()
{
	return getNumRays(this->getElevationAngleDown());
}


double VerticalAutoAimingModel::getMaxAngle()
{
	return MyMath::max(this->getElevationAngleDown(), this->getElevationAngleUp());
}


VerticalAngles* VerticalAutoAimingModel::verticalAngles()
{
	auto result = object()->adapt<VerticalAngles>().get();

	if (result == nullptr)
	{
		throw Error("VerticalAutoAimingModel::verticalAngles");
	}

	return result;
}
