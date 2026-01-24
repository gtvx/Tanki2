#ifndef ROTATINGTURRETMODEL_H
#define ROTATINGTURRETMODEL_H

#include "game/alternativa/tanks/models/tank/WeaponMountProvider.h"
#include "game/alternativa/tanks/battle/objects/tank/controllers/LocalTurretController.h"
#include "game/alternativa/tanks/models/tank/InitTankPart.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/turret/RotatingTurretModelBase.h"
#include "IRotatingTurretModel.h"

class Turret;
class TurretStateCommand;
class BattleService;

//extends RotatingTurretModelBase implements IRotatingTurretModelBase, IRotatingTurretModel, ObjectLoadListener, InitTankPart, TurretStateSender, WeaponMountProvider, AddToBattleListener, RemoveFromBattleListener


class RotatingTurretModel :
		public RotatingTurretModelBase,
		public WeaponMountProvider,
		public InitTankPart,
		public IRotatingTurretModel
{
public:

	static BattleService *battleService;

	RotatingTurretModel();

	void objectLoaded();

	TurretStateCommand *getState();

	void initTankPart(std::shared_ptr<Tank> tan);

private:
	void saveState(std::shared_ptr<TurretStateCommand> command);
	Tank *getTank();
	void setTurretState(Turret *turret, TurretStateCommand *turretStateCommand);

	std::shared_ptr<WeaponMount> createWeaponMount(IGameObject *object) override;
	LocalTurretController* getLocalTurretController() override;
	Turret* getTurret() override;
};

#endif // ROTATINGTURRETMODEL_H
