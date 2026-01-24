#ifndef CHASSISCONTROLLER_H
#define CHASSISCONTROLLER_H

#include "game/alternativa/tanks/utils/BitMask.h"

class ChassisControlListener;
class Tank;

class ChassisController
{
	BitMask lockMask;
	Tank *tank;
	bool isNotLocked();
	void applyControlState(int controlState, int turnSpeedNumber);

protected:
	int controlState;
	int turnSpeedNumber;
	int appliedControlState;
	ChassisControlListener *listener;
	void onAppliedControlStateChanged(int);

public:
	ChassisController(Tank *tank, ChassisControlListener *listener);

	void lock(int status);
	void unlock(int status);

	void setControlState(int controlState, int turnSpeedNumber);
	int getControlState();

	enum FLAGS
	{
		BIT_FORWARD = 0,
		BIT_BACK = 1,
		BIT_LEFT = 2,
		BIT_RIGHT = 3,
		BIT_REVERSE_TURN = 4,
	};
};

#endif // CHASSISCONTROLLER_H
