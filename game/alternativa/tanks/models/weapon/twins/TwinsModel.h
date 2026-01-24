#ifndef TWINSMODEL_H
#define TWINSMODEL_H

#include "../IWeaponModel.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/twins/TwinsModelBase.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/twins/ITwinsModelBase.h"
#include "TwinsWeaponCallback.h"
#include <QHash>


//extends TwinsModelBase implements ITwinsModelBase, ObjectLoadListener, IWeaponModel, TwinsWeaponCallback, WeaponBuffListener, WeaponReloadTimeChangedListener, UltimateStunListener

class TwinsModel :
		public TwinsModelBase,
		public ITwinsModelBase,
		public IWeaponModel,
		public TwinsWeaponCallback
{
	QHash<IGameObject*, Weapon*> weapons;

	Weapon* createLocalWeapon(IGameObject*) override;
	Weapon* createRemoteWeapon(IGameObject*) override;

	void fire(IGameObject*, int, int, const Vector3d*) override;
	void fireDummy(IGameObject*, int) override;


	void onShot(int time, int shotId, int currentBarrel, const Vector3 *shotDirection) override;
	void onDummyShot(int time, int currentBarrel) override;
	void onTargetHit(int shotId, Body *targetBody, const Vector3 *hitPoint) override;
	void onStaticHit(int shotId, const Vector3 *position) override;

public:
	TwinsModel();
	void objectLoaded();
};

#endif // TWINSMODEL_H
