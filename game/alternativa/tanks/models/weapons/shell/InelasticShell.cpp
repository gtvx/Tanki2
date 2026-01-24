#include "InelasticShell.h"
#include "game/alternativa/math/Vector3.h"
#include "states/ShellStates.h"
#include "Error.h"
#include "game/alternativa/physics/collision/CollisionDetector.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/tanks/models/weapon/shared/MarginalCollider.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "game/alternativa/tanks/physics/TanksCollisionDetector.h"



InelasticShell::InelasticShell(Pool *pool) :
	Shell(pool)
{

}


void InelasticShell::update(double _arg_1)
{
	this->updatePosition(_arg_1);

	Vector3 _rayCastDirection;

	_rayCastDirection.diff(currPosition, prevPosition);

	double _local_2 = _rayCastDirection.length();

	_rayCastDirection.normalize();

	CollisionDetector *_local_3 = battleService->getBattleRunner()->getCollisionDetector();

	Vector3 _hitPoint;

	if (_local_3->raycast(prevPosition, &_rayCastDirection, CollisionGroup::WEAPON, _local_2, this, _rayHit))
	{
		Body *_local_6;

		bool _local_5 = (BattleUtils::isTankBody(_rayHit->shape->body) && (!MarginalCollider::segmentWithStaticIntersection(prevPosition, &_rayHit->position)));
		_hitPoint.copy(&_rayHit->position);

		if (_local_5)
		{
			_local_6 = _rayHit->shape->body;
		}
		else
		{
			_local_6 = nullptr;
			_hitPoint.subtract(&_rayCastDirection);
		}


		processHit(_local_6, &_hitPoint, &_rayCastDirection, _local_2);
		return;
	}

	int _local_4 = 0;
	while (_local_4 < getNumRadialRays())
	{
		Vector3 *_local_7 = radialPoints[_local_4];
		if (_local_3->raycast(_local_7, &_rayCastDirection, CollisionGroup::WEAPON, _local_2, this, _rayHit))
		{
			if (BattleUtils::isTankBody(_rayHit->shape->body))
			{
				if ((!(MarginalCollider::segmentWithStaticIntersection(prevPosition, &_rayHit->position))))
				{
					_hitPoint.copy(prevPosition);
					_hitPoint.addScaled(_rayHit->t, &_rayCastDirection);

					processHit(_rayHit->shape->body, &_hitPoint, &_rayCastDirection, _local_2);
					return;
				}
			}
		}

		_local_7->addScaled(_local_2, &_rayCastDirection);
		_local_4++;
	}

	shellStates->updateState(currPosition, flightDirection);
	this->updateTotalDistance(_arg_1);

	if (this->isFlightFinished())
	{
		this->handleFlightFinish();
	}
}


void InelasticShell::processHitImpl(Body *_arg_1, const Vector3 *_arg_2, const Vector3 *_arg_3, double _arg_4, int _arg_5)
{
	(void)_arg_1;
	(void)_arg_4;
	shellStates->updateState(_arg_2, _arg_3, _arg_5);
}


void InelasticShell::updateTotalDistance(double _arg_1)
{
	totalDistance = totalDistance + (_arg_1 * this->getSpeed());
}


void InelasticShell::handleFlightFinish()
{
	destroy();
}


bool InelasticShell::isFlightFinished()
{
	return totalDistance > this->getMaxDistance();
}


double InelasticShell::getMaxDistance()
{
	return std::numeric_limits<double>::max();
}


double InelasticShell::getSpeed()
{
	throw IllegalOperationError();
}


void InelasticShell::updatePosition(double _arg_1)
{
	prevPosition->copy(currPosition);
	currPosition->addScaled((this->getSpeed() * _arg_1), flightDirection);
}
