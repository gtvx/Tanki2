#include "HitInfo.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/tanks/models/weapon/AllGlobalGunParams.h"
#include "game/alternativa/tanks/models/weapons/targeting/TargetingResult.h"


void HitInfo::setResult(AllGlobalGunParams *allGlobalGunParams, TargetingResult *targetingResult)
{
	if (targetingResult->hasAnyHit())
	{
		this->setHit(&allGlobalGunParams->barrelOrigin, targetingResult->getDirection(), targetingResult->getSingleHit());
	}
	else
	{
		this->setMiss(targetingResult->getDirection());
	}
}


void HitInfo::setHit(const Vector3 *a, const Vector3 *b, const RayHit *rayHit)
{
	this->distance = rayHit->t;
	this->body = rayHit->shape->body;
	this->normal.copy(&rayHit->normal);
	this->direction.copy(b);
	this->position.copy(a);
	this->position.addScaled(rayHit->t, b);
}


void HitInfo::setMiss(const Vector3 *a)
{
	this->direction.copy(a);
	this->body = nullptr;
}
