#include "WeaponCommonModel.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/common/WeaponCommonCC.h"
#include "getTimer.h"
#include "../WeaponConst.h"
#include "game/alternativa/tanks/utils/ConstEncryptedNumber.h"
#include "WeaponCommonData.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"


WeaponCommonModel::WeaponCommonModel()
{

}


void WeaponCommonModel::objectLoaded()
{
	putData<bool>(getInitParam()->buffed);
	putData<int>(0);

	WeaponCommonCC *cc = getInitParam();

	auto weaponCommonData = std::make_shared<WeaponCommonData>(cc->turretRotationSpeed,
									cc->turretRotationAcceleration,
									cc->impactForce * WeaponConst::BASE_IMPACT_FORCE->getNumber(),
									cc->kickback * WeaponConst::BASE_IMPACT_FORCE->getNumber());

	putData<WeaponCommonData>(weaponCommonData);
}


void WeaponCommonModel::setBuffed(bool _arg_1, double _arg_2)
{
	putData<bool>(_arg_1);

	double _local_3 = _arg_2 * WeaponConst::BASE_IMPACT_FORCE->getNumber();

	WeaponCommonData *weaponCommonData = getData<WeaponCommonData>().get();

	weaponCommonData->setRecoilForce(_local_3);

#if 0
	WeaponBuffListener(object.event(WeaponBuffListener)).weaponBuffStateChanged(this.getTank().user, _arg_1, _local_3);

	if (!_arg_1 && (getInitParam()->buffShotCooldownMs > 0))
	{
		this->getWeaponController()->lockWeapon(TankControlLockBits.DEBUFF, false);
		putData<int>(getTimer() + getInitParam().buffShotCooldownMs);
	}
#endif
}


void WeaponCommonModel::storeTank(std::shared_ptr<Tank> tank)
{
	putData<Tank>(tank);

	//if (((getInitParam().buffShotCooldownMs > 0) && (ITankModel(_arg_1.user.adapt(ITankModel)).isLocal())))
	//{
	//	display.stage.addEventListener(Event.ENTER_FRAME, getFunctionWrapper(this.onEnterFrame));
	//}
}


Tank* WeaponCommonModel::getTank()
{
	return getData<Tank>().get();
}


void WeaponCommonModel::getGunParams(AllGlobalGunParams *allGunParams, int index)
{
	this->getTank()->getAllGunParams(allGunParams, index);
}


std::shared_ptr<WeaponCommonData> WeaponCommonModel::getCommonData()
{
	std::shared_ptr<WeaponCommonData> weaponCommonData = getData<WeaponCommonData>();
	return weaponCommonData;
}



#if 0

public function objectLoaded():void
{
	putData(Boolean, getInitParam().buffed);
	putData(int, 0);
}

public function objectUnloaded():void
{
	display.stage.removeEventListener(Event.ENTER_FRAME, getFunctionWrapper(this.onEnterFrame));
}

public function setBuffed(_arg_1:Boolean, _arg_2:Number):void
{
	putData(Boolean, _arg_1);
	var _local_3:Number = (_arg_2 * WeaponConst.BASE_IMPACT_FORCE.getNumber());
	WeaponCommonData(getData(WeaponCommonData)).setRecoilForce(_local_3);
	WeaponBuffListener(object.event(WeaponBuffListener)).weaponBuffStateChanged(this.getTank().user, _arg_1, _local_3);
	if (((!(_arg_1)) && (getInitParam().buffShotCooldownMs > 0)))
	{
		this.getWeaponController().lockWeapon(TankControlLockBits.DEBUFF, false);
		putData(int, (getTimer() + getInitParam().buffShotCooldownMs));
	};
}

private function getWeaponController():IWeaponController
{
	var _local_1:ITankModel = ITankModel(this.getTank().user.adapt(ITankModel));
	_local_1.getWeaponController();
	return (IWeaponController(_local_1.getWeaponController()));
}

public function getCommonData():WeaponCommonData
{
	var _local_2:WeaponCommonCC;
	var _local_1:WeaponCommonData = WeaponCommonData(getData(WeaponCommonData));
	if (_local_1 == null)
	{
		_local_2 = getInitParam();
		_local_1 = new WeaponCommonData(_local_2.turretRotationSpeed, _local_2.turretRotationAcceleration, (_local_2.impactForce * WeaponConst.BASE_IMPACT_FORCE.getNumber()), (_local_2.kickback * WeaponConst.BASE_IMPACT_FORCE.getNumber()));
		putData(WeaponCommonData, _local_1);
	};
	return (_local_1);
}

public function getTurretRotationSound():SoundResource
{
	return (getInitParam().turretRotationSound);
}

public function storeTank(_arg_1:Tank):void
{
	putData(Tank, _arg_1);
	if (((getInitParam().buffShotCooldownMs > 0) && (ITankModel(_arg_1.user.adapt(ITankModel)).isLocal())))
	{
		display.stage.addEventListener(Event.ENTER_FRAME, getFunctionWrapper(this.onEnterFrame));
	};
}

public function getTank():Tank
{
	return (Tank(getData(Tank)));
}

public function getGunParams(_arg_1:int=0):AllGlobalGunParams
{
	this.getTank().getAllGunParams(allGunParams, _arg_1);
	return (allGunParams);
}

private function onEnterFrame(_arg_1:Event):void
{
	var _local_2:int = int(getData(int));
	if (((!(_local_2 == 0)) && (_local_2 < getTimer())))
	{
		this.getWeaponController().unlockWeapon(TankControlLockBits.DEBUFF);
		putData(int, 0);
	};
}

#endif
