#ifndef LOCALCHASSISCONTROLLER_H
#define LOCALCHASSISCONTROLLER_H

#include "ChassisController.h"
#include "entrance/alternativa/tanks/service/settings/keybinding/GameActionEnum.h"
#include "game/alternativa/tanks/services/battleinput/GameActionListener.h"

class LocalChassisController : public ChassisController, public virtual GameActionListener
{
	bool isEnabled;
	int reverseBackTurnMask;

public:
	LocalChassisController(Tank *tank, ChassisControlListener *listener);
	void enable();
	void onGameAction(GameActionEnum, bool);
	void setReversedBackTurn(bool);
	void disable();
};

#endif // LOCALCHASSISCONTROLLER_H
