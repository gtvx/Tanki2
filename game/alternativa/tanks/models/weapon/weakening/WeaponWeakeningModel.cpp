#include "WeaponWeakeningModel.h"
#include "WeaponWeakeningData.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/weakening/WeaponWeakeningCC.h"
#include "DistanceWeakening.h"
#include "_global.h"


void WeaponWeakeningModel::objectLoaded()
{
	WeaponWeakeningCC *cc = getInitParam();
	WeaponWeakeningData *data = new WeaponWeakeningData();

	data->maximumDamageRadius = (isNaN(cc->maximumDamageRadius)) ? 0 : BattleUtils::toClientScale(cc->maximumDamageRadius);
	data->minimumDamageRadius = (isNaN(cc->minimumDamageRadius)) ? 1 : BattleUtils::toClientScale(cc->minimumDamageRadius);
	data->minimumDamagePercent = (isNaN(cc->minimumDamagePercent)) ? 0 : cc->minimumDamagePercent;

	if (data->minimumDamagePercent > 100)
	{
		data->minimumDamagePercent = 100;
	}

	putData<WeaponWeakeningData>(std::shared_ptr<WeaponWeakeningData>(data));
}


void WeaponWeakeningModel::getDistanceWeakening(DistanceWeakening *distanceWeakening)
{
	WeaponWeakeningData *data = getData<WeaponWeakeningData>().get();
	return distanceWeakening->init(data->maximumDamageRadius, data->minimumDamageRadius, data->minimumDamagePercent);
}


#if 0
public function reconfigureWeapon(_arg_1:Number, _arg_2:Number):void
{
	var _local_3:WeaponWeakeningData = WeaponWeakeningData(getData(WeaponWeakeningData));
	_local_3.maximumDamageRadius = ((isNaN(_arg_1)) ? 0 : BattleUtils.toClientScale(_arg_1));
	_local_3.minimumDamageRadius = ((isNaN(_arg_2)) ? 1 : BattleUtils.toClientScale(_arg_2));
	var _local_4:IGameObject = IWeaponCommonModel(object.adapt(IWeaponCommonModel)).getTank().user;
	StreamWeaponReconfiguredListener(object.event(StreamWeaponReconfiguredListener)).streamWeaponDistanceChanged(_local_4, _local_3.minimumDamageRadius);
}
#endif
