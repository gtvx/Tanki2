#ifndef REMOTEHULLTRANSFORMUPDATER_H
#define REMOTEHULLTRANSFORMUPDATER_H

#include "HullTransformUpdater.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/math/Quaternion.h"

class Tank;

class RemoteHullTransformUpdater : public HullTransformUpdater
{
	Vector3 smoothedPosition;
	Quaternion smoothedOrientation;
	Tank *tank;

	double smoothValue(double, double, double, double, double);

public:
	RemoteHullTransformUpdater(Tank *tank);
	void reset();
	void update(double);
};

#endif // REMOTEHULLTRANSFORMUPDATER_H
