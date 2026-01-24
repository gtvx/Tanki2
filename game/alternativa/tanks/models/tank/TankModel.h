#ifndef TANKMODEL_H
#define TANKMODEL_H


#include "entrance/platform/client/fp10/core/model/impl/Model.h"
#include "game/alternativa/tanks/battle/objects/tank/controllers/ChassisControlListener.h"
#include "LocalTankInfoService.h"
#include "game/projects/tanks/client/battlefield/models/user/tank/TankModelBase.h"
#include "ITankModel.h"
#include <QHash>

class IGameObject;
class Tank;
class WeaponMount;
class Weapon;
class UserTitle;
class TankCC;
class IWeaponCommonModel;
class IWeaponController;
class Vector3d;
class ChassisController;
class LocalChassisController;
class ChassisStateCorrectionTask;
class BattleService;


//extends TankModelBase implements ITankModelBase, ObjectLoadListener, ObjectUnloadListener, ITankModel, LocalTankInfoService, ChassisControlListener

class TankModel :
		public TankModelBase,
		public ChassisControlListener,
		public LocalTankInfoService,
		public ITankModel
{
public:
	TankModel();

	bool isLocal();
	void objectLoaded(IGameObject *object);
	Tank *getTank();
	void addTankToBattle();
	void lockMovementControl(int);
	void unlockMovementControl(int);
	IWeaponController *getWeaponController();
	void setChassisState(const Vector3d*, const Vector3d*, const Vector3d*, const Vector3d*, int, int);
	ChassisController *getChassisController();
	void activateTank();
	void initLocalTank(Tank *tank, bool);
	void enableStateCorrection();
	void objectUnloaded();
	void removeTankFromBattle();

	QHash<Long, Tank*> tanksInBattle;

private:

	std::shared_ptr<Tank> initTank(IGameObject*, IGameObject*, IGameObject*, int, bool);
	void setInitialSpeedCharacteristics();
	std::shared_ptr<Tank> createTank(IGameObject*, IGameObject*, IGameObject*, WeaponMount*, Weapon*, UserTitle*);
	void createChassisController(Tank*);
	WeaponMount* createWeaponMount(IGameObject*);
	void registerWeaponController(Weapon*);
	void onChassisControlChanged(int, bool);
	void initTankPart(IGameObject*, std::shared_ptr<Tank> tank);
	IWeaponCommonModel *getWeaponCommon(IGameObject*);
	void storeTank(std::shared_ptr<Tank>, IGameObject*, IGameObject*, IGameObject*, int);
	Weapon* createWeapon(IGameObject*);
	void addExistingTankToBattle();
	void registerUser();
	bool tankShouldBeAddedToBattle();
	LocalChassisController *getLocalChassisController();	
	void doSetHealth(double health);
	void unregisterUser();
	void destroyTank(bool);
	void unloadLocalTank(bool);

	bool isLocalTankLoaded() override;
	IGameObject* getLocalTankObject() override;
	IGameObject* getLocalTankObjectOrNull() override;
	Tank* getLocalTank() override;

	ChassisStateCorrectionTask *chassisStateCorrectionTask;
	IGameObject *localObject;
};

#endif // TANKMODEL_H
