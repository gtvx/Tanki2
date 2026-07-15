#include "BattleScene3D.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/turret/TurretSkin.h"
#include "game/alternativa/tanks/battle/scene3d/scene3dcontainer/Object3DContainerProxy.h"
#include "game/alternativa/tanks/camera/DummyCameraController.h"
#include "game/alternativa/tanks/camera/GameCamera.h"
#include "hardware/alternativa/engine3d/core/Object3DContainer.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/sfx/GraphicEffect.h"
#include "game/alternativa/tanks/models/battle/battlefield/map/Billboards.h"
#include "hardware/alternativa/engine3d/core/Camera3D.h"
#include "hardware/alternativa/engine3d/objects/Decal.h"
#include "vulkan/VulkanWindow.h"
#include "MyMath.h"
#include "RenderGroup.h"
#include "Error.h"
#include "Renderer.h"
#include "DecalFactory.h"
#include "Object3D_isType.h"
#include "RotationState.h"
#include "hardware/alternativa/engine3d/containers/KDContainer.h"
#include "qhash_shared_ptr.h"
#include <QVector>
#include <QDebug>


BattleScene3D::BattleScene3D()
{
	this->mapContainerProxy = new Object3DContainerProxy();
	this->cameraController = DummyCameraController::INSTANCE;

	this->billboards = new Billboards;

	this->vulkanWindow = nullptr;

	//hidableGraphicObjects = new HidableGraphicObjects();

	renderGroups[0] = new RenderGroup();
	renderGroups[1] = new RenderGroup();


	this->decalFactory = nullptr;

	this->numEffects = 0;

	this->initContainers();
	this->createCamera();

	this->enableRendering(); //my
}


void BattleScene3D::setDecalFactory(DecalFactory *decalFactory)
{
	this->decalFactory = decalFactory;
}


void BattleScene3D::addObject(std::shared_ptr<Object3D> object3d)
{
	this->mapContainerProxy->addChild(object3d);

	//if (object3d->name != Object3DNames::STATIC && object3d->name != Object3DNames::TANK_PART)
	//{
	//  this->shaftRaycastExcludedObjects[object3d] = true;
	//}
}


void BattleScene3D::removeObject(std::shared_ptr<Object3D> object3D)
{
	this->mapContainerProxy->removeChild(object3D);

	//if (((!(_arg_1.name == Object3DNames.STATIC)) && (!(_arg_1.name == Object3DNames.TANK_PART))))
	//{
	//	delete this->shaftRaycastExcludedObjects[_arg_1];
	//}
}


void BattleScene3D::addTurret(TurretSkin *turretSkin)
{
	this->addObject(turretSkin->getTurret3D());
}


void BattleScene3D::addAmbientShadow(Shadow*)
{
	//this->ambientShadows.add(shadow);
}


void BattleScene3D::addTank(Tank *tank)
{
	tanks.append(tank);
}


void BattleScene3D::render(int param1, int param2)
{
	if (this->renderingEnabled)
	{
		//this->fpsCounter.update();
		//this->ambientShadows.adjustDistances(this->fpsCounter.getFPS()); //окружающие тени
		//this->dustEngine.update(); //Пыль

		this->renderTanks(param1, param2);
		this->cameraController->update(this->camera.get(), param1, param2);

		this->camera->calculateAdditionalData();

		this->runRenderers(param1, param2);

		this->updateEffects(param2);

		//if (this->userTitleRenderer != null)
		//{
		//	this->userTitleRenderer.renderUserTitles();
		//}


		//_local_3 = ((this->camera.shadowMap != null) ? this->camera.shadowMapStrength : 0);

		//if (this->camera.directionalLight != null)
		//{
		//	this->camera.directionalLight.intensity = (this->shadowMapCorrectionFactor + (_local_3 * (1 - this->shadowMapCorrectionFactor)));
		//}

		//this->camera.render();

		if (vulkanWindow != nullptr)
		{
			if (vulkanWindow->isRenderEnable())
			{
				this->camera->renderVulkan(vulkanWindow);
			}
		}
	}
}


void BattleScene3D::createCamera()
{
	this->camera = new_GameCamera();
	//this->camera->ssaoRadius = 400;
	//this->camera->ssaoRange = 20 * 60;
	//this->camera->ssaoColor = 0;
	//this->camera->ssaoAlpha = 1.4;
	//this->camera->addToDebug(Debug.BOUNDS, Object3D);
	//this->camera->addToDebug(Debug.EDGES,Object3D);
	this->rootContainer->addChild(this->camera);
}


void BattleScene3D::initContainers()
{
	this->rootContainer = new_Object3DContainer();

	this->skyboxContainer = new_Object3DContainer();
	this->mainContainer = new_Object3DContainer();
	this->frontContainer = new_Object3DContainer();

	this->rootContainer->addChild(this->skyboxContainer);
	this->rootContainer->addChild(this->mainContainer);
	this->rootContainer->addChild(this->frontContainer);

	this->frontContainerProxy = new Object3DContainerProxy(this->frontContainer);
}


void BattleScene3D::setCameraController(CameraController *cameraController)
{
	if (this->cameraController != cameraController)
	{
		if (this->cameraController != nullptr)
		{
			this->cameraController->deactivate();
		}
		this->cameraController = cameraController == nullptr ? DummyCameraController::INSTANCE : cameraController;
		this->cameraController->activate(this->camera.get());
	}
}


GameCamera* BattleScene3D::getCamera()
{
	return camera.get();
}


void BattleScene3D::renderTanks(int _arg_1, int _arg_2)
{
	for (Tank *tank : tanks)
	{
		tank->render(_arg_1, _arg_2);
	}
}


void BattleScene3D::enableRendering()
{
	this->renderingEnabled = true;
}


void BattleScene3D::disableRendering()
{
	this->renderingEnabled = false;
}

/*
void BattleScene3D::removeObjectFromExclusion(Object3D *object3D)
{
	delete this->excludedObjects[object3D];
}
*/

void BattleScene3D::removeTurret(TurretSkin *turretSkin)
{
	this->removeObject(turretSkin->getTurret3D());
}


void BattleScene3D::removeTank(Tank *tank)
{
	tanks.removeOne(tank);
}


void BattleScene3D::setMapContainer(std::shared_ptr<Object3DContainer> container)
{
	if (this->mapContainer != nullptr)
	{
		this->mainContainer->removeChild(this->mapContainer);
		this->mapContainer = nullptr;
	}

	this->mapContainerProxy->setContainer(container);
	this->mapContainer = container;

	if (this->mapContainer != nullptr)
	{
		this->mainContainer->addChild(this->mapContainer);
		this->camera->farClipping = 1.5 * this->getBoundSphereDiameter(this->mapContainer.get());
	}
}


double BattleScene3D::getBoundSphereDiameter(Object3D *object)
{
	double X = object->bound.boundMaxX - object->bound.boundMinX;
	double Y = object->bound.boundMaxY - object->bound.boundMinY;
	double Z = object->bound.boundMaxZ - object->bound.boundMinZ;
	return MyMath::sqrt((X * X) + (Y * Y) + (Z * Z));
}


void BattleScene3D::addGraphicEffect(GraphicEffect *effect)
{
	if (effect == nullptr)
	{
		throw ArgumentError("BattleScene3D::addGraphicEffect");
	}

	int index = this->numEffects++;

	if (this->effects.length() <= this->numEffects)
		this->effects.resize(this->numEffects);

	this->effects[index] = effect;

	effect->addedToScene(this->mapContainerProxy);
}


void BattleScene3D::addBillboard(std::shared_ptr<Mesh> &mesh)
{
	this->billboards->add(mesh);
}


HidableGraphicObjects* BattleScene3D::getHidableGraphicObjects()
{
	return hidableGraphicObjects;
}


void BattleScene3D::setRenderVulkan(VulkanWindow *vulkanWindow)
{
	this->vulkanWindow = vulkanWindow;
}


void BattleScene3D::setBillboardImage(std::shared_ptr<BitmapData> bitmapData)
{
	this->billboards->setImage(bitmapData);
}


void BattleScene3D::addRenderer(Renderer *renderer, int index)
{
	//qDebug() << "BattleScene3D::addRenderer" << typeid(*renderer).name() << index;
	this->renderGroups[index]->addRenderer(renderer);
}


void BattleScene3D::removeRenderer(Renderer *renderer, int index)
{
	//qDebug() << "BattleScene3D::removeRenderer" << typeid(*renderer).name() << index;

	this->renderGroups[index]->removeRenderer(renderer);
}


void BattleScene3D::runRenderers(int _arg_1, int _arg_2)
{
	for (int i = 0; i < 2; i++)
		renderGroups[i]->render(_arg_1, _arg_2);
}


void BattleScene3D::updateEffects(int _arg_1)
{
	int i = 0;
	while (i < this->numEffects)
	{
		GraphicEffect *_local_3 = this->effects[i];

		//qDebug() << typeid(*_local_3).name();

		if ((!(_local_3->play(_arg_1, this->camera.get()))))
		{
			_local_3->destroy();
			int _local_4 = i--;
			this->effects[_local_4] = this->effects[--this->numEffects];
			this->effects[this->numEffects] = nullptr;
		}
		i++;
	}
}


std::shared_ptr<Decal> BattleScene3D::addPermanentDecal(const Vector3 *position,
										const Vector3 *projectionOrigin,
										double radius,
										std::shared_ptr<TextureMaterial> material)
{
	return this->createDecal(position, projectionOrigin, radius, material);
}


std::shared_ptr<Decal> BattleScene3D::createDecal(const Vector3 *position,
								  const Vector3 *projectionOrigin,
								  double radius,
								  std::shared_ptr<TextureMaterial> material,
								  RotationState const *rotationState)
{
	if (rotationState == nullptr)
	{
		rotationState = RotationState::USE_RANDOM_ROTATION;
	}

	if (Object3D_isType<KDContainer>(this->mapContainer.get()))
	{
		auto decal = this->decalFactory->createDecal(position,
													 projectionOrigin,
													 radius,
													 material,
													 dynamic_cast<KDContainer*>(this->mapContainer.get()),
													 rotationState);

		this->mapContainerProxy->addChildAt(decal, 0);
		this->allDecals.insert(decal);
		this->addObjectToExclusion(decal);
		//this->shaftRaycastExcludedObjects[decal] = true;
		return decal;
	}

	return nullptr;
}


void BattleScene3D::removeDecal(std::shared_ptr<Decal> decal)
{
	if (decal != nullptr)
	{
		this->mapContainerProxy->removeChild(decal);
		this->removeObjectFromExclusion(decal);
		//delete this.shaftRaycastExcludedObjects[decal];
		//delete this.allDecals[decal];
	}
}


void BattleScene3D::addObjectToExclusion(std::shared_ptr<Object3D> object)
{
	this->excludedObjects.insert(object);
}


void BattleScene3D::removeObjectFromExclusion(std::shared_ptr<Object3D> object)
{
	this->excludedObjects.remove(object);
}
