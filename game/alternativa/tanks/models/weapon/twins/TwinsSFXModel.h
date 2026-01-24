#ifndef TWINSSFXMODEL_H
#define TWINSSFXMODEL_H

#include "ITwinsSFXModel.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/sfx/shoot/twins/TwinsShootSFXModelBase.h"

class EffectsMaterialRegistry;
class BattleService;

//extends TwinsShootSFXModelBase implements ITwinsShootSFXModelBase, ObjectLoadPostListener, ObjectUnloadListener, ITwinsSFXModel


class TwinsSFXModel :
		public TwinsShootSFXModelBase,
		public ITwinsSFXModel
{
	TwinsEffects* getPlasmaWeaponEffects() override;
	TwinsSFXData* getSFXData() override;

public:

	static BattleService *battleService;
	static EffectsMaterialRegistry *materialRegistry;

	TwinsSFXModel();

	void objectLoadedPost();
	void objectUnloaded();
};

#endif // TWINSSFXMODEL_H
