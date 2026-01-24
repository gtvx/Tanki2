#include "TargetingResult.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/alternativa/physics/collision/CollisionShape.h"


TargetingResult::TargetingResult()
{
	staticHit_e = false;
	staticHit.clear();
}


bool TargetingResult::hasStaticHit()
{
	return !(this->staticHit_e == false);
}


RayHit* TargetingResult::getStaticHit()
{
	return &this->staticHit;
}


Vector3* TargetingResult::getDirection()
{
	return &this->direction;
}


QVector<RayHit>* TargetingResult::getHits()
{
	return &this->hits;
}


bool TargetingResult::hasTankHit()
{
	return this->hits.count() > 0;
}


bool TargetingResult::hasAnyHit()
{
	return (!(this->staticHit_e == false)) || (this->hits.count() > 0);
}


void TargetingResult::setData(Vector3 *direction, QVector<RayHit> *b)
{
	this->direction.copy(direction);

	this->hits = *b;

	this->staticHit_e = false;
	if (this->hits.count() > 0)
	{
		RayHit *rayHit = &this->hits.last();
		if (BattleUtils::isTankBody(rayHit->shape->body) == false)
		{
			this->staticHit_e = true;
			this->staticHit = this->hits.takeLast();
		}
	}
}


RayHit* TargetingResult::getSingleHit()
{

	if (this->staticHit_e != false)
	{
		return &this->staticHit;
	}
	return &this->hits[0];
}
