#include "RotatingTurretModel.h"
#include "game/alternativa/tanks/models/weapon/common/WeaponCommonData.h"
#include "game/alternativa/tanks/models/weapon/common/IWeaponCommonModel.h"
#include "game/projects/tanks/client/battlefield/models/user/tank/commands/TurretControlType.h"
#include "game/projects/tanks/client/battlefield/models/user/tank/commands/TurretStateCommand.h"
#include "game/alternativa/tanks/battle/objects/tank/controllers/Turret.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/turret/RotatingTurretCC.h"
#include "../common/asWeaponCommon.h"
#include "Error.h"
#include <QDebug>

BattleService *RotatingTurretModel::battleService;


RotatingTurretModel::RotatingTurretModel()
{

}


void RotatingTurretModel::objectLoaded()
{
	this->saveState(getInitParam()->turretState);
}


std::shared_ptr<WeaponMount> RotatingTurretModel::createWeaponMount(IGameObject *object)
{
	(void)object;
	WeaponCommonData *weaponCommonData = asWeaponCommon(this->object())->getCommonData().get();
	std::shared_ptr<Turret> turret = std::make_shared<Turret>(weaponCommonData->getMaxTurretRotationSpeed(), weaponCommonData->getTurretRotationAcceleration());
	putData<Turret>(turret);
	this->setTurretState(turret.get(), this->getState());
	return turret;
}


void RotatingTurretModel::initTankPart(std::shared_ptr<Tank> tank)
{
	putData<Tank>(tank);

	if (BattleUtils::isLocalTank(tank->user))
	{
		std::shared_ptr<LocalTurretController> _local_2 = std::make_shared<LocalTurretController>(tank.get(), this->getTurret());
		putData<LocalTurretController>(_local_2);

		//TurretStateUpdater *_local_3 = new TurretStateUpdater(this.getTurret(), getFunctionWrapper(this.updateStateOnServer));
		//putData<TurretStateUpdater>(_local_3);
	}
}


LocalTurretController* RotatingTurretModel::getLocalTurretController()
{
	return getData<LocalTurretController>().get();
}


void RotatingTurretModel::setTurretState(Turret *turret, TurretStateCommand *turretStateCommand)
{
	if (turretStateCommand->controlType == TurretControlType::ROTATION_DIRECTION)
	{
		turret->setTurretPhysicsDirection(turretStateCommand->direction);
	}

	turret->setTurretControlState(turretStateCommand->controlType, turretStateCommand->controlInput, turretStateCommand->rotationSpeedNumber);
}


Turret* RotatingTurretModel::getTurret()
{
	auto result = getData<Turret>().get();

	if (result == nullptr)
	{
		qDebug("error RotatingTurretModel::getTurret()");
	}

	return result;
}


Tank* RotatingTurretModel::getTank()
{
	return getData<Tank>().get();
}


void RotatingTurretModel::saveState(std::shared_ptr<TurretStateCommand> command)
{
	putData<TurretStateCommand>(command);
}


TurretStateCommand* RotatingTurretModel::getState()
{
	return getData<TurretStateCommand>().get();
}
