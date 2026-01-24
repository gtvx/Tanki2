#include "LocalTurretController.h"
#include "Turret.h"
#include "game/alternativa/tanks/camera/FollowCameraController.h"
#include "game/alternativa/tanks/services/battleinput/BattleInputServiceImpl.h"

extern BattleInputService *battleInputService;


LocalTurretController::LocalTurretController(Tank *tank, Turret *turret)
{
	this->right = this->left = false;
	this->tank = tank;
	this->turret = turret;
	this->isEnabled = false;
	enable();
}


void LocalTurretController::rotateLeft(bool s)
{
	this->left = s;
	this->setDirectionalRotationControlState();
	FollowCameraController::setFollowCameraMode(FollowCameraController::CAMERA_FOLLOWS_TURRET);
	//battleInputService.releaseMouse();
}


void LocalTurretController::rotateRight(bool s)
{
	this->right = s;
	this->setDirectionalRotationControlState();
	FollowCameraController::setFollowCameraMode(FollowCameraController::CAMERA_FOLLOWS_TURRET);
	//battleInputService.releaseMouse();
}


void LocalTurretController::center(bool s)
{
	if (s && (!((this->left) || (this->right))))
	{
		this->turret->setTurretControlState(TurretControlType::TARGET_ANGLE_LOCAL, 0, Turret::TURN_SPEED_COUNT);
		FollowCameraController::setFollowCameraMode(FollowCameraController::CAMERA_FOLLOWS_TURRET);
		//battleInputService.releaseMouse();
	}
}


void LocalTurretController::setDirectionalRotationControlState()
{
	int v = int(this->left) - int(this->right);
	this->turret->setTurretControlState(TurretControlType::ROTATION_DIRECTION, v, Turret::TURN_SPEED_COUNT);
}


void LocalTurretController::onGameAction(GameActionEnum e, bool s)
{
	if (e == GameActionEnum::LOOK_AROUND)
	{
		/*
		this.lockTurretDirection = _arg_2;

		if (FollowCameraController.getFollowCameraMode() == FollowCameraController.CAMERA_FOLLOWS_MOUSE)
		{
			if (this.lockTurretDirection)
			{
				this->turret.setTurretControlState(TurretControlType.TARGET_ANGLE_LOCAL, this.turret.getTurretPhysicsDirection(), Turret.TURN_SPEED_COUNT);
			}
			else
			{
				this.turret.setTurretControlState(TurretControlType.TARGET_ANGLE_WORLD, this.mouseLookDirection, Turret.TURN_SPEED_COUNT);
			};
		};
		*/
	}
	else
	{
		switch (e)
		{
		case GameActionEnum::ROTATE_TURRET_LEFT:
			this->rotateLeft(s);
			return;
		case GameActionEnum::ROTATE_TURRET_RIGHT:
			this->rotateRight(s);
			return;
		case GameActionEnum::CENTER_TURRET:
			this->center(s);
			return;
		default:
			return;
		}
	}
}


void LocalTurretController::update(GameCamera *, int, int)
{

}


void LocalTurretController::activate(GameCamera *)
{

}


void LocalTurretController::deactivate()
{

}


void LocalTurretController::enable()
{
	if (!this->isEnabled)
	{
		this->isEnabled = true;
		battleInputService->addGameActionListener(this);
		//battleInputService->addMouseLockListener(this);
		//battleInputService->addMouseMoveListener(this);
	}
}


void LocalTurretController::disable()
{
	if (this->isEnabled)
	{
		this->isEnabled = false;
		battleInputService->removeGameActionListener(this);
		//battleInputService->removeMouseLockListener(this);
		//battleInputService->removeMouseMoveListener(this);
		this->left = false;
		this->right = false;
		FollowCameraController::setFollowCameraMode(FollowCameraController::CAMERA_FOLLOWS_TURRET);
		this->turret->setTurretControlState(TurretControlType::ROTATION_DIRECTION, 0, Turret::TURN_SPEED_COUNT);
	}
}
