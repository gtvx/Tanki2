#ifndef BATTLESCENE3D_H
#define BATTLESCENE3D_H

#include <memory>
#include <QVector>
#include <QSet>

class Object3DContainerProxy;
class TurretSkin;
class Object3D;
class AmbientShadows;
class Shadow;
class Tank;
class CameraController;
class GameCamera;
class Object3DContainer;
class GraphicEffect;
class Mesh;
class Billboards;
class HidableGraphicObjects;
class VulkanWindow;
class BitmapData;
class Renderer;
class RenderGroup;
class Decal;
class Vector3;
class TextureMaterial;
class RotationState;
class DecalFactory;


class BattleScene3D
{
	Object3DContainerProxy *mapContainerProxy;
	AmbientShadows *ambientShadows;
	QVector<Tank*> tanks;
	CameraController *cameraController;
	std::shared_ptr<GameCamera> camera;
	std::shared_ptr<Object3DContainer> rootContainer;
	Object3DContainerProxy *frontContainerProxy;
	std::shared_ptr<Object3DContainer> frontContainer;
	std::shared_ptr<Object3DContainer> skyboxContainer;
	std::shared_ptr<Object3DContainer> mainContainer;
	std::shared_ptr<Object3DContainer> mapContainer;
	bool renderingEnabled;
	int numEffects;
	QVector<GraphicEffect*> effects;
	Billboards *billboards;
	HidableGraphicObjects *hidableGraphicObjects;
	VulkanWindow *vulkanWindow;

	RenderGroup *renderGroups[2];


	QSet<std::shared_ptr<Decal>> allDecals;

	QSet<std::shared_ptr<Object3D>> excludedObjects;


	DecalFactory *decalFactory;


public:
	BattleScene3D();
	void addObject(std::shared_ptr<Object3D> object3d);
	void addTurret(TurretSkin *turretSkin);
	void addAmbientShadow(Shadow *shadow);
	void addTank(Tank *tank);
	void render(int, int);
	void setCameraController(CameraController*);
	GameCamera *getCamera();
	void enableRendering();
	void disableRendering();
	//void removeObjectFromExclusion(Object3D *);
	void removeTurret(TurretSkin *);
	void removeObject(std::shared_ptr<Object3D>);
	void removeTank(Tank*);
	void setMapContainer(std::shared_ptr<Object3DContainer> container);

	void addGraphicEffect(GraphicEffect *effect);

	void addBillboard(std::shared_ptr<Mesh> &mesh);

	HidableGraphicObjects* getHidableGraphicObjects();

	void setRenderVulkan(VulkanWindow *vulkanWindow);

	void setBillboardImage(BitmapData *bitmapData);


	void addRenderer(Renderer *renderer, int index = 0);
	void removeRenderer(Renderer *renderer, int index = 0);

	std::shared_ptr<Decal> addPermanentDecal(const Vector3 *position,
							 const Vector3 *projectionOrigin,
							 double radius,
							 std::shared_ptr<TextureMaterial> material);

	void removeDecal(std::shared_ptr<Decal> decal);



	void addObjectToExclusion(std::shared_ptr<Object3D> object);
	void removeObjectFromExclusion(std::shared_ptr<Object3D> object);


	void setDecalFactory(DecalFactory *decalFactory);

private:
	void createCamera();
	void initContainers();
	void renderTanks(int, int);
	double getBoundSphereDiameter(Object3D *object);
	void runRenderers(int _arg_1, int _arg_2);
	void updateEffects(int _arg_1);
	std::shared_ptr<Decal> createDecal(const Vector3 *position,
									   const Vector3 *projectionOrigin,
									   double radius, std::shared_ptr<TextureMaterial> material,
									   RotationState const *rotationState = nullptr);
};

#endif // BATTLESCENE3D_H
