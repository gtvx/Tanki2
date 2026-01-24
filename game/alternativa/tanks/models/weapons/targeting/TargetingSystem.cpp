#include "TargetingSystem.h"
#include "Error.h"
#include <QVector>
#include "_global.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/TargetingDirection.h"
#include "game/alternativa/tanks/models/weapon/AllGlobalGunParams.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/TargetingDirectionCalculator.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "game/alternativa/tanks/models/weapons/targeting/TargetingResult.h"
#include "game/alternativa/tanks/models/weapons/targeting/priority/TargetingPriorityCalculator.h"
#include "game/alternativa/tanks/models/weapons/targeting/processor/TargetingDirectionProcessor.h"



void TargetingSystem::init(TargetingDirectionCalculator *directionCalculator,
						   TargetingDirectionProcessor *directionProcessor,
						   TargetingPriorityCalculator *priorityCalculator)
{
	this->result = new TargetingResult();
	this->directionCalculator = directionCalculator;
	this->directionProcessor = directionProcessor;
	this->priorityCalculator = priorityCalculator;
}


TargetingDirectionProcessor* TargetingSystem::getProcessor()
{
	return this->directionProcessor;
}


static void sort(QVector<TargetingDirection*> *v)
{
	std::sort(v->begin(), v->end(), [](const TargetingDirection* a, const TargetingDirection* b){
		return 0 < (b->getMaxPriority() - a->getMaxPriority());
	});
}


TargetingResult* TargetingSystem::target(AllGlobalGunParams *allGlobalGunParams)
{
	QVector<TargetingDirection*> *_loc2_ = this->directionCalculator->getDirections(allGlobalGunParams);
	_loc2_->append(new TargetingDirection(&allGlobalGunParams->direction, 0, 0));

	sort(_loc2_);

	double _loc3_ = NEGATIVE_INFINITY();

	for (int i = 0; i < _loc2_->count(); i++)
	{
		TargetingDirection *targetingDirection = _loc2_->at(i);
		if (targetingDirection->getMaxPriority() < _loc3_)
		{
			break;
		}
		QVector<RayHit> *_loc5_ = this->directionProcessor->process(allGlobalGunParams, targetingDirection->getDirection());
		double _loc6_ = this->priorityCalculator->getPriorityForRayHits(targetingDirection->getAngle(), _loc5_);
		if (_loc6_ > 0)
		{
			_loc6_ = _loc6_ + targetingDirection->getBonusPriority();
		}
		if ((_loc6_ > _loc3_) || ((_loc6_ == _loc3_) && (targetingDirection->getAngle() == 0)))
		{
			_loc3_ = _loc6_;
			this->result->setData(targetingDirection->getDirection(), _loc5_);
		}
	}

	return this->result;
}
