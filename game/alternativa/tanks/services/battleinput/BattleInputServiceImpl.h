#ifndef BATTLEINPUTSERVICEIMPL_H
#define BATTLEINPUTSERVICEIMPL_H

#include "game/alternativa/tanks/utils/BitMask.h"
#include "entrance/alternativa/tanks/service/settings/keybinding/GameActionEnum.h"
#include "game/alternativa/tanks/services/battleinput/GameActionListener.h"
#include "MyList.h"
#include "game/alternativa/tanks/services/battleinput/BattleInputService.h"


class BattleInputServiceImpl : public BattleInputService
{
	KeyCode pressedKeys[256];
	char activeActions[256];

	MyList<GameActionListener*> actionListeners;

public:
	BattleInputServiceImpl();

	void onKeyDown(KeyCode keyCode) override;
	void onKeyUp(KeyCode keyCode) override;
	void lock(BattleInputLockType*) override;
	void unlock(BattleInputLockType*) override;
	void lockMouseLocking(MouseLockLockType*) override;
	void unlockMouseLocking(MouseLockLockType*) override;
	bool isInputLocked() override;
	void addGameActionListener(GameActionListener*) override;
	void removeGameActionListener(GameActionListener*) override;
	void addMouseLockListener(MouseLockListener*) override;
	void removeMouseLockListener(MouseLockListener*) override;
	void addMouseMoveListener(MouseMovementListener*) override;
	void removeMouseMoveListener(MouseMovementListener*) override;
	void addMouseWheelListener(MouseWheelListener*) override;
	void removeMouseWheelListener(MouseWheelListener*) override;
	void releaseMouse() override;
	void forbidMouseLock() override;
	void allowMouseLock() override;

private:
	bool isLocked();
	bool isNotLocked();
	BitMask inputLocks;
	void handleActionActivation(GameActionEnum);
	void handleActionDeactivation(GameActionEnum);
	void dispatchActionEvent(GameActionEnum, bool);
};

#endif // BATTLEINPUTSERVICEIMPL_H
