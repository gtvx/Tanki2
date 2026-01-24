#ifndef SINGLETARGETDIRECTIONPROCESSOR_H
#define SINGLETARGETDIRECTIONPROCESSOR_H

#include "CommonDirectionProcessor.h"
#include "TargetingDirectionProcessor.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "game/alternativa/tanks/models/weapon/RayCollisionFilter.h"
#include <QVector>

class RayHit;
class RayCollisionFilter;
class IGameObject;

class SingleTargetDirectionProcessor :
		public CommonDirectionProcessor,
		public TargetingDirectionProcessor
{
	IGameObject *self;
	RayHit rayHitResult;
	RayCollisionFilter collisionFilter;
	QVector<RayHit> rayHits;

public:
	SingleTargetDirectionProcessor(IGameObject *self, double maxDistance);
	virtual QVector<RayHit>* process(AllGlobalGunParams*, Vector3*);
	virtual void setShotFromMuzzle();
};

#endif // SINGLETARGETDIRECTIONPROCESSOR_H
