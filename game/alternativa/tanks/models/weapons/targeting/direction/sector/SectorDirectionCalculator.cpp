#include "SectorDirectionCalculator.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/sector/TargetingSectorsCalculator.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/sector/splitter/SectorsSplitter.h"


SectorDirectionCalculator::SectorDirectionCalculator(IGameObject *param1, WeaponObject *weaponObject, double maxDistance, TargetingPriorityCalculator *param4)
{
	this->targetingSectorsCalculator = new TargetingSectorsCalculator(param1, weaponObject, maxDistance);
	this->sectorsSplitter = new SectorsSplitter(param4);
}


QVector<TargetingDirection*>* SectorDirectionCalculator::getDirections(AllGlobalGunParams *allGlobalGunParams)
{
	TargetingSectorList *sectors = this->targetingSectorsCalculator->getSectors(allGlobalGunParams);
	return this->sectorsSplitter->splitSectorsOnDirections(allGlobalGunParams, sectors);
}
