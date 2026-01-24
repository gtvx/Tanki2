#include "game/alternativa/tanks/models/tank/TankModel.h"
#include "game/alternativa/tanks/models/tank/speedcharacteristics/SpeedCharacteristics.h"
#include "game/alternativa/tanks/battle/objects/tank/WeaponMount.h"
#include "game/alternativa/tanks/models/tank/hullcommon/HullCommon.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankSkin.h"
#include "game/alternativa/tanks/models/tank/configuration/TankConfiguration.h"
#include "game/alternativa/tanks/battle/objects/tank/controllers/ChassisController.h"
#include "game/alternativa/tanks/battle/objects/tank/controllers/LocalChassisController.h"
#include "game/alternativa/tanks/battle/objects/tank/TankControlLockBits.h"
#include "game/projects/tanks/client/battlefield/models/user/tank/TankCC.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "game/alternativa/tanks/models/weapon/common/IWeaponCommonModel.h"
#include "game/alternativa/tanks/models/weapon/IWeaponModel.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/projects/tanks/client/battlefield/types/TankState.h"
#include "game/projects/tanks/client/battlefield/models/user/tank/TankLogicState.h"
#include "game/projects/tanks/client/battlefield/types/Vector3d.h"
#include "entrance/alternativa/types/Long.h"
#include "game/alternativa/tanks/battle/objects/tank/LocalHullTransformUpdater.h"
#include "game/alternativa/tanks/battle/objects/tank/RemoteHullTransformUpdater.h"
#include "game/alternativa/tanks/physics/CollisionGroup.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "game/alternativa/tanks/models/weapon/turret/RotatingTurretModel.h"
#include "WeaponMountProvider.h"
#include "LocalWeaponController.h"
#include "InitTankPart.h"
#include "IWeaponController.h"
#include <QMap>
#include "_global.h"
#include "game/alternativa/tanks/models/tank/RemoteWeaponController.h"
#include "game/alternativa/tanks/services/tankregistry/TankUsersRegistry.h"
#include "global_class.h"
#include "Error.h"
#include <QDebug>
#include "entrance/platform/client/fp10/core/type/impl/GameObject.h"
#include "entrance/alternativa/types/LongHash.h"
#include "../weapon/turret/RotatingTurretModel.h"
#include "game/alternativa/tanks/models/tank/InitTankPart.h"


struct TankSoundEffects;
static const Vector3d ZERO_VECTOR_3D(0, 0, 0);
extern BattleService *battleService;



static int getDefaultMaxHealth()
{
	/*
	TankConfiguration *_loc1_ = TankConfiguration(object.adapt(TankConfiguration));
	IGameObject *_loc2_ = _loc1_.getHullObject();
	Armor * _loc3_ = Armor(_loc2_.adapt(Armor));
	return _loc3_.getMaxHealth();
	*/
	return 1000;
}


TankModel::TankModel()
{
	localObject = nullptr;
	chassisStateCorrectionTask = nullptr;
}


std::shared_ptr<Tank> TankModel::createTank(IGameObject *param1,
							IGameObject *param2,
							IGameObject *param3,
							WeaponMount *param4,
							Weapon *weapon,
							UserTitle *param6)
{
	if (param1 == nullptr)
	{
		qDebug() << "error TankModel::createTank param1";
	}

	if (param2 == nullptr)
	{
		qDebug() << "error TankModel::createTank param2";
	}

	if (param3 == nullptr)
	{
		//qDebug() << "error TankModel::createTank param3";
	}

	if (param4 == nullptr)
	{
		qDebug() << "error TankModel::createTank param4";
	}


	localObject = nullptr;

	std::shared_ptr<HullCommon> _loc7_ = param1->adapt<HullCommon>();

	//ITrackedChassis *_loc8_ = ITrackedChassis(param1.adapt(ITrackedChassis));
	//Armor *_loc9_ = Armor(param1.adapt(Armor));
	TankSkin *skin = new TankSkin(param1, param2, param3);
	TankSoundEffects *_loc11_ = nullptr; //createTankSoundEffects(param1, param2);
	double mass = _loc7_->getMass();

	return std::make_shared<Tank>(object(),
								  mass,
								  1000,//_loc8_->getDamping()
								  _loc11_,
								  skin,
								  param4,
								  weapon,
								  param6,
								  nullptr, //battleEventDispatcher
								  1000);
}


std::shared_ptr<Tank> TankModel::initTank(IGameObject *param1,
										  IGameObject *param2,
										  IGameObject *param3,
										  int param4,
										  bool param5)
{


	if (param1 == nullptr)
	{
		qDebug() << "error TankModel::initTank param1";
	}

	if (param2 == nullptr)
	{
		qDebug() << "error TankModel::initTank param2";
	}

	if (param3 == nullptr)
	{
		//qDebug() << "error TankModel::initTank param3";
	}



	TankConfiguration *tankConfiguration = object()->adapt<TankConfiguration>().get();


	WeaponMount *_loc7_ = this->createWeaponMount(param2);
	//int _loc8_ = 0;
	UserTitle *_loc9_ = nullptr;//getData(UserTitle) as UserTitle;
	/*
	if (_loc9_ != nullptr)
	{
		_loc8_ = _loc9_.getResistance();
		_loc9_.removeFromContainer();
	}
	_loc9_ = this->createUserTitle(param4);
	_loc9_.setResistance(_loc8_);
	//putData(UserTitle,_loc9_);
	*/


	Weapon *weapon = this->createWeapon(param2);

	if (weapon == nullptr)
	{
		throw Error("error createWeapon");
	}

	this->registerWeaponController(weapon);

	//param1->adapt<HullCommon>()->setTankObject(nullptr);

	auto tank = this->createTank(param1, param2, tankConfiguration->getColoringObject(), _loc7_, weapon, _loc9_);
	this->storeTank(tank, param1, param2, param3, param4);
	this->initTankPart(param2, tank);


	//tank->setLocalTurretController(param2->adapt<RotatingTurretModel>()->getLocalTurretController());

	if (param5)
	{
		//TankPartReset(param2.event(TankPartReset)).resetTankPart(tank);
	}

	//this->createTankDataValidator(tank);
	//this->createSmokeEffect(param1);
	this->createChassisController(tank.get());
	//this->createTankKillHandler();

	if (this->isLocal())
	{
		//this->initLocalObjectBattleEventListeners();
		this->initLocalTank(tank.get(), param5);
		LocalHullTransformUpdater *l = new LocalHullTransformUpdater(tank.get());
		tank->setHullTransformUpdater(l);
	}
	else
	{
		tank->setHullTransformUpdater(new RemoteHullTransformUpdater(tank.get()));
		//this->createMovementAnticheatTask();
	}

	setInitialSpeedCharacteristics();
	//this->initUltimate();
	return tank;
}


void TankModel::setInitialSpeedCharacteristics()
{
	object()->adapt<SpeedCharacteristics>()->setInitialTankState();
}


bool TankModel::isLocal()
{
	return getInitParam()->local;
}


void TankModel::createChassisController(Tank *tank)
{
	std::shared_ptr<ChassisController> _local_2;
	std::shared_ptr<LocalChassisController> _local_3;

	if (getInitParam()->local)
	{
		_local_3 = std::make_shared<LocalChassisController>(tank, this);
		_local_3->lock(TankControlLockBits::DEAD);
		//setBackTurnMode(_local_3);
		_local_2 = _local_3;
		//putData<TankSettingsSupport>(new TankSettingsSupport(_local_3));
		putData<LocalChassisController>(_local_3);
	}
	else
	{
		_local_2 = std::make_shared<ChassisController>(tank, this);
		_local_2->lock(TankControlLockBits::DEAD);
	}

	putData<ChassisController>(_local_2);
}


WeaponMount* TankModel::createWeaponMount(IGameObject *param1)
{
	WeaponMountProvider *weaponMountProvider = param1->adapt<WeaponMountProvider>().get();
	std::shared_ptr<WeaponMount> weaponMount = weaponMountProvider->createWeaponMount(this->object());
	putData<WeaponMount>(weaponMount);
	return weaponMount.get();
}


void TankModel::registerWeaponController(Weapon *weapon)
{
	if (getInitParam()->local)
	{
		putData<IWeaponController>(std::make_shared<LocalWeaponController>(weapon));
	}
	else
	{
		putData<IWeaponController>(std::make_shared<RemoteWeaponController>(weapon));
	}
}


void TankModel::onChassisControlChanged(int , bool )
{
	//if (param2)
		//this->chassisStateCorrectionTask.controlChanged();

	//this->smokeEffect.controlChanged(object != null?object:this->localObject,param1);
}


void TankModel::initTankPart(IGameObject *o, std::shared_ptr<Tank> tank)
{
	o->adapt<InitTankPart>()->initTankPart(tank);
	//InitTankPart(o.event(InitTankPart)).initTankPart(_arg_2);
}


void TankModel::storeTank(std::shared_ptr<Tank> tank, IGameObject *, IGameObject *_arg_3, IGameObject *, int)
{
	putData<Tank>(tank);
	(void)_arg_3;
	//this->getWeaponCommon(_arg_3)->storeTank(tank.get());
	//putData(TankSet, new TankSet(_arg_2, _arg_3, _arg_4, _arg_5));
}


IWeaponCommonModel* TankModel::getWeaponCommon(IGameObject *param1)
{
	return param1->adapt<IWeaponCommonModel>().get();
}


Weapon* TankModel::createWeapon(IGameObject *o)
{
	IWeaponModel *weaponModel = o->adapt<IWeaponModel>().get();

	if (weaponModel == nullptr)
	{
		throw Error("TankModel::createWeapon");
	}

	if (this->isLocal())
	{
		return weaponModel->createLocalWeapon(this->object());
	}
	else
	{
		return weaponModel->createRemoteWeapon(this->object());
	}
}


void TankModel::objectLoaded(IGameObject *o)
{
	this->registerUser();
	//putData(UserInfo,new UserInfo(battleUserInfoService.getUserName(object.id),
	//battleUserInfoService.getUserRank(object.id),
	//getInitParam().local,battleUserInfoService.getChatModeratorLevel(object.id),
	//battleUserInfoService.hasUserPremium(object.id)));

	TankConfiguration *tankconfiguration = o->adapt<TankConfiguration>().get();

	if (tankconfiguration == nullptr)
	{
		throw Error("error TankModel::objectLoaded");
	}

	int MaxHealth = getDefaultMaxHealth();

	IGameObject *object_drone = nullptr; //tankconfiguration->hasDrone() ? tankconfiguration->getDrone() : nullptr;

	this->initTank(tankconfiguration->getHullObject(),
				   tankconfiguration->getWeaponObject(),
				   object_drone,
				   MaxHealth,
				   false);

	//this->showLoginMessage();
	//this->battleEventSupport.dispatchEvent(new TankLoadedEvent(this->getTank(), this->isLocal()));
	//TankEntityCreationListener *_loc4_ = TankEntityCreationListener(object.event(TankEntityCreationListener));
	//TankEntityCreationListener *_loc5_ = TankEntityCreationListener(this->getTankSet().hull.event(TankEntityCreationListener));
	//_loc4_.onTankEntityCreated(this->getTank(),this->isLocal(),getInitParam()->logicState);
	//_loc5_.onTankEntityCreated(this->getTank(),this->isLocal(),getInitParam()->logicState);
	//TankEntityCreationListener *_loc6_ = TankEntityCreationListener(battleService.getBattle().event(TankEntityCreationListener));
	//_loc6_.onTankEntityCreated(this->getTank(), this->isLocal(), getInitParam()->logicState);

	if (this->tankShouldBeAddedToBattle())
	{
		this->addExistingTankToBattle();
	}
}


Tank* TankModel::getTank()
{
	return getData<Tank>().get();
}


void TankModel::addExistingTankToBattle()
{
	Tank *tank = this->getTank();
	//ITankSpawner *_loc2_ = ITankSpawner(object.adapt(ITankSpawner));
	//_loc2_.getIncarnationId()

	tank->spawn(getInitParam()->team, 1);

	this->doSetHealth(getInitParam()->health);

	if (getInitParam()->health <= 0)
	{
		//tank->getSkin().setDeadState();
		//this->getTitle().hide();
		//this->lockMovementControl(TankControlLockBits::DEAD);
	}
	else
	{
		this->unlockMovementControl(TankControlLockBits::ALL);
		//this->getTitle().show();
		//this->configureTankTitleAsRemote(object);
		this->getWeaponController()->activateWeapon(); //Weapon
		this->getWeaponController()->unlockWeapon(TankControlLockBits::ALL); //Weapon
	}

	TankState *_loc3_ = getInitParam()->tankState;

	this->setChassisState(_loc3_->get_position(),
						  _loc3_->get_orientation(),
						  &ZERO_VECTOR_3D,
						  &ZERO_VECTOR_3D,
						  _loc3_->get_chassisControl(),
						  _loc3_->get_chassisTurnSpeedNumber());

	if (getInitParam()->logicState == TankLogicState::ACTIVATING) {
		//tank->setSemiActivatedState();
	} else if (getInitParam()->logicState == TankLogicState::ACTIVE) {
		tank->setActivatedState();
	}

	this->addTankToBattle();
}


void TankModel::addTankToBattle()
{
	Tank *tank = this->getTank();
	this->tanksInBattle[object()->id()] = tank;
	tank->addToBattle(battleService);
	//this->asAddToBattleListener(object()).onAddToBattle();
	//this->asAddToBattleListener(this->getTankSet().turret).onAddToBattle();
	//this->battleEventSupport.dispatchEvent(new TankAddedToBattleEvent(tank,this->isLocal()));
	if (this->isLocal())
		this->getLocalChassisController()->enable();

}


void TankModel::registerUser()
{
	getTankUsersRegistry()->addUser(object());
	//if (tankUsersRegistry.getUserCount() == 1)
		//battleUserInfoService.addBattleUserInfoListener(this.tankUserInfoUpdater);
}


void TankModel::unregisterUser()
{
	getTankUsersRegistry()->removeUser(object());
	//if (tankUsersRegistry->getUserCount() == 0)
		//battleUserInfoService.removeBattleUserInfoListener(this.tankUserInfoUpdater);
}


void TankModel::objectUnloaded()
{
	this->unregisterUser();
	this->destroyTank(false);
}


void TankModel::destroyTank(bool s)
{
	Tank *tank = this->getTank();
	//this->removeMovementAnticheatTask();
	this->removeTankFromBattle();
	//this->removeTankFromExclusionSet(tank);
	//this->dispatchTankUnloadedEvent();
	this->unloadLocalTank(s);
	//this->sendDestroyPartEvent(this->getTankSet().turret);
	tank->destroy();
}


void TankModel::unloadLocalTank(bool)
{
	if (this->isLocal())
	{
		//LocalTankUnloadListener(object.event(LocalTankUnloadListener)).localTankUnloaded(_arg_1);
		//_local_2 = TankConfiguration(object.adapt(TankConfiguration));
		//LocalTankUnloadListener(_local_2.getWeaponObject().event(LocalTankUnloadListener)).localTankUnloaded(_arg_1);
		battleService->setFollowCameraTarget(nullptr);
		//this->localObject = nullptr;
		//this.chassisStateCorrectionTask = null;
		//RegularUserTitleRenderer(getData(RegularUserTitleRenderer)).close();
		//StateCorrectionSupport(getData(StateCorrectionSupport)).close();
		//MainLoopExecutionErrorHandler(getData(MainLoopExecutionErrorHandler)).close();
		//ClientDeactivationSupport(getData(ClientDeactivationSupport)).close();
		//clearData(RegularUserTitleRenderer);
		//clearData(StateCorrectionSupport);
		//clearData(MainLoopExecutionErrorHandler);
		//clearData(ClientDeactivationSupport);
	}
}


bool TankModel::tankShouldBeAddedToBattle()
{
	return getInitParam()->tankState != nullptr && getInitParam()->health > 0;
}


void TankModel::lockMovementControl(int c)
{
	this->getChassisController()->lock(c);
	//this->getWeaponMount()->lock(c);
}


void TankModel::unlockMovementControl(int c)
{
	this->getChassisController()->unlock(c);
	//this->getWeaponMount()->unlock(c);
}


IWeaponController* TankModel::getWeaponController()
{
	return getData<IWeaponController>().get();
}


void TankModel::setChassisState(const Vector3d *param1, const Vector3d *param2, const Vector3d *param3, const Vector3d *param4, int param5, int param6)
{
	Tank *tank = this->getTank();
	if (tank != nullptr)
	{
		if (BattleUtils::isFiniteVector3d(param1) && BattleUtils::isFiniteVector3d(param2) &&
		   BattleUtils::isFiniteVector3d(param3) && BattleUtils::isFiniteVector3d(param4))
		{
			tank->setPhysicsState(param1, param2, param3, param4);
		}
		this->getChassisController()->setControlState(param5, param6);
	}
}


ChassisController* TankModel::getChassisController()
{
	return getData<ChassisController>().get();
}


LocalChassisController* TankModel::getLocalChassisController()
{
	return getData<LocalChassisController>().get();
}


Tank* TankModel::getLocalTank()
{
	auto object = this->getLocalTankObject();

	ITankModel *model = object->adapt<ITankModel>().get();

	return model->getTank();
}


IGameObject* TankModel::getLocalTankObjectOrNull()
{
	return this->localObject;
}


IGameObject* TankModel::getLocalTankObject()
{
	if (!this->isLocalTankLoaded())
	{
		throw Error("Incorrect call method 'getLocalTank' because local tank was not loaded");
	}

	return this->localObject;
}


bool TankModel::isLocalTankLoaded()
{
	return localObject != nullptr;
}


void TankModel::doSetHealth(double )
{
	//IGameObject *_loc2_ = nullptr;
	//bool _loc3_ = false;
	//this->getTank()->health = health;
	//this->getTitle()->setHealth(health);
	if (this->isLocal()) {
		//_loc2_ = battleService->getBattle();
		//_loc3_ = param1 >= this->getTankSet()->maxHealth && !IDroneModel(object.adapt(IDroneModel)).canOverheal();
		//IInventoryModel(_loc2_.adapt(IInventoryModel)).lockItem(InventoryItemType.FIRST_AID,InventoryLock.FORCED,_loc3_);
	}
}


void TankModel::activateTank()
{
	Tank *tank = this->getTank();
	if (tank != nullptr)
	{
		tank->setActivatedState();
		this->getWeaponController()->activateWeapon();
		this->getWeaponController()->unlockWeapon(TankControlLockBits::DEAD | TankControlLockBits::DISABLED);

		if (object() == this->localObject)
		{
			tank->setBodyCollisionGroup(tank->getBodyCollisionGroup() | CollisionGroup::BONUS_WITH_TANK);
			//this.battleEventSupport.dispatchEvent(new LocalTankActivationEvent(tank));
		}
		//this.battleEventSupport.dispatchEvent(new TankActivationEvent(tank));
		//this.removeTankFromExclusionSet(tank);
	}

	battleService->activateFollowCamera();
}


void TankModel::initLocalTank(Tank *tank, bool)
{
	this->localObject = object();
	//this.chassisStateCorrectionTask = new ChassisStateCorrectionTask(param1,this.tanksInBattle);
	//var _loc3_:RegularUserTitleRenderer = new RegularUserTitleRenderer(param1,this.tanksInBattle);
	//putData(RegularUserTitleRenderer,_loc3_);
	//battleService.getBattleScene3D().setUserTitleRenderer(_loc3_);
	battleService->setFollowCameraTarget(tank);
	//putData(StateCorrectionSupport,new StateCorrectionSupport(object));
	//putData(ClientDeactivationSupport,new ClientDeactivationSupport(object));
	//putData(MainLoopExecutionErrorHandler,new MainLoopExecutionErrorHandler(object));
	//var _loc4_:LocalTankLoadListener = LocalTankLoadListener(object.event(LocalTankLoadListener));
	//_loc4_.localTankLoaded(param2);
}


void TankModel::enableStateCorrection()
{
	//battleService->getBattleRunner()->addPostPhysicsController(this->chassisStateCorrectionTask);
	//this->chassisStateCorrectionTask.reset();
}


void TankModel::removeTankFromBattle()
{
	Tank *tank = this->getTank();

	Long id = object()->id();

	if (tanksInBattle.contains(id))
	{
		tanksInBattle.remove(id);

		//this->asRemoveFromBattleListener(this.getTankSet().turret).onRemoveFromBattle();
		//this->asRemoveFromBattleListener(object).onRemoveFromBattle();
		tank->removeFromBattle();
		//battleEventDispatcher.dispatchEvent(new TankRemovedFromBattleEvent(_local_1));
		if (this->isLocal())
		{
			this->getLocalChassisController()->disable();
		}
	}
}
