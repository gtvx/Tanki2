#include "BattleUtils.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/utils/EncryptedIntImpl.h"
#include "game/projects/tanks/client/battlefield/types/Vector3d.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/models/weapon/AllGlobalGunParams.h"
#include "game/alternativa/physics/collision/CollisionDetector.h"
#include "game/alternativa/tanks/physics/CollisionGroup.h"
#include "game/alternativa/physics/PhysicsScene.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "game/alternativa/tanks/models/tank/ITankModel.h"
#include "_global.h"
#include "MyMath.h"
#include "flash/geom/Vector3D.h"


static const EncryptedIntImpl hundred(100);


bool BattleUtils::isTankBody(Body *body)
{
	return body != nullptr && body->tank != nullptr;
}


double BattleUtils::toClientScale(double value)
{
	return value * hundred.getInt();
}


bool BattleUtils::isFiniteVector3d(const Vector3d *vector3d)
{
	return vector3d != nullptr && isFinite(vector3d->getX()) && isFinite(vector3d->getY()) && isFinite(vector3d->getZ());
}


void BattleUtils::copyToVector3(const Vector3d *vector3d, Vector3 *vector3)
{
	vector3->setX(vector3d->getX());
	vector3->setY(vector3d->getY());
	vector3->setZ(vector3d->getZ());
}


void BattleUtils::copyToVector3d(const Vector3 *vector3, Vector3d *vector3d)
{
	vector3d->setX(vector3->getX());
	vector3d->setY(vector3->getY());
	vector3d->setZ(vector3->getZ());
}


void BattleUtils::copyToVector3D(const Vector3 *vector3, Vector3D *vector3D)
{
	vector3D->setX(vector3->getX());
	vector3D->setY(vector3->getY());
	vector3D->setZ(vector3->getZ());
	vector3D->setW(0);
}


double BattleUtils::getDirectionAngle(const Vector3 *param1)
{
	return MyMath::atan2(-param1->getX(), param1->getY());
}


void BattleUtils::fillDirectionVector(Vector3 *param1, double param2)
{
	param1->setX(-MyMath::sin(param2));
	param1->setY(MyMath::cos(param2));
	param1->setZ(0);
}


bool BattleUtils::isLocalTank(IGameObject *object)
{
	return object->adapt<ITankModel>()->isLocal();
}


bool BattleUtils::isTurretAboveGround(Body *body, AllGlobalGunParams *allGlobalGunParams)
{
	Vector3 *v = &body->state.position;
	RayHit _rayHit;
	Vector3 _direction;
	_direction.diff(&allGlobalGunParams->barrelOrigin, v);

	if (body->scene != nullptr)
	{
		CollisionDetector *collisionDetector = body->scene->collisionDetector;
		return !(collisionDetector->raycastStatic(v, &_direction, CollisionGroup::STATIC, 1, nullptr, &_rayHit));
	}

	return false;
}


void BattleUtils::globalToLocal(Body *body, Vector3 *v)
{
	v->subtract(&body->state.position);
	v->transformTransposed3(&body->baseMatrix);
}
