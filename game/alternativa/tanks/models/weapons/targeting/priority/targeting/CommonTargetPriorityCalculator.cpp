#include "CommonTargetPriorityCalculator.h"
#include "game/alternativa/tanks/models/weapon/WeaponObject.h"
#include "game/alternativa/tanks/models/weapon/angles/verticals/autoaiming/VerticalAutoAiming.h"
#include "game/alternativa/tanks/models/weapon/weakening/DistanceWeakening.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "Error.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/models/weapon/shared/CommonTargetEvaluator.h"
#include "game/alternativa/tanks/models/battle/battlefield/CommonTargetEvaluatorConst.h"
#include "MyMath.h"

extern BattleService *battleService;


CommonTargetPriorityCalculator::CommonTargetPriorityCalculator(WeaponObject *weaponObject)
{
	this->commonTargetEvaluator = battleService->getCommonTargetEvaluator();
	this->fullDamageDistance = this->getFullDamageDistance(weaponObject);
	VerticalAutoAiming *_loc2_ = weaponObject->verticalAutoAiming();
	this->maxAngle = _loc2_->getMaxAngle();
}


double CommonTargetPriorityCalculator::getTargetPriority(Tank *, double distance, double _arg_3)
{
	//return this->commonTargetEvaluator->getTargetPriority(tank->getBody(), distance, _arg_3, this->fullDamageDistance, this->maxAngle);

	//if ((!(_local_6.isSameTeam(this.localTeamType)))))
	return CommonTargetEvaluatorConst::MAX_PRIORITY -
			(((CommonTargetEvaluatorConst::DISTANCE_WEIGHT * distance) / this->fullDamageDistance) +
			 (((1 - CommonTargetEvaluatorConst::DISTANCE_WEIGHT) * MyMath::abs(_arg_3)) / this->maxAngle));

}


double CommonTargetPriorityCalculator::getFullDamageDistance(WeaponObject *weaponObject)
{
	DistanceWeakening distanceWeakening;
	weaponObject->distanceWeakening(&distanceWeakening);
	return distanceWeakening.getFullDamageDistance();
}
