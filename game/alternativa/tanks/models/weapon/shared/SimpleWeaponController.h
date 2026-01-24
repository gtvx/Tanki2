#ifndef SIMPLEWEAPONCONTROLLER_H
#define SIMPLEWEAPONCONTROLLER_H

#include "entrance/alternativa/tanks/service/settings/keybinding/GameActionEnum.h"
#include "game/alternativa/tanks/services/battleinput/GameActionListener.h"

class LocalWeapon;

class SimpleWeaponController : public virtual GameActionListener
{
	LocalWeapon *weapon;
	bool triggerPulled;
	bool wasTriggerPulled;

public:
	SimpleWeaponController();

	void init();
	void destroy();
	void setWeapon(LocalWeapon *localWeapon);
	bool isTriggerPulled();
	void discardStoredAction();
	void onGameAction(GameActionEnum, bool) override;
	bool wasActive();

	void pullTrigger();
	void releaseTrigger();
};

#endif // SIMPLEWEAPONCONTROLLER_H
