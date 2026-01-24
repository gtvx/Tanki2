#include "SpeedCharacteristicsModel.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/alternativa/tanks/battle/objects/tank/WeaponMount.h"
#include "game/alternativa/tanks/models/tank/ITankModel.h"
#include "game/projects/tanks/client/battlefield/models/user/speedcharacteristics/SpeedCharacteristicsCC.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"



struct CurrentSpeedValues
{
	double speed;
	double turnSpeed;
	double turretRotationSpeed;
	double acceleration;
} currentspeedvalues;


SpeedCharacteristicsModel::SpeedCharacteristicsModel()
{

}


void SpeedCharacteristicsModel::setInitialTankState()
{
	ITankModel *tankModel = object()->adapt<ITankModel>().get();
	Tank *tank = tankModel->getTank();

	SpeedCharacteristicsCC *cc = getInitParam();

	tank->setReverseAcceleration(BattleUtils::toClientScale(cc->_reverseAcceleration));
	tank->setSideAcceleration(BattleUtils::toClientScale(cc->_sideAcceleration));
	tank->setTurnAcceleration(cc->_turnAcceleration);
	tank->setReverseTurnAcceleration(cc->_reverseTurnAcceleration);
	tank->setStabilizationAcceleration(cc->_turnStabilizationAcceleration);

	this->setTankSpec(tank,
					  cc->_currentSpeed,
					  cc->_currentTurnSpeed,
					  cc->_currentTurretRotationSpeed,
					  cc->_currentAcceleration,
					  true);
}


double SpeedCharacteristicsModel::getMaxHullTurnSpeed()
{
	return this->getCurrentValues()->turnSpeed;
}


double SpeedCharacteristicsModel::getMaxTurretTurnSpeed()
{
	return this->getCurrentValues()->turretRotationSpeed;
}


double SpeedCharacteristicsModel::getTurretRotationCoefficient()
{
	double baseTurretRotationSpeed = getInitParam()->_baseTurretRotationSpeed;

	if (baseTurretRotationSpeed == 0)
		return 0;

	return this->getCurrentValues()->turretRotationSpeed / baseTurretRotationSpeed;
}


CurrentSpeedValues* SpeedCharacteristicsModel::getCurrentValues()
{
	return &currentspeedvalues;
}


void SpeedCharacteristicsModel::setTankSpec(Tank *tank,
											double speed,
											double turnSpeed,
											double turretRotationSpeed,
											double acceleration,
											bool b)
{
	currentspeedvalues.speed = speed;
	currentspeedvalues.turnSpeed = turnSpeed;
	currentspeedvalues.turretRotationSpeed = turretRotationSpeed;
	currentspeedvalues.acceleration = acceleration;
	tank->setMaxSpeed(BattleUtils::toClientScale(speed), b);
	tank->setMaxTurnSpeed(turnSpeed, b);
	tank->setAcceleration(BattleUtils::toClientScale(acceleration));
	tank->getWeaponMount()->setMaxTurnSpeed(turretRotationSpeed, b);
}
