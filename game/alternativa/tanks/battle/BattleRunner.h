#ifndef BATTLERUNNER_H
#define BATTLERUNNER_H

#include <QVector>
#include "MyList.h"

class TankPhysicsScene;
class Sound;
class BattleEventDispatcher;
class PhysicsController;
class TankBody;
class PhysicsInterpolator;
class CollisionShape;
class TanksCollisionDetector;
class PostPhysicsController;
class Canvas;
class PerformanceMonitor;
class FPSService;
class LogicUnit;
class DeferredAction;
class Vector3;
class Tank;
class LocalTankInfoService;


class BattleRunner
{
public:
	static const int PHYSICS_STEP_IN_MS;
	static const double PHYSICS_STEP_IN_S;

	static LocalTankInfoService *localTankInfoService;


	BattleRunner(double, Sound*, BattleEventDispatcher*);

	void addPhysicsController(PhysicsController*);
	void tick();
	void addBodyWrapper(TankBody *tankbody);
	void addPhysicsInterpolator(PhysicsInterpolator*);
	void initStaticGeometry(MyList<CollisionShape *>*);
	TanksCollisionDetector *getCollisionDetector();
	MyList<CollisionShape *> *mapStaticChildren();
	void addPostPhysicsController(PostPhysicsController*);
	void addLogicUnit(LogicUnit*);	
	int getPhysicsTime();
	void removeLogicUnit(LogicUnit *logicUnit);
	void removeBodyWrapper(TankBody *tankBody);
	void removePhysicsController(PhysicsController *);
	void removePostPhysicsController(PostPhysicsController *);
	void removePhysicsInterpolator(PhysicsInterpolator *);
	void battleViewUpdate(Canvas *canvas);

	PerformanceMonitor *physicsPerformanceMonitor;
	FPSService *fpsService;

private:
	void runPhysics(int param1);
	void runInputProcessors(int, int);
	void runPhysicsInterpolators(int);
	void updateLocalTankPhysicsState();
	void runPhysicsControllers(double);
	void runPostPhysicsControllers(double);
	void runLogicUnits(int);
	void executeDeferedActions();
	void removeLogicUnitFromArray(LogicUnit *logicUnit, QVector<LogicUnit *>*);
	void addDeferredAction(DeferredAction *deferredAction);

	TankPhysicsScene *physicsScene;
	QVector<PostPhysicsController*> postPhysicsControllers;
	QVector<PhysicsInterpolator*> physicsInterpolators;
	QVector<DeferredAction*> deferredActions;
	QVector<LogicUnit*> logicUnits;
	QVector<PhysicsController*> physicsControllers;
	int lastRunLogicTime;
	int time;
	bool logicInProgress;
	bool physicsInProgress;
};

#endif // BATTLERUNNER_H
