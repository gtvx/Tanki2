#ifndef SECTORDIRECTIONCALCULATOR_H
#define SECTORDIRECTIONCALCULATOR_H

#include "splitter/SectorsSplitter.h"
#include "TargetingSectorsCalculator.h"
#include "TargetingSectorList.h"
#include "../../priority/TargetingPriorityCalculator.h"
#include "../../direction/TargetingDirectionCalculator.h"

class SectorDirectionCalculator : public TargetingDirectionCalculator
{
	TargetingSectorsCalculator *targetingSectorsCalculator;
	SectorsSplitter *sectorsSplitter;

public:
	SectorDirectionCalculator(IGameObject*, WeaponObject*, double maxDistance, TargetingPriorityCalculator*);
	QVector<TargetingDirection *> *getDirections(AllGlobalGunParams*);
};

#endif // SECTORDIRECTIONCALCULATOR_H
