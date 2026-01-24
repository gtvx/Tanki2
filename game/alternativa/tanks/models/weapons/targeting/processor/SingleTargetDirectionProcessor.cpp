#include "SingleTargetDirectionProcessor.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "Error.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/models/weapon/RayCollisionFilter.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "game/alternativa/tanks/models/tank/ITankModel.h"


static Tank* getTank(IGameObject *object)
{
	ITankModel *model = object->adapt<ITankModel>().get();

	if (model == nullptr)
	{
		throw Error("getTank");
	}

	return model->getTank();
}


SingleTargetDirectionProcessor::SingleTargetDirectionProcessor(IGameObject *self, double maxDistance) :
	CommonDirectionProcessor(maxDistance, &collisionFilter),
	collisionFilter(nullptr)

{
	this->self = self;
	collisionFilter = nullptr;
}


QVector<RayHit>* SingleTargetDirectionProcessor::process(AllGlobalGunParams *allGlobalGunParams, Vector3 *vector)
{
	//this->collisionFilter->exclusion = getTank(this->self)->getBody();
	this->collisionFilter.setBody(getTank(this->self)->getBody());

	this->rayHits.clear();;
	this->rayHitResult.clear();

	if (raycast(getOrigin(allGlobalGunParams, vector), vector, &this->rayHitResult))
	{
		this->rayHits.append(this->rayHitResult);
	}

	return &this->rayHits;
}


void SingleTargetDirectionProcessor::setShotFromMuzzle()
{
	CommonDirectionProcessor::setShotFromMuzzle();
}
