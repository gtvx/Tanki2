#include "ITankModelAdapt.h"
#include "entrance/platform/client/fp10/core/model/impl/Model.h"
#include <QDebug>


ITankModelAdapt::ITankModelAdapt(IGameObject *object, std::shared_ptr<ITankModel> impl)
{
	this->object = object;
	this->impl = impl;
}


Tank* ITankModelAdapt::getTank()
{
	Tank *result = nullptr;

	try
	{
		Model::set_object(this->object);
		result = this->impl->getTank();
	}
	catch (...)
	{
		qDebug("error ITankModelAdapt::getTank");
	}

	Model::popObject();

	return result;
}


bool ITankModelAdapt::isLocal()
{
	bool result = false;

	try
	{
		Model::set_object(this->object);
		result = this->impl->isLocal();
	}
	catch (...)
	{

	}

	Model::popObject();

	return result;
}


#if 0
public function lockMovementControl(bits:int):void
{
	try
	{
		Model.object = this.object;
		this.impl.lockMovementControl(bits);
	}
	finally
	{
		Model.popObject();
	};
}

public function unlockMovementControl(bits:int):void
{
	try
	{
		Model.object = this.object;
		this.impl.unlockMovementControl(bits);
	}
	finally
	{
		Model.popObject();
	};
}

public function getWeaponController():IWeaponController
{
	var result:IWeaponController;
	try
	{
		Model.object = this.object;
		result = this.impl.getWeaponController();
	}
	finally
	{
		Model.popObject();
	};
	return (result);
}

public function getChassisController():ChassisController
{
	var result:ChassisController;
	try
	{
		Model.object = this.object;
		result = this.impl.getChassisController();
	}
	finally
	{
		Model.popObject();
	};
	return (result);
}

public function getWeaponMount():WeaponMount
{
	var result:WeaponMount;
	try
	{
		Model.object = this.object;
		result = this.impl.getWeaponMount();
	}
	finally
	{
		Model.popObject();
	};
	return (result);
}

public function getTitle():UserTitle
{
	var result:UserTitle;
	try
	{
		Model.object = this.object;
		result = this.impl.getTitle();
	}
	finally
	{
		Model.popObject();
	};
	return (result);
}

public function getUserInfo():UserInfo
{
	var result:UserInfo;
	try
	{
		Model.object = this.object;
		result = this.impl.getUserInfo();
	}
	finally
	{
		Model.popObject();
	};
	return (result);
}

public function sendStateCorrection(mandatoryCorrection:Boolean):void
{
	try
	{
		Model.object = this.object;
		this.impl.sendStateCorrection(mandatoryCorrection);
	}
	finally
	{
		Model.popObject();
	};
}

public function onPrevStateCorrection(mandatoryCorrection:Boolean):void
{
	try
	{
		Model.object = this.object;
		this.impl.onPrevStateCorrection(mandatoryCorrection);
	}
	finally
	{
		Model.popObject();
	};
}

public function handleCollisionWithOtherTank(tank:Tank):void
{
	try
	{
		Model.object = this.object;
		this.impl.handleCollisionWithOtherTank(tank);
	}
	finally
	{
		Model.popObject();
	};
}

public function sendDeathConfirmationCommand():void
{
	try
	{
		Model.object = this.object;
		this.impl.sendDeathConfirmationCommand();
	}
	finally
	{
		Model.popObject();
	};
}

public function disableStateCorrection():void
{
	try
	{
		Model.object = this.object;
		this.impl.disableStateCorrection();
	}
	finally
	{
		Model.popObject();
	};
}

public function enableStateCorrection():void
{
	try
	{
		Model.object = this.object;
		this.impl.enableStateCorrection();
	}
	finally
	{
		Model.popObject();
	};
}

public function die(respawnDelay:int):void
{
	try
	{
		Model.object = this.object;
		this.impl.die(respawnDelay);
	}
	finally
	{
		Model.popObject();
	};
}

public function isLocal():Boolean
{
	var result:Boolean;
	try
	{
		Model.object = this.object;
		result = this.impl.isLocal();
	}
	finally
	{
		Model.popObject();
	};
	return (result);
}

public function configureTankTitleAsRemote(user:IGameObject):void
{
	try
	{
		Model.object = this.object;
		this.impl.configureTankTitleAsRemote(user);
	}
	finally
	{
		Model.popObject();
	};
}

public function configureRemoteTankTitles():void
{
	try
	{
		Model.object = this.object;
		this.impl.configureRemoteTankTitles();
	}
	finally
	{
		Model.popObject();
	};
}

public function addTankToBattle():void
{
	try
	{
		Model.object = this.object;
		this.impl.addTankToBattle();
	}
	finally
	{
		Model.popObject();
	};
}

public function removeTankFromBattle():void
{
	try
	{
		Model.object = this.object;
		this.impl.removeTankFromBattle();
	}
	finally
	{
		Model.popObject();
	};
}

public function doSetHealth(health:Number):void
{
	try
	{
		Model.object = this.object;
		this.impl.doSetHealth(health);
	}
	finally
	{
		Model.popObject();
	};
}

public function setChassisState(position:Vector3d, orientation:Vector3d, linearVelocity:Vector3d, angularVelocity:Vector3d, controlState:int, turnSpeedNumber:int):void
{
	try
	{
		Model.object = this.object;
		this.impl.setChassisState(position, orientation, linearVelocity, angularVelocity, controlState, turnSpeedNumber);
	}
	finally
	{
		Model.popObject();
	};
}

public function getMovementAnticheatTask():MovementTimeoutAndDistanceAnticheatTask
{
	var result:MovementTimeoutAndDistanceAnticheatTask;
	try
	{
		Model.object = this.object;
		result = this.impl.getMovementAnticheatTask();
	}
	finally
	{
		Model.popObject();
	};
	return (result);
}

public function addTankToExclusionSet(tank:Tank):void
{
	try
	{
		Model.object = this.object;
		this.impl.addTankToExclusionSet(tank);
	}
	finally
	{
		Model.popObject();
	};
}

public function getTankSet():TankSet
{
	var result:TankSet;
	try
	{
		Model.object = this.object;
		result = this.impl.getTankSet();
	}
	finally
	{
		Model.popObject();
	};
	return (result);
}
#endif
