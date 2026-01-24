#include "CommonDirectionProcessor.h"
#include "game/alternativa/physics/collision/IRayCollisionFilter.h"
#include "game/alternativa/tanks/models/weapon/AllGlobalGunParams.h"
#include "game/alternativa/tanks/physics/CollisionGroup.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/tanks/models/weapon/shared/MarginalCollider.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "game/alternativa/tanks/physics/TanksCollisionDetector.h"

extern BattleService *battleService;

static const double COS_ALMOST_ZERO_ANGLE = 0.9999;


CommonDirectionProcessor::CommonDirectionProcessor(double maxDistance, IRayCollisionFilter *filter)
{
	this->maxDistance = maxDistance;
	this->filter = filter;
	this->shotFromMuzzle = false;
	this->collisionDetector = battleService->getBattleRunner()->getCollisionDetector();
}


void CommonDirectionProcessor::setShotFromMuzzle()
{
	this->shotFromMuzzle = true;
}


bool CommonDirectionProcessor::raycast(const Vector3 *_arg_1, const Vector3 *_arg_2, RayHit *_arg_3)
{
	if (this->collisionDetector->raycast(_arg_1, _arg_2, CollisionGroup::WEAPON, this->maxDistance, this->filter, _arg_3))
	{
		_arg_3->staticHit = ((!(BattleUtils::isTankBody(_arg_3->shape->body))) || (MarginalCollider::segmentWithStaticIntersection(_arg_1, &_arg_3->position)));
		return true;
	}

	return false;
}


Vector3* CommonDirectionProcessor::getOrigin(AllGlobalGunParams *allGlobalGunParams, Vector3 *vector3)
{
	if ((!(this->shotFromMuzzle)) || (vector3->dot(&allGlobalGunParams->direction) > COS_ALMOST_ZERO_ANGLE))
	{
		return &allGlobalGunParams->barrelOrigin;
	}

	return &allGlobalGunParams->muzzlePosition;
}

