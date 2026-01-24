#include "ShellStates.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/tanks/models/weapon/shared/MarginalCollider.h"
#include "MyMath.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/common/shell/ShellState.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "game/alternativa/tanks/battle/BattleService.h"


BattleService *ShellStates::battleService;


ShellStates::ShellStates(Pool *pool) :
	PooledObject(pool)
{
	lastControlPosition = new Vector3;
	lastControlDirection = new Vector3;

	prevPosition = new Vector3;
	prevDirection = new Vector3;

	lastControlTime = 0;
	startTime = 0;
	prevPositionAlreadyAdded = false;
	prevStateTime = 0;
}


Vector3* ShellStates::getLastControlPosition()
{
	return this->lastControlPosition;
}


Vector3* ShellStates::getLastControlDirection()
{
	return this->lastControlDirection;
}


int ShellStates::getLastControlTime()
{
	return this->lastControlTime;
}


int ShellStates::getTimeSinceLastControlState()
{
	return (((battleService->getPhysicsTime() + BattleRunner::PHYSICS_STEP_IN_MS) - this->startTime) - this->lastControlTime);
}


void ShellStates::start(const Vector3 *_arg_1, const Vector3 *_arg_2)
{
	this->states.clear();
	this->lastControlPosition->copy(_arg_1);
	this->lastControlDirection->copy(_arg_2);
	this->startTime = battleService->getPhysicsTime();
	this->lastControlTime = 0;
	this->prevPositionAlreadyAdded = true;
}


void ShellStates::updateState(const Vector3 *_arg_1, const Vector3 *_arg_2, int _arg_3)
{
	if (MarginalCollider::segmentWithStaticIntersection(this->lastControlPosition, _arg_1))
	{
		this->savePrevPosition();
	}
	this->prevPosition->copy(_arg_1);
	this->prevDirection->copy(_arg_2);
	this->prevStateTime = ((battleService->getPhysicsTime() + MyMath::min(_arg_3, BattleRunner::PHYSICS_STEP_IN_MS)) - this->startTime);
	this->prevPositionAlreadyAdded = false;
}


QVector<ShellState*> ShellStates::getControlStates()
{
	this->savePrevPosition();
	return this->states;
}


static Vector3d ToVector3d(const Vector3 *v)
{
	return Vector3d(v->x, v->y, v->z);
}


void ShellStates::savePrevPosition()
{
	if (!this->prevPositionAlreadyAdded)
	{
		this->states.append(new ShellState(this->prevStateTime, ToVector3d(this->prevDirection), ToVector3d(this->prevPosition)));
		this->lastControlTime = this->prevStateTime;
		this->lastControlPosition->copy(this->prevPosition);
		this->lastControlDirection->copy(this->prevDirection);
		this->prevPositionAlreadyAdded = true;
	}
}


void ShellStates::destroy()
{
	this->states.clear();
	recycle();
}
