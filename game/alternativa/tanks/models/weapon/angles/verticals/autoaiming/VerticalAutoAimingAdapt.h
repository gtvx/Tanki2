#ifndef VERTICALAUTOAIMINGADAPT_H
#define VERTICALAUTOAIMINGADAPT_H

#include "VerticalAutoAiming.h"
#include <memory>

class IGameObject;

class VerticalAutoAimingAdapt : public VerticalAutoAiming
{
	IGameObject *object;
	std::shared_ptr<VerticalAutoAiming> impl;

public:
	VerticalAutoAimingAdapt(IGameObject *object, std::shared_ptr<VerticalAutoAiming> impl);

private:
	double getElevationAngleUp() override;
	int getNumRaysUp() override;
	double getElevationAngleDown() override;
	int getNumRaysDown() override;
	double getMaxAngle() override;
};

#endif // VERTICALAUTOAIMINGADAPT_H
