#ifndef BATTLEFIELDMODEL_H
#define BATTLEFIELDMODEL_H

#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"

class MapBuilder;
class FollowCameraController;
class CameraController;
class CameraTarget;
class BattleView;
class Canvas;
class IBattleMap;
class TextureMaterialRegistry;
class BitmapData;


class BattlefieldModel : public BattleService
{
	BattleRunner *battleRunner;
	BattleScene3D *battleScene;
	CommonTargetEvaluator *commonTargetEvaluator;
	BattleView *battleView;
	ObjectPool *objectPool;

public:
	BattlefieldModel();

	BattleScene3D* getBattleScene3D() override;
	BattleRunner* getBattleRunner() override;
	CommonTargetEvaluator* getCommonTargetEvaluator() override;

	void onEnterFrame();
	void objectLoaded();
	MyList<CollisionShape *> *mapStaticChildren();
	void setCommonTargetEvaluator(CommonTargetEvaluator *commonTargetEvaluator) override;
	void setCameraController(CameraController *cameraController);
	void activateFollowCamera() override;
	void setFollowCameraTarget(CameraTarget *cameraTarget) override;
	void setFollowCameraState(const Vector3*, const Vector3*) override;
	GameActionListener* getCameraAction() override;
	void lockFollowCamera() override;
	void unlockFollowCamera() override;
	BattleView* getBattleView() override;
	void setCameraPosition() override;
	ObjectPool* getObjectPool() override;
	void setBillboardImage(BitmapData *bitmapData);

private:
	void tick();
	void initBattleCore();
	void initBattleMap();
	void onMapBuildingComplete();
	IBattleMap* getBattleMap();
	int getPhysicsTime() override;
	void addGraphicEffect(GraphicEffect *effect) override;

	MapBuilder *mapBuilder;
	IBattleMap *battleMap;
	TextureMaterialRegistry *materialRegistry;
	FollowCameraController *followCameraController;
};

#endif // BATTLEFIELDMODEL_H
