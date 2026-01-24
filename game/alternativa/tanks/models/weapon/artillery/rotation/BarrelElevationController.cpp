#include "BarrelElevationController.h"
#include "game/alternativa/tanks/camera/FollowCameraController.h"
#include "game/alternativa/tanks/battle/objects/tank/controllers/BarrelElevator.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "Error.h"

static const double MOUSE_SENS_MUL = 0.0001;


BarrelElevationController::BarrelElevationController(Tank *tank, BarrelElevator *barrelElevator)
{
	this->tank = nullptr;
	this->isEnabled = false;
	this->up = false;
	this->down = false;
	this->mouseLookDirection = 0;
	this->tank = tank;
	this->barrelElevator = barrelElevator;
}


void BarrelElevationController::enable()
{
	if (!this->isEnabled)
	{
		this->isEnabled = true;
		//battleInputService.addMouseLockListener(this);
		//battleInputService.addMouseMoveListener(this);
		//battleInputService.addGameActionListener(this);
	}
}


void BarrelElevationController::disable()
{
	if (this->isEnabled)
	{
		this->isEnabled = false;
		this->up = false;
		this->down = false;
		this->barrelElevator->setUserControl(BarrelElevator::STOP);
		//battleInputService.removeMouseLockListener(this);
		//battleInputService.removeMouseMoveListener(this);
		//battleInputService.removeGameActionListener(this);
	}
}


void BarrelElevationController::onMouseLock(bool param1)
{
	if (param1)
	{
		if (FollowCameraController::getFollowCameraMode() != FollowCameraController::CAMERA_FOLLOWS_MOUSE)
		{
			this->mouseLookDirection = this->tank->getInterpolatedTurretWorldDirection();
			FollowCameraController::setFollowCameraMode(FollowCameraController::CAMERA_FOLLOWS_MOUSE);
			FollowCameraController::setFollowCameraDirection(this->mouseLookDirection);
		}
	}
	else //if (!fullScreenService.isFullScreenNow())
	{
		FollowCameraController::setFollowCameraMode(FollowCameraController::CAMERA_FOLLOWS_TURRET);
	}
}


void BarrelElevationController::onMouseRelativeMovement(double, double)
{
	throw Error("used BarrelElevationController::onMouseRelativeMovement");
	//this->mouseLookDirection = MathUtils::clampAngle(this->mouseLookDirection - param1 * settingsService.mouseSensitivity * MOUSE_SENS_MUL);
	//FollowCameraController::setFollowCameraDirection(this->mouseLookDirection);
}


void BarrelElevationController::onGameAction(GameActionEnum param1, bool param2)
{
	switch(param1)
	{
	case GameActionEnum::ROTATE_TURRET_LEFT:
		this->rotateDown(param2);
		break;
	case GameActionEnum::ROTATE_TURRET_RIGHT:
		this->rotateUp(param2);
		break;
	case GameActionEnum::CENTER_TURRET:
		this->center(param2);
		break;
	case GameActionEnum::LOOK_AROUND:
		if (param2)
		{
			//battleInputService.releaseMouse();
			FollowCameraController::setFollowCameraMode(FollowCameraController::CAMERA_FOLLOWS_TURRET);
			break;
		}
		break;
	default:
		break;
	}
}


void BarrelElevationController::rotateUp(bool param1)
{
	this->up = param1;
	this->setDirectionalRotation();
}


void BarrelElevationController::rotateDown(bool param1)
{
	this->down = param1;
	this->setDirectionalRotation();
}


void BarrelElevationController::setDirectionalRotation()
{
	if (this->up == this->down)
	{
		this->barrelElevator->setUserControl(BarrelElevator::STOP);
	}
	else if (this->up)
	{
		this->barrelElevator->setUserControl(BarrelElevator::UP);
	}
	else if (this->down)
	{
		this->barrelElevator->setUserControl(BarrelElevator::DOWN);
	}
}


void BarrelElevationController::center(bool param1)
{
	if (param1 && !(this->up || this->down))
	{
		this->barrelElevator->setUserControl(BarrelElevator::CENTER);
	}
}


void BarrelElevationController::onAddToBattle()
{
	if (FollowCameraController::getFollowCameraMode() == FollowCameraController::CAMERA_FOLLOWS_MOUSE)
	{
		this->mouseLookDirection = this->tank->getInterpolatedTurretWorldDirection();
		FollowCameraController::setFollowCameraDirection(this->mouseLookDirection);
	}
}
