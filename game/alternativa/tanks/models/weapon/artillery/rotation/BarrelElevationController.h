#ifndef BARRELELEVATIONCONTROLLER_H
#define BARRELELEVATIONCONTROLLER_H

#include "entrance/alternativa/tanks/service/settings/keybinding/GameActionEnum.h"

class Tank;
class BarrelElevator;

class BarrelElevationController
{
	Tank *tank;
	bool isEnabled;
	BarrelElevator *barrelElevator;
	bool up;
	bool down;
	double mouseLookDirection;

public:
	BarrelElevationController(Tank *tank, BarrelElevator *barrelElevator);

	void enable();
	void disable();
	void onMouseRelativeMovement(double, double);
	void onGameAction(GameActionEnum, bool);
	void onAddToBattle();
	void onMouseLock(bool);

private:
	void rotateUp(bool);
	void rotateDown(bool);
	void setDirectionalRotation();
	void center(bool);
};

#endif // BARRELELEVATIONCONTROLLER_H
