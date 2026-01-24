#ifndef BATTLESERVICE_H
#define BATTLESERVICE_H

class BattleRunner;
class BattleScene3D;
class CommonTargetEvaluator;
class CameraTarget;
class Vector3;
class GameActionListener;
class BattleView;
class ObjectPool;
class GraphicEffect;

class BattleService
{
public:	
	virtual ObjectPool* getObjectPool() = 0;
	virtual BattleScene3D* getBattleScene3D() = 0;
	virtual BattleRunner* getBattleRunner() = 0;
	virtual void setCommonTargetEvaluator(CommonTargetEvaluator*) = 0;
	virtual CommonTargetEvaluator* getCommonTargetEvaluator() = 0;
	virtual void activateFollowCamera() = 0;
	virtual void setFollowCameraTarget(CameraTarget*) = 0;
	virtual void setCameraPosition() = 0;
	virtual void setFollowCameraState(const Vector3*, const Vector3*) = 0;
	virtual void lockFollowCamera() = 0;
	virtual void unlockFollowCamera() = 0;
	virtual BattleView* getBattleView() = 0;
	virtual GameActionListener* getCameraAction() = 0;
	virtual int getPhysicsTime() = 0;
	virtual void addGraphicEffect(GraphicEffect*) = 0;
};

#endif // BATTLESERVICE_H
