#include "TargetingPriorityCalculator.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/models/weapons/targeting/priority/targeting/CommonTargetPriorityCalculator.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/sector/TargetingSector.h"
#include "MyMath.h"


TargetingPriorityCalculator::TargetingPriorityCalculator(TargetPriorityCalculator *priorityEvalutor, double weakingCoef)
{
	this->priorityEvalutor = priorityEvalutor;
	this->weakingCoef = weakingCoef;
}


double TargetingPriorityCalculator::getPriorityForSectors(double value, QVector<TargetingSector*> *vector)
{
	double v = 0;
	int i = vector->count() - 1;
	while (i >= 0)
	{
		TargetingSector *targetingSector = vector->at(i);
		double d = this->getPriorityForSector(targetingSector, value) + this->weakingCoef * v;
		v = MyMath::max(d, v);
		i--;
	}
	return v;
}


double TargetingPriorityCalculator::getPriorityForSector(TargetingSector *targetingSector, double v)
{
	return this->priorityEvalutor->getTargetPriority(targetingSector->getTank(), targetingSector->getDistance(), v);
}


double TargetingPriorityCalculator::getPriorityForRayHits(double param1, QVector<RayHit> *param2)
{
	double _loc3_ = 1;
	double _loc4_ = 0;
	for (int i = 0; i < param2->count(); i++)
	{
		const RayHit *rayHit = &param2->at(i);
		_loc4_ = _loc4_ + this->getPriorityForRayHit(param1, rayHit) * _loc3_;
		_loc3_ = _loc3_ * this->weakingCoef;
	}
	return _loc4_;
}


double TargetingPriorityCalculator::getPriorityForRayHit(double param1, const RayHit *rayHit)
{
	Body *body = rayHit->shape->body;
	if (rayHit->staticHit == true)
		return 0;

	return this->priorityEvalutor->getTargetPriority(body->tank, rayHit->t, param1);
}
