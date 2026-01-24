#include "Shell.h"
#include "game/alternativa/math/Vector3.h"
#include "../../weapon/AllGlobalGunParams.h"
#include "game/alternativa/tanks/utils/EncryptedIntImpl.h"
#include "Error.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "MyMath.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "game/alternativa/tanks/battle/scene3d/BattleScene3D.h"
#include "game/alternativa/math/Matrix3.h"
#include "game/alternativa/tanks/physics/TanksCollisionDetector.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/alternativa/tanks/utils/objectpool/ObjectPool.h"
#include "game/alternativa/tanks/models/weapons/shell/states/ShellStates.h"
#include "game/alternativa/tanks/models/weapons/shell/states/DummyShellStates.h"


BattleService *Shell::battleService;

static const int NUM_RADIAL_RAYS = 8;
static const double RADIAL_ANGLE_STEP = ((2 * MyMath::PI) / NUM_RADIAL_RAYS);



Shell::Shell(Pool *pool) :
	PooledObject(pool)
{
	prevPosition = new Vector3(0, 0, 0);
	currPosition = new Vector3(0, 0, 0);
	flightDirection = new Vector3(0, 0, 0);
	interpolatedPosition = new Vector3(0, 0, 0);
	barrelOrigin = new Vector3(0, 0, 0);

	isRemoteShot = false;

	_rayHit = new RayHit;

	totalDistance = 0;

	shotId = 0;

	isFirstTick = false;

	shooterBody = nullptr;

	thousandth = new EncryptedIntImpl(1000);

	this->radialPoints = new Vector3*[this->getNumRadialRays()];

	int i = 0;

	while (i < this->getNumRadialRays())
	{
		this->radialPoints[i] = new Vector3();
		i++;
	}
}


int Shell::getNumRadialRays()
{
	return NUM_RADIAL_RAYS;
}


void Shell::destroy()
{
	battleService->getBattleRunner()->removePhysicsController(this);
	battleService->getBattleRunner()->removePhysicsInterpolator(this);
	battleService->getBattleScene3D()->removeRenderer(this, 0);
	this->shellStates->destroy();
	recycle();
}


void Shell::addToGame(AllGlobalGunParams *_arg_1, const Vector3 *flightDirection, Body *body, bool isRemoteShot, int shotId)
{
	this->totalDistance = 0;
	this->isFirstTick = true;
	this->shotId = shotId;
	this->isRemoteShot = isRemoteShot;
	this->barrelOrigin->copy(&_arg_1->barrelOrigin);
	this->currPosition->copy(&_arg_1->muzzlePosition);
	this->prevPosition->copy(&_arg_1->muzzlePosition);

	this->flightDirection->copy(flightDirection);
	this->shooterBody = body;

	battleService->getBattleRunner()->addPhysicsController(this);
	battleService->getBattleRunner()->addPhysicsInterpolator(this);
	battleService->getBattleScene3D()->addRenderer(this, 0);
	this->shellStates = this->createShellStates();
	this->shellStates->start(this->currPosition, flightDirection);
}


void Shell::processHitImpl(Body*, const Vector3*, const Vector3*, double, int)
{
	throw IllegalOperationError();
}


int Shell::getShotId()
{
	return this->shotId;
}


void Shell::runBeforePhysicsUpdate(double v)
{
	if (this->isFirstTick)
	{
		this->isFirstTick = false;
		this->processFirstTick(v);
	}
	else
	{
		this->update(v);
	}
}


void Shell::processFirstTick(double _arg_1)
{
	if (!this->checkIfBarrelIntersectsWithObstacle())
	{
		this->initRadialPoints(this->currPosition, this->flightDirection);
		this->update(_arg_1);
	}
}


bool Shell::checkIfBarrelIntersectsWithObstacle()
{
	CollisionDetector *_local_1 = battleService->getBattleRunner()->getCollisionDetector();

	Vector3 _barrelDirection;

	_barrelDirection.diff(this->currPosition, this->barrelOrigin);

	double _local_2 = _barrelDirection.length();

	_barrelDirection.normalize();

	if (_local_1->raycast(this->barrelOrigin, &_barrelDirection, CollisionGroup::WEAPON, _local_2, this, _rayHit))
	{
		Body *_local_3 = _rayHit->shape->body;
		if (_local_3->tank == nullptr)
		{
			_rayHit->position.subtract(&_barrelDirection);
		}
		this->processHit(_local_3, &_rayHit->position, &_barrelDirection, _local_2);
		return true;
	}

	this->initRadialPoints(this->barrelOrigin, &_barrelDirection);
	int _local_4 = 0;
	while (_local_4 < this->getNumRadialRays())
	{
		Vector3 *_local_5 = this->radialPoints[_local_4];
		if (_local_1->raycast(_local_5, &_barrelDirection, CollisionGroup::WEAPON, _local_2, this, _rayHit))
		{
			Body *_local_3 = _rayHit->shape->body;
			if (BattleUtils::isTankBody(_local_3))
			{
				Vector3 _hitPoint;

				_hitPoint.copy(this->barrelOrigin);

				_hitPoint.addScaled(_rayHit->t, &_barrelDirection);
				this->processHit(_local_3, &_hitPoint, &_barrelDirection, _local_2);
				return true;
			}
		}
		_local_4++;
	}

	return false;
}


static void getRadialVector(const Vector3 *_arg_1, Vector3 *_arg_2)
{
	int _local_3;
	double _local_4 = 10000000000;
	double _local_5 = ((_arg_1->x < 0) ? -(_arg_1->x) : _arg_1->x);
	if (_local_5 < _local_4)
	{
		_local_4 = _local_5;
		_local_3 = 0;
	}
	_local_5 = ((_arg_1->y < 0) ? -(_arg_1->y) : _arg_1->y);
	if (_local_5 < _local_4)
	{
		_local_4 = _local_5;
		_local_3 = 1;
	}
	_local_5 = ((_arg_1->z < 0) ? -(_arg_1->z) : _arg_1->z);
	if (_local_5 < _local_4)
	{
		_local_3 = 2;
	}
	if (_local_3 == 0)
	{
		_arg_2->reset(0, _arg_1->z, -(_arg_1->y));
	}
	else
	{
		if (_local_3 == 1)
		{
			_arg_2->reset(-(_arg_1->z), 0, _arg_1->x);
		}
		else
		{
			if (_local_3 == 2)
			{
				_arg_2->reset(_arg_1->y, -(_arg_1->x), 0);
			}
		}
	}
}


void Shell::initRadialPoints(const Vector3 *_arg_1, const Vector3 *_arg_2)
{
	Vector3 _radialVector;

	getRadialVector(_arg_2, &_radialVector);

	_radialVector.normalize();
	_radialVector.scale(this->getRadius());

	Matrix3 _rotationMatrix;

	_rotationMatrix.fromAxisAngle(_arg_2, RADIAL_ANGLE_STEP);

	this->radialPoints[0]->sum(_arg_1, &_radialVector);

	int _local_3 = 1;
	while (_local_3 < this->getNumRadialRays())
	{
		_radialVector.transform3(&_rotationMatrix);
		this->radialPoints[_local_3]->sum(_arg_1, &_radialVector);
		_local_3++;
	}
}


double Shell::getRadius()
{
	throw IllegalOperationError();
}


void Shell::update(double)
{
	throw IllegalOperationError();
}


void Shell::interpolatePhysicsState(double _arg_1, int _arg_2)
{
	(void)_arg_2;
	this->interpolatedPosition->interpolate(_arg_1, this->prevPosition, this->currPosition);
}


bool Shell::considerBody(Body *body)
{
	return this->shooterBody != body;
}


int Shell::getTimeMsToHit(double _arg_1)
{
	return MyMath::round(((_rayHit->t / _arg_1) * BattleRunner::PHYSICS_STEP_IN_MS));
}


void Shell::processHit(Body *body, const Vector3 *_arg_2, const Vector3 *_arg_3, double _arg_4)
{
	this->processHitImpl(body, _arg_2, _arg_3, this->totalDistance + _rayHit->t, this->getTimeMsToHit(_arg_4));
}


void Shell::render(int, int)
{
	throw IllegalOperationError();
}


ShellStates* Shell::createShellStates()
{
	if (this->isRemoteShot)
	{
		return (ShellStates*)DummyShellStates::INSTANCE;
	}

	return battleService->getObjectPool()->getObject<ShellStates>();
}
