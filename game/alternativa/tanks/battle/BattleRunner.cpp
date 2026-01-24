#include "BattleRunner.h"
#include "getTimer.h"
#include "BattleService.h"
#include "PhysicsInterpolator.h"
#include "game/alternativa/tanks/physics/TankPhysicsScene.h"
#include "game/alternativa/tanks/models/tank/LocalTankInfoService.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/physics/TanksCollisionDetector.h"
#include "game/alternativa/physics/collision/CollisionKdTree.h"
#include "game/alternativa/tanks/utils/EncryptedIntImpl.h"
#include "game/alternativa/tanks/battle/scene3d/BattleScene3D.h"
#include "game/alternativa/tanks/battle/objects/tank/controllers/LocalChassisController.h"
#include "game/alternativa/tanks/battle/PostPhysicsController.h"
#include "game/alternativa/tanks/battle/BattleView.h"
#include "game/alternativa/tanks/battle/PerformanceMonitor.h"
#include "FPSService.h"
#include "LogicUnit.h"
#include "game/alternativa/tanks/physics/TankBodyIdProvider.h"
#include "game/alternativa/tanks/services/tankregistry/TankUsersRegistry.h"
#include "game/alternativa/math/Matrix3.h"
#include "game/alternativa/physics/Body.h"
#include "global_class.h"
#include "game/alternativa/tanks/services/tankregistry/TankUsersRegistryServiceImpl.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "game/alternativa/tanks/physics/TankBody.h"
#include "DeferredAction.h"
#include "utils/PhysicsControllerDeferredAction.h"
#include "utils/AddPostPhysicsControllerAction.h"
#include "utils/RemovePostPhysicsController.h"
#include <QDebug>


const int BattleRunner::PHYSICS_STEP_IN_MS = 33;
const double BattleRunner::PHYSICS_STEP_IN_S = 0.033;
extern BattleService *battleService;
static const EncryptedIntImpl thousand(1000);

LocalTankInfoService *BattleRunner::localTankInfoService;




BattleRunner::BattleRunner(double gravity, Sound *, BattleEventDispatcher *param3)
{
	this->physicsInProgress = false;
	this->logicInProgress = false;
	//this->physicsPerformanceMonitor = new PerformanceMonitor(30);
	this->fpsService = new FPSService();
	this->fpsService->start();
	//this->inputProcessors = new QVector<LogicUnit>();
	//this->logicUnits = new QVector<LogicUnit>();
	//this->physicsControllers = new QVector<PhysicsController>();
	//this->postPhysicsControllers = new QVector<PostPhysicsController>();
	//this->physicsInterpolators = new QVector<PhysicsInterpolator>();
	//this->deferredActions = new QVector<DeferredAction>();
	//this->battleEventDispatcher = param3;

	setTankUsersRegistry(new TankUsersRegistryServiceImpl);

	this->time = getTimer();
	this->physicsScene = new TankPhysicsScene(this->time, gravity, param3);
	//this->soundManager = SoundManager.createSoundManager(param2);
	this->lastRunLogicTime = this->time;
	TankBodyIdProvider::resetIds();
}


void BattleRunner::runPhysics(int param1)
{
	fpsService->onEnterFrame();

	while (this->physicsScene->getPhysicsTime() < this->time)
	{
		this->updateLocalTankPhysicsState();
		this->runLogicUnits(this->getPhysicsTime());

		this->physicsInProgress = true;

		this->runPhysicsControllers(double(param1) / thousand.getInt());

		this->physicsScene->update(param1);


		this->runPostPhysicsControllers(param1 / thousand.getInt());

		//this->_triggers.check(this->localBody);
		this->physicsInProgress = false;

		this->executeDeferedActions();
	}

	this->runPhysicsInterpolators(param1);
}


void BattleRunner::tick()
{
	int time = getTimer();
	int t = time - this->time;
	this->time = time;
	this->runInputProcessors(this->time, t);
	this->runPhysics(PHYSICS_STEP_IN_MS);
	battleService->getBattleScene3D()->render(this->time, t); //camera
	//battleService->getBattleView()->update(canvas, fpsService);
	//this->soundManager->updateSoundEffects(t, battleService.getBattleScene3D().getCamera());
}


void BattleRunner::battleViewUpdate(Canvas *canvas)
{
	battleService->getBattleView()->update(canvas, fpsService);
}


void BattleRunner::runInputProcessors(int, int)
{
	/*
	int count = this->inputProcessors.length;
	int index = 0;
	while (_loc4_ < count)
	{
		this->inputProcessors[index].runLogic(param1, param2);
		index++;
	}
	*/
}


void BattleRunner::addBodyWrapper(TankBody *tankbody)
{
	this->physicsScene->addBody(tankbody);
}


void BattleRunner::initStaticGeometry(MyList<CollisionShape*> *param1)
{
	this->physicsScene->initStaticGeometry(param1);
}


MyList<CollisionShape*>* BattleRunner::mapStaticChildren()
{
	return &physicsScene->getCollisionDetector()->tree->staticChildren;
}


void BattleRunner::updateLocalTankPhysicsState()
{
	if (localTankInfoService->isLocalTankLoaded())
	{
		localTankInfoService->getLocalTank()->updatePhysicsState();
	}
}


TanksCollisionDetector* BattleRunner::getCollisionDetector()
{
   return this->physicsScene->getCollisionDetector();
}


void BattleRunner::runLogicUnits(int t)
{
	int t2 = (t - this->lastRunLogicTime);
	this->lastRunLogicTime = t;
	this->logicInProgress = true;
	int count = this->logicUnits.count();
	int i = 0;
	while (i < count)
	{
		this->logicUnits[i]->runLogic(t, t2);
		i++;
	}
	this->logicInProgress = false;
	this->executeDeferedActions();
}


void BattleRunner::addLogicUnit(LogicUnit *logicUnit)
{
	if (this->logicInProgress)
	{
		qDebug("ERROR addLogicUnit");
		//this->addDeferredAction(new LogicUnitDeferredAction(logicUnit, true));
	}
	else
	{
		if (this->logicUnits.indexOf(logicUnit) < 0)
		{
			this->logicUnits.append(logicUnit);
		}
	}
}



int BattleRunner::getPhysicsTime()
{
	return this->physicsScene->getPhysicsTime();
}


void BattleRunner::removeLogicUnit(LogicUnit *logicUnit)
{
	if (this->logicInProgress) {
		//this->addDeferredAction(new LogicUnitDeferredAction(logicUnit, false));
	} else {
		this->removeLogicUnitFromArray(logicUnit, &this->logicUnits);
	}
}


void BattleRunner::removeLogicUnitFromArray(LogicUnit *logicUnit, QVector<LogicUnit*> *v)
{
	int i = v->indexOf(logicUnit);
	if (i >= 0) {
		int a = v->count() - 1;
		v->begin()[i] = v->at(a);
		v->resize(a);
	}
}


void BattleRunner::removeBodyWrapper(TankBody *tankBody)
{
	this->physicsScene->removeBody(tankBody);
	//if (this->localBody == tankBody->body)
		//this->setLocalBody(nullptr);
}


void BattleRunner::addPhysicsController(PhysicsController *controller)
{
	//qDebug() << "BattleRunner::addPhysicsController" << typeid(*controller).name();

	if (this->physicsInProgress)
	{
		this->addDeferredAction(new PhysicsControllerDeferredAction(controller, true));
	}
	else if (this->physicsControllers.indexOf(controller) < 0)
	{
		this->physicsControllers.append(controller);
	}
}


void BattleRunner::removePhysicsController(PhysicsController *controller)
{
	//qDebug() << "BattleRunner::removePhysicsController" << typeid(*controller).name();

	if (this->physicsInProgress) {
		this->addDeferredAction(new PhysicsControllerDeferredAction(controller, false));
	} else {
		int count = this->physicsControllers.count();
		if (count > 0) {
			int index = this->physicsControllers.indexOf(controller);
			if (index >= 0) {
				this->physicsControllers.removeAt(index);
			}
		}
	}
}


void BattleRunner::runPhysicsControllers(double param1)
{
	const int count = this->physicsControllers.count();

	int index = 0;
	while (index < count)
	{
		PhysicsController *controller = this->physicsControllers.at(index);
		//qDebug() << "BattleRunner::runPhysicsControllers" <<typeid(*controller).name();
		controller->runBeforePhysicsUpdate(param1);
		index++;
	}
}
//PhysicsController






//PostPhysicsControllers
void BattleRunner::addPostPhysicsController(PostPhysicsController *controller)
{
	//qDebug() << "BattleRunner::addPostPhysicsController" << typeid(*controller).name();

	if (this->physicsInProgress) {
		this->addDeferredAction(new AddPostPhysicsControllerAction(controller));
	} else {
		if (this->postPhysicsControllers.indexOf(controller) < 0)
			this->postPhysicsControllers.append(controller);
	}
}


void BattleRunner::removePostPhysicsController(PostPhysicsController *controller)
{
	qDebug() << "BattleRunner::removePostPhysicsController" << typeid(*controller).name();

	if (this->physicsInProgress == true) {
		this->addDeferredAction(new RemovePostPhysicsController(controller));
	} else {
		if (this->postPhysicsControllers.count() > 0) {
			int index = this->postPhysicsControllers.indexOf(controller);
			if (index >= 0) {
				this->postPhysicsControllers.removeAt(index);
			}
		}
	}
}


void BattleRunner::runPostPhysicsControllers(double _arg_1)
{
	int count = this->postPhysicsControllers.count();

	int i = 0;
	while (i < count)
	{
		PostPhysicsController *post = this->postPhysicsControllers[i];
		//qDebug() << "BattleRunner::runPostPhysicsControllers" << typeid(*post).name();
		post->runAfterPhysicsUpdate(_arg_1);
		i++;
	}
}
//PostPhysicsControllers








//PhysicsInterpolator
void BattleRunner::addPhysicsInterpolator(PhysicsInterpolator *controller)
{
	//qDebug() << "BattleRunner::addPhysicsInterpolator" << typeid(*controller).name();

	if (this->physicsInterpolators.indexOf(controller) < 0)
	{
		this->physicsInterpolators.append(controller);
	}
}


void BattleRunner::removePhysicsInterpolator(PhysicsInterpolator *controller)
{
	//qDebug() << "BattleRunner::removePhysicsInterpolator" << typeid(*controller).name();

	int count = this->physicsInterpolators.count();
	if (count > 0) {
		int index = this->physicsInterpolators.indexOf(controller);
		if (index >= 0) {
			this->physicsInterpolators[index] = this->physicsInterpolators[--count];
			this->physicsInterpolators.resize(count);
		}
	}
}


void BattleRunner::runPhysicsInterpolators(int param1)
{
	const int count = this->physicsInterpolators.count();

	double v = 1. + double((this->time - this->physicsScene->getPhysicsTime()) / double(PHYSICS_STEP_IN_MS));


	for (int i = 0; i < count; i++)
	{
		PhysicsInterpolator *physicsInterpolator = this->physicsInterpolators[i];
		//qDebug() << "BattleRunner::runPhysicsInterpolators" <<typeid(*physicsInterpolator).name();
		physicsInterpolator->interpolatePhysicsState(v, param1);
	}
}
//PhysicsInterpolator




void BattleRunner::addDeferredAction(DeferredAction *deferredAction)
{
	this->deferredActions.append(deferredAction);
}


void BattleRunner::executeDeferedActions()
{
	for (DeferredAction *item : this->deferredActions)
	{
		item->execute();
		delete item;
	}

	this->deferredActions.clear();
}
