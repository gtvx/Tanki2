#include "TargetingSectorsCalculator.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/models/weapon/angles/verticals/autoaiming/VerticalAutoAiming.h"
#include "MyMath.h"
#include "game/alternativa/tanks/models/weapon/AllGlobalGunParams.h"
#include "game/alternativa/tanks/models/weapon/WeaponObject.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/services/tankregistry/TankUsersRegistry.h"
#include "global_class.h"
#include "Error.h"
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


TargetingSectorsCalculator::TargetingSectorsCalculator(IGameObject *self, WeaponObject *weapon, double maxDistance)
{
	this->upDirection = new Vector3();
	this->self = self;
	this->weapon = weapon;
	this->maxDistance = maxDistance;
}


TargetingSectorList* TargetingSectorsCalculator::getSectors(AllGlobalGunParams *allGlobalGunParams)
{
	this->upDirection->cross2(&allGlobalGunParams->elevationAxis, &allGlobalGunParams->direction);

	MyList<IGameObject*> *users = getTankUsersRegistry()->getUsers();

	targetingSectorList.clear();

	Tank *selfTank = getTank(this->self);

	const int count = users->count();

	for (int i = 0; i < count; i++)
	{
		Tank *tank = getTank(users->at(i));
		if (tank == selfTank)
			continue;

		this->calculateSectorAndAdd(allGlobalGunParams, this->upDirection, tank, &targetingSectorList);
	}

	return &targetingSectorList;
}


void TargetingSectorsCalculator::calculateSectorAndAdd(AllGlobalGunParams *_arg_1, Vector3 *_arg_2, Tank *tank, TargetingSectorList *sector)
{
	Vector3 _local_5;
	_local_5.diff(&tank->getBody()->state.position, &_arg_1->barrelOrigin);
	double _local_6 = _local_5.length();
	double distance = MyMath::max(0, (_local_5.length() - tank->getBoundSphereRadius()));
	if (distance > this->maxDistance)
		return;

	const Vector3 *_local_8 = &_arg_1->elevationAxis;
	const Vector3* _local_9 = &_arg_1->direction;
	double _local_10 = _local_5.dot(_local_8);
	double _local_11 = _local_5.dot(_local_9);
	double _local_12 = _local_5.dot(_arg_2);
	double _local_13 = MyMath::atan2(_local_10, _local_11);
	double _local_14 = MyMath::atan2(_local_12, _local_11);
	double _local_15 = MyMath::min(tank->getBoundSphereRadius(), _local_6);
	double _local_16 = MyMath::asin(_local_15 / _local_6);
	double _local_17 = _local_16;
	double _local_18 = _local_16;
	if (MyMath::abs(_local_13) > _local_17)
		return;

	VerticalAutoAiming *_local_19 = this->weapon->verticalAutoAiming();
	double left = MyMath::max(_local_14 - _local_18, -(_local_19->getElevationAngleDown()));
	double right = MyMath::min(_local_14 + _local_18, _local_19->getElevationAngleUp());
	if (left < right)
	{
		TargetingSector *t = sector->add();
		t->set(left, right, distance, tank);
	}
}
