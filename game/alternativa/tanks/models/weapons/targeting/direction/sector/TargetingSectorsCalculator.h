#ifndef TARGETINGSECTORSCALCULATOR_H
#define TARGETINGSECTORSCALCULATOR_H

#include "game/alternativa/tanks/models/weapons/targeting/direction/sector/TargetingSectorList.h"

class Vector3;
class IGameObject;
class WeaponObject;
class IGameObject;
class TargetingSector;
class AllGlobalGunParams;
class Tank;

class TargetingSectorsCalculator
{
	Vector3 *upDirection;
	IGameObject *self;
	double maxDistance;
	WeaponObject *weapon;
	TargetingSectorList targetingSectorList;

public:
	TargetingSectorsCalculator(IGameObject *self, WeaponObject *weapon, double maxDistance);
	TargetingSectorList* getSectors(AllGlobalGunParams *allGlobalGunParams);
	void calculateSectorAndAdd(AllGlobalGunParams*, Vector3*, Tank *tank, TargetingSectorList *sector);
};

#endif // TARGETINGSECTORSCALCULATOR_H
