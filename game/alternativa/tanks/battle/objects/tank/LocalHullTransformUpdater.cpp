#include "LocalHullTransformUpdater.h"
#include "game/alternativa/math/Quaternion.h"
#include "Tank.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankSkin.h"


LocalHullTransformUpdater::LocalHullTransformUpdater(Tank *tank)
{
	this->tank = tank;
}


void LocalHullTransformUpdater::reset()
{

}


void LocalHullTransformUpdater::update(double)
{
	this->position.copy(&this->tank->skinCenterOffset);
	this->position.transform4(&this->tank->interpolatedTransform);
	this->tank->interpolatedOrientation.getEulerAngles(&eulerAngles);
	this->tank->getSkin()->updateHullTransform(&position, &eulerAngles);
}
