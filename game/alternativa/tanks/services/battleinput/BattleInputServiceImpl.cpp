#include "BattleInputServiceImpl.h"
#include "getBindingAction.h"
#include "Error.h"



BattleInputServiceImpl::BattleInputServiceImpl()
{
	for (int i = 0; i < 256; i++)
		pressedKeys[i] = 0;
	for (int i = 0; i < 256; i++)
		activeActions[i] = 0;
}


void BattleInputServiceImpl::onKeyDown(KeyCode keyCode)
{
	if (!(this->pressedKeys[keyCode] == true))
	{
		this->pressedKeys[keyCode] = true;
		if (this->isNotLocked())
		{
			GameActionEnum e = getBindingAction(keyCode);
			if (e != GameActionEnum::_NULL)
			{
				this->handleActionActivation(e);
			}
		}
	}
}


void BattleInputServiceImpl::onKeyUp(KeyCode keyCode)
{
	if (this->pressedKeys[keyCode] == true)
	{
		this->pressedKeys[keyCode] = false;
		if (this->isNotLocked())
		{
			GameActionEnum e = getBindingAction(keyCode);
			if (e != GameActionEnum::_NULL)
			{
				this->handleActionDeactivation(e);
			}
		}
	}
}


void BattleInputServiceImpl::lock(BattleInputLockType *)
{

}


void BattleInputServiceImpl::unlock(BattleInputLockType *)
{

}


void BattleInputServiceImpl::lockMouseLocking(MouseLockLockType *)
{

}


void BattleInputServiceImpl::unlockMouseLocking(MouseLockLockType *)
{

}


bool BattleInputServiceImpl::isInputLocked()
{
	throw Error(2345);
}


bool BattleInputServiceImpl::isLocked()
{
	return this->inputLocks.isNotEmpty();
}


bool BattleInputServiceImpl::isNotLocked()
{
	return this->inputLocks.isEmpty();
}


void BattleInputServiceImpl::handleActionActivation(GameActionEnum e)
{
	//int b = this->activeActions[(int)e] + 1;
	//this->activeActions[(int)e] = b;
	//if (b == 1)
		this->dispatchActionEvent(e, true);
}


void BattleInputServiceImpl::handleActionDeactivation(GameActionEnum e)
{
	this->dispatchActionEvent(e, false);
	/*
	int b = this->activeActions[(int)e];
	if (b > 0) {
		if (b == 1) {
			this->activeActions[(int)e] = 0;
			this->dispatchActionEvent(e, false);
		} else {
			this->activeActions[(int)e] = b - 1;
		}
	}
	*/
}


void BattleInputServiceImpl::dispatchActionEvent(GameActionEnum e, bool b)
{
	for (int i = 0; i < this->actionListeners.count(); i++)
	{
		this->actionListeners.at(i)->onGameAction(e, b);
	}
}


void BattleInputServiceImpl::addGameActionListener(GameActionListener *l)
{
	if (actionListeners.contains(l) == false)
	{
		actionListeners.append(l);
		if (this->inputLocks.isEmpty())
		{
			/*
			GameActionEnum p
			for (_local_2 in this.activeActions)
			{
				_arg_1.onGameAction(GameActionEnum(_local_2), true);
			}
			*/
		}
	}
}


void BattleInputServiceImpl::removeGameActionListener(GameActionListener *l)
{
	this->actionListeners.removeAt(l);
}


void BattleInputServiceImpl::addMouseLockListener(MouseLockListener *)
{

}


void BattleInputServiceImpl::removeMouseLockListener(MouseLockListener *)
{

}


void BattleInputServiceImpl::addMouseMoveListener(MouseMovementListener *)
{

}


void BattleInputServiceImpl::removeMouseMoveListener(MouseMovementListener *)
{

}

void BattleInputServiceImpl::addMouseWheelListener(MouseWheelListener *)
{

}


void BattleInputServiceImpl::removeMouseWheelListener(MouseWheelListener *)
{

}


void BattleInputServiceImpl::releaseMouse()
{

}


void BattleInputServiceImpl::forbidMouseLock()
{

}


void BattleInputServiceImpl::allowMouseLock()
{

}
