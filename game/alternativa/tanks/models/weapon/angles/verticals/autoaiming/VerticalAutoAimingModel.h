#ifndef VERTICALAUTOAIMINGMODEL_H
#define VERTICALAUTOAIMINGMODEL_H

#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/angles/verticals/autoaiming/VerticalAutoAimingModelBase.h"
#include "VerticalAutoAiming.h"

class VerticalAngles;

// extends VerticalAutoAimingModelBase implements IVerticalAutoAimingModelBase, VerticalAutoAiming

class VerticalAutoAimingModel : public VerticalAutoAimingModelBase, public VerticalAutoAiming
{
public:
	VerticalAutoAimingModel();

	int getNumRaysDown() override;
	double getElevationAngleDown() override;
	int getNumRaysUp() override;
	double getElevationAngleUp() override;
	double getMaxAngle() override;

	static double calculateRaysPerRadian(double value);

private:
	VerticalAngles *verticalAngles();
	static int getNumRays(double _arg_1);
};

#endif // VERTICALAUTOAIMINGMODEL_H
