#ifndef SHELL_H
#define SHELL_H

#include "game/alternativa/tanks/utils/objectpool/PooledObject.h"
#include "game/alternativa/tanks/battle/PhysicsController.h"
#include "game/alternativa/tanks/battle/PhysicsInterpolator.h"
#include "game/alternativa/physics/collision/IRayCollisionFilter.h"
#include "game/alternativa/tanks/battle/scene3d/Renderer.h"


class Vector3;
class ShellStates;
class AllGlobalGunParams;
class Body;
class BattleService;
class EncryptedInt;
class RayHit;


// extends PooledObject implements PhysicsController, Renderer, PhysicsInterpolator, IRayCollisionFilter

class Shell : public PooledObject,
		public virtual PhysicsController,
		public virtual PhysicsInterpolator,
		public virtual IRayCollisionFilter,
		public virtual Renderer
{
	int shotId;

	void runBeforePhysicsUpdate(double) override;

public:

	static BattleService *battleService;

	Shell(Pool *pool);

	virtual void addToGame(AllGlobalGunParams *_arg_1, const Vector3 *flightDirection, Body *body, bool isRemoteShot, int shotId);
	int getShotId();
	void render(int, int) override;

protected:

	Vector3 **radialPoints;
	Vector3 *prevPosition;
	Vector3 *currPosition;
	Vector3 *flightDirection;
	double totalDistance;
	ShellStates *shellStates;
	EncryptedInt *thousandth;
	Vector3 *interpolatedPosition;
	Body *shooterBody;
	RayHit *_rayHit;
	bool isRemoteShot;
	Vector3 *barrelOrigin;
	bool isFirstTick;

	int getNumRadialRays();
	virtual void destroy();
	virtual void processHitImpl(Body*, const Vector3*, const Vector3*, double, int);
	void processHit(Body *body, const Vector3 *_arg_2, const Vector3 *_arg_3, double _arg_4);
	bool checkIfBarrelIntersectsWithObstacle();
	void initRadialPoints(const Vector3 *_arg_1, const Vector3 *_arg_2);
	virtual void update(double);
	int getTimeMsToHit(double _arg_1);
	virtual double getRadius();
	virtual ShellStates* createShellStates();

private:
	void processFirstTick(double _arg_1);
	void interpolatePhysicsState(double _arg_1, int _arg_2) override;
	bool considerBody(Body*) override;
};

#endif // SHELL_H
