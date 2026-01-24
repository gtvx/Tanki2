#ifndef SECTORSSPLITTER_H
#define SECTORSSPLITTER_H

#include <QVector>
#include "game/alternativa/math/Matrix3.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/sector/splitter/SortedTargetingSectors.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/sector/TargetingSectorList.h"

class TargetingPriorityCalculator;
class TargetingDirection;
class AllGlobalGunParams;
class TargetingSector;

class SectorsSplitter
{
	SortedTargetingSectors currentOverlapSectors;
	QVector<TargetingDirection*> directions;
	TargetingPriorityCalculator *targetPriority;
	Matrix3 matrix;
	Vector3 direction;

	void directions_clear();

public:
	SectorsSplitter(TargetingPriorityCalculator *targetPriority);
	QVector<TargetingDirection*> *splitSectorsOnDirections(AllGlobalGunParams *allGlobalGunParams, TargetingSectorList *sectors);

private:
	void addDirections(AllGlobalGunParams *allGlobalGunParams, double, double);
};

#endif // SECTORSSPLITTER_H
