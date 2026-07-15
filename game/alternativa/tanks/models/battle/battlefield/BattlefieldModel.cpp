#include "BattlefieldModel.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "game/alternativa/tanks/battle/scene3d/BattleScene3D.h"
#include "game/alternativa/tanks/models/battle/battlefield/map/MapBuilder.h"
#include "game/alternativa/tanks/camera/FollowCameraController.h"
#include "hardware/alternativa/engine3d/core/Object3DContainer.h"
#include "game/alternativa/tanks/battle/BattleView.h"
#include "path.h"
#include "getBattleRunner.h"
#include "hardware/alternativa/engine3d/containers/KDContainer.h"
#include "game/alternativa/tanks/models/map/IBattleMap.h"
#include "game/alternativa/tanks/models/map/BattleMapModel.h"
#include "entrance/platform/client/fp10/core/resource/ResourceInfo.h"
#include "game/projects/tanks/clients/flash/resources/resource/MapResource.h"
#include "game/alternativa/tanks/utils/objectpool/ObjectPool.h"
#include "game/alternativa/tanks/engine3d/TextureMaterialRegistryBase.h"
#include "file.h"
#include "flash/display/BitmapData.h"
#include "game/alternativa/tanks/engine3d/DefaultEffectsMaterialRegistry.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/materialfactory/DefaultTankSkinMaterialsFactory.h"
#include "game/alternativa/tanks/models/weapon/twins/TwinsSFXModel.h"
#include "game/alternativa/tanks/models/weapon/twins/TwinsAmmunition.h"
#include "game/alternativa/tanks/models/weapons/shell/Shell.h"
#include "game/alternativa/tanks/models/weapons/shell/states/ShellStates.h"
#include "game/alternativa/tanks/sfx/DecalEffect.h"
#include "game/alternativa/tanks/battle/scene3d/DecalFactory.h"
#include "game/alternativa/tanks/physics/TanksCollisionDetector.h"
#include "ResourcesData.h"
#include <QDebug>


BattleService *battleService;



BattlefieldModel::BattlefieldModel()
{
	battleService = this;
	this->battleRunner = nullptr;
	this->battleScene = nullptr;
	this->followCameraController = nullptr;

	this->objectPool = new ObjectPool;

	//materialRegistry = new TextureMaterialRegistryBase(materialFactory);

	DefaultEffectsMaterialRegistry *defaultEffectsMaterialRegistry = new DefaultEffectsMaterialRegistry;
	TextureMaterialRegistryBase *textureMaterialRegistryBase = defaultEffectsMaterialRegistry;


	materialRegistry = textureMaterialRegistryBase;

	DefaultTankSkinMaterialsFactory::textureMaterialRegistry = materialRegistry;

	TwinsSFXModel::battleService = this;
	TwinsAmmunition::battleService = this;
	Shell::battleService = this;
	ShellStates::battleService = this;
	DecalEffect::battleService = this;

	{
		BattleMapModel *battleMap = new BattleMapModel;

		ResourceInfo *resourceInfo = new ResourceInfo(7, Long(1,1), Long(1,1), false);
		battleMap->mapResource = new MapResource(resourceInfo);

		this->battleMap = battleMap;

		QByteArray data = ResourcesData::getMapPropLibs();

		battleMap->mapResource->load(data);
	}
}


void BattlefieldModel::initBattleCore()
{
	//this->battleScene = new BattleScene3D(display.stage,materialRegistry,getInitParam().shadowMapCorrectionFactor);
	this->battleScene = new BattleScene3D();

	//BattleRunner(_loc4_,this.battleData.ambientSound,battleEventDispatcher);

	this->battleView = new BattleView();
	this->battleRunner = new BattleRunner(1000, nullptr, nullptr);
	this->followCameraController = new FollowCameraController();

	::setBattleRunner(this->battleRunner);

	{
		QByteArray data = File::read(path::getResources() + "/logo/logo.png");

		if (!data.isEmpty())
		{
			std::shared_ptr<BitmapData> bitmapData = std::make_shared<BitmapData>();
			bitmapData->loadFromData(data);
			setBillboardImage(bitmapData);
		}
	}

	//OSGi.getInstance().registerService(BattleService, this);
	//this.battleScene = new BattleScene3D(display.stage, materialRegistry, getInitParam().shadowMapCorrectionFactor);
	//memoryLeakTrackerService.track(this.battleScene, this.battleCounter.toString());
	//var _local_1:IBattleMap = this.getBattleMap();
	//var _local_2:FogParams = _local_1.getFogParams();
	//var _local_3:DynamicShadowParams = _local_1.getDynamicShadowParams();
	//this.battleScene.setupFog(_local_2.color, _local_2.alpha, _local_2.nearLimit, _local_2.farLimit);
	//this.battleScene.setupDynamicShadows(_local_3.lightColor, _local_3.shadowColor, _local_3.angleX, _local_3.angleZ);
	//this.battleView = new BattleView();
	//memoryLeakTrackerService.track(this.battleView, this.battleCounter.toString());
	//var _local_4:Number = _local_1.getGravity();
	//this.battleRunner = new BattleRunner(_local_4, this.battleData.ambientSound, battleEventDispatcher);
	//memoryLeakTrackerService.track(this.battleRunner, this.battleCounter.toString());

	this->battleScene->setDecalFactory(new DecalFactory(this->battleRunner->getCollisionDetector()));

	//this.battleScene.initDustEngine(this, _local_1.getDustParams());
	//this.battleScene.setSSAOColor(_local_1.getSSAOColor());
	//this.frameRateController = new StageFrameRateController(display.stage, this.battleRunner, this.fullTimeStatistics);
	//this.frameRateController.setAdaptiveFrameRate(settings.adaptiveFPS);
	//this.battleView.addToScreen(battleGuiService.getViewportContainer());
	//this.debugPanel = new DebugPanel();
	//this.debugPanel.visible = false;
	//display.stage.addChild(this.debugPanel);
	//this.followCameraController = new FollowCameraController();
	//this.flyCameraController = new FlyCameraController(getInitParam().respawnDuration);
	//putData(DialogWindowSupport, new DialogWindowSupport());

}


BattleScene3D* BattlefieldModel::getBattleScene3D()
{
	return battleScene;
}


BattleRunner* BattlefieldModel::getBattleRunner()
{
	return this->battleRunner;
}


CommonTargetEvaluator *BattlefieldModel::getCommonTargetEvaluator()
{
	return commonTargetEvaluator;
}


void BattlefieldModel::setCommonTargetEvaluator(CommonTargetEvaluator *commonTargetEvaluator)
{
   this->commonTargetEvaluator = commonTargetEvaluator;
}


void BattlefieldModel::tick()
{
	//this->fullTimeStatistics.update();
	this->battleRunner->tick();
}


void BattlefieldModel::onEnterFrame()
{
	this->tick();
}


void BattlefieldModel::initBattleMap()
{

	IBattleMap *battleMap = this->getBattleMap();
	MapResource *mapResource = battleMap->getMapResource();

	this->mapBuilder = new MapBuilder(materialRegistry, mapResource->libRegistry);
	//_loc4_.build(_loc3_,getFunctionWrapper(this.onMapBuildingComplete));
	//putData(MapBuilder,_loc4_);

	QByteArray data = ResourcesData::getMap();

	//QByteArray data = File::read(path::get() + "/map.xml");
	mapBuilder->build(data);

	this->onMapBuildingComplete();
}


void BattlefieldModel::objectLoaded()
{
	initBattleCore();
	initBattleMap();
}


MyList<CollisionShape*>* BattlefieldModel::mapStaticChildren()
{
	return battleRunner->mapStaticChildren();
}


void BattlefieldModel::setCameraController(CameraController *cameraController)
{
	this->battleScene->setCameraController(cameraController);
}


void BattlefieldModel::activateFollowCamera()
{
	this->setCameraController(this->followCameraController);
}


void BattlefieldModel::onMapBuildingComplete()
{
	//var _loc1_:MapBuilder = MapBuilder(getData(MapBuilder));
	this->followCameraController->setCollisionObject(mapBuilder->getMapContainer());
	//this->fixHardwareKeyboardInputBug();
	//battleReadinessService.unlockMap();
	//battleEventDispatcher.dispatchEvent(new MapBuildingCompleteEvent());
}


void BattlefieldModel::setFollowCameraTarget(CameraTarget *cameraTarget)
{
	this->followCameraController->setTarget(cameraTarget);
}


void BattlefieldModel::setCameraPosition()
{
	this->followCameraController->setCameraPosition();
}


ObjectPool* BattlefieldModel::getObjectPool()
{
	return this->objectPool;
}


void BattlefieldModel::setFollowCameraState(const Vector3 *position, const Vector3 *direction)
{
	this->followCameraController->setCurrentState(position, direction);
}


GameActionListener* BattlefieldModel::getCameraAction()
{
	return this->followCameraController;
}


void BattlefieldModel::lockFollowCamera()
{
	this->followCameraController->setLocked(true);
}

void BattlefieldModel::unlockFollowCamera()
{
	this->followCameraController->setLocked(false);
}


BattleView* BattlefieldModel::getBattleView()
{
	return this->battleView;
}


IBattleMap* BattlefieldModel::getBattleMap()
{
	//return (IBattleMap(getInitParam().map.adapt(IBattleMap)));
	return battleMap;
}


int BattlefieldModel::getPhysicsTime()
{
	 return this->battleRunner->getPhysicsTime();
}


void BattlefieldModel::setBillboardImage(std::shared_ptr<BitmapData> bitmapData)
{
	this->battleScene->setBillboardImage(bitmapData);
}


void BattlefieldModel::addGraphicEffect(GraphicEffect *effect)
{
	this->battleScene->addGraphicEffect(effect);
}
