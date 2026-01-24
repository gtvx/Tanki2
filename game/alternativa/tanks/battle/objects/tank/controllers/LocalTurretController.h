#ifndef LOCALTURRETCONTROLLER_H
#define LOCALTURRETCONTROLLER_H

#include "entrance/alternativa/tanks/service/settings/keybinding/GameActionEnum.h"
#include "game/alternativa/tanks/camera/CameraController.h"
#include "game/alternativa/tanks/services/battleinput/GameActionListener.h"

class Tank;
class Turret;

class LocalTurretController : virtual public CameraController, public virtual GameActionListener
{
	Tank *tank;
	Turret *turret;
	bool left, right;
	bool isEnabled;

public:
	LocalTurretController(Tank *tank, Turret *turret);

	void onGameAction(GameActionEnum, bool) override;
	void deactivate() override;
	void activate(GameCamera*) override;
	void update(GameCamera*, int, int) override;

	void enable();
	void disable();

private:
	void rotateLeft(bool);
	void rotateRight(bool);
	void setDirectionalRotationControlState();
	void center(bool s);
};

#endif // LOCALTURRETCONTROLLER_H
