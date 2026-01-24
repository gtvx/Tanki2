#include "SectorsSplitter.h"
#include "_global.h"
#include "MyMath.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/sector/TargetingSector.h"
#include "game/alternativa/tanks/models/weapons/targeting/priority/TargetingPriorityCalculator.h"
#include "game/alternativa/tanks/models/weapon/AllGlobalGunParams.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/TargetingDirection.h"

static int DIRECTIONS_PER_SECTOR = 16;


void SectorsSplitter::directions_clear()
{
	qDeleteAll(directions);
	directions.clear();
}


SectorsSplitter::SectorsSplitter(TargetingPriorityCalculator *targetPriority)
{
	this->targetPriority = targetPriority;
}


QVector<TargetingDirection*>* SectorsSplitter::splitSectorsOnDirections(AllGlobalGunParams *allGlobalGunParams, TargetingSectorList *sectors)
{
	this->directions_clear();
	this->currentOverlapSectors.clear();

	double value = 0;

	for (int i = 0; i < sectors->count(); i++)
	{
		TargetingSector *targetingSector = sectors->at(i);
		const double left = targetingSector->getLeft();
		while (this->currentOverlapSectors.isEmpty() == false && value < left)
		{
			double temp = MyMath::min(this->currentOverlapSectors.getRight(), left);
			this->addDirections(allGlobalGunParams, value, temp);
			this->currentOverlapSectors.removeSectorsWhichRightCoordIsLessOrEqualThan(temp);
			value = temp;
		}
		value = left;
		this->currentOverlapSectors.add(targetingSector);
	}

	while (this->currentOverlapSectors.isEmpty() == false)
	{
		const double right = this->currentOverlapSectors.getRight();
		this->addDirections(allGlobalGunParams, value, right);
		this->currentOverlapSectors.removeSectorsWhichRightCoordIsLessOrEqualThan(right);
		value = right;
	}

	return &this->directions;
}


void SectorsSplitter::addDirections(AllGlobalGunParams *allGlobalGunParams, double _arg_2, double _arg_3)
{
	const double _local_4 = _arg_3 - _arg_2;
	const double _local_5 = _local_4 / DIRECTIONS_PER_SECTOR;
	double _local_6 = _arg_2 + (_local_5 * 0.5);
	QVector<TargetingSector*> *sectors = this->currentOverlapSectors.getOrderedByDistance();
	this->matrix.fromAxisAngle(&allGlobalGunParams->elevationAxis, _local_6);
	this->matrix.transformVector(&allGlobalGunParams->direction, &this->direction);
	this->matrix.fromAxisAngle(&allGlobalGunParams->elevationAxis, _local_5);
	int index = 0;
	while (index < DIRECTIONS_PER_SECTOR)
	{
		this->directions.append(new TargetingDirection(&this->direction, _local_6, this->targetPriority->getPriorityForSectors(_local_6, sectors)));
		Vector3 v;
		v.copy(&this->direction);
		this->matrix.transformVector(&v, &this->direction);
		_local_6 += _local_5;
		index++;
	}
	if (_arg_2 <= 0 && 0 <= _arg_3)
	{
		this->directions.append(new TargetingDirection(&allGlobalGunParams->direction, 0, this->targetPriority->getPriorityForSectors(0, sectors)));
	}
}
