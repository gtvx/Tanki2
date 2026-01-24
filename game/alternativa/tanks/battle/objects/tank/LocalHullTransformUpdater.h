#ifndef LOCALHULLTRANSFORMUPDATER_H
#define LOCALHULLTRANSFORMUPDATER_H

#include "game/alternativa/math/Vector3.h"
#include "Tank.h"
#include "HullTransformUpdater.h"

class LocalHullTransformUpdater : public HullTransformUpdater
{
	Vector3 position;
	Vector3 eulerAngles;
	Tank *tank;

public:
	LocalHullTransformUpdater(Tank *tank);
	void reset();
	void update(double param1);
};

#endif // LOCALHULLTRANSFORMUPDATER_H
