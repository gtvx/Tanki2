#ifndef BATTLEINPUTSERVICE_H
#define BATTLEINPUTSERVICE_H

typedef unsigned char KeyCode;

class BattleInputLockType;
class MouseLockLockType;
class GameActionListener;
class MouseLockListener;
class MouseMovementListener;
class MouseWheelListener;

class BattleInputService
{
public:
	virtual void onKeyDown(KeyCode keyCode) = 0;
	virtual void onKeyUp(KeyCode keyCode) = 0;
	virtual void lock(BattleInputLockType*) = 0;
	virtual void unlock(BattleInputLockType*) = 0;
	virtual void lockMouseLocking(MouseLockLockType*) = 0;
	virtual void unlockMouseLocking(MouseLockLockType*) = 0;
	virtual bool isInputLocked() = 0;
	virtual void addGameActionListener(GameActionListener*) = 0;
	virtual void removeGameActionListener(GameActionListener*) = 0;
	virtual void addMouseLockListener(MouseLockListener*) = 0;
	virtual void removeMouseLockListener(MouseLockListener*) = 0;
	virtual void addMouseMoveListener(MouseMovementListener*) = 0;
	virtual void removeMouseMoveListener(MouseMovementListener*) = 0;
	virtual void addMouseWheelListener(MouseWheelListener*) = 0;
	virtual void removeMouseWheelListener(MouseWheelListener*) = 0;
	virtual void releaseMouse() = 0;
	virtual void forbidMouseLock() = 0;
	virtual void allowMouseLock() = 0;
};

#endif // BATTLEINPUTSERVICE_H
