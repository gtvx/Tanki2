#include "TwinsModel.h"
#include "TwinsWeapon.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/twins/TwinsCC.h"
#include "entrance/platform/client/fp10/core/type/impl/GameObject.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "RemoteTwinsWeapon.h"


TwinsModel::TwinsModel()
{
	//this.battleEventSupport = new BattleEventSupport(battleEventDispatcher);
	//this.battleEventSupport.addEventHandler(TankUnloadedEvent, this.onTankUnloaded);
	//this.battleEventSupport.activateHandlers();
}


void TwinsModel::objectLoaded()
{
	TwinsCC *cc = getInitParam();
	cc->speed = BattleUtils::toClientScale(cc->speed);
	cc->shellRadius = BattleUtils::toClientScale(cc->shellRadius);
}


Weapon* TwinsModel::createLocalWeapon(IGameObject *user)
{
	Weapon *weapon = new TwinsWeapon(user, this->object(), getInitParam());
	this->weapons[user] = weapon;
	return weapon;
}


Weapon* TwinsModel::createRemoteWeapon(IGameObject *user)
{
	Weapon *weapon = new RemoteTwinsWeapon(this->object(), getInitParam());
	this->weapons[user] = weapon;
	return weapon;
}


void TwinsModel::fire(IGameObject *, int, int, const Vector3d *)
{

}


void TwinsModel::fireDummy(IGameObject *, int)
{

}


void TwinsModel::onShot(int time, int shotId, int currentBarrel, const Vector3 *shotDirection)
{
	(void)time;
	(void)shotId;
	(void)currentBarrel;
	(void)shotDirection;

	/*
	if (battleService.isBattleActive())
	{
		this->battleEventSupport->dispatchEvent(StateCorrectionEvent.MANDATORY_UPDATE);
		server->fireCommand(_arg_1, _arg_3, _arg_2, BattleUtils.getVector3d(_arg_4));
	}
	*/
}


void TwinsModel::onDummyShot(int time, int currentBarrel)
{
	(void)time;
	(void)currentBarrel;
}


void TwinsModel::onTargetHit(int shotId, Body *targetBody, const Vector3 *hitPoint)
{
	(void)shotId;
	(void)targetBody;
	(void)hitPoint;
}


void TwinsModel::onStaticHit(int shotId, const Vector3 *position)
{
	(void)shotId;
	(void)position;
}

