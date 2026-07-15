#include "TankSkin.h"
#include "game/alternativa/tanks/models/tank/TankPartsRegistry.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "game/projects/tanks/clients/flash/resources/object3ds/IObject3DS.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/turret/TurretSkinCacheItem.h"
#include "game/alternativa/tanks/battle/scene3d/BattleScene3D.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "game/alternativa/tanks/battle/scene3d/Object3DNames.h"
#include "TankHullSkinCacheItem.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/turret/TurretSkin.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/turret/CustomTurretSkin.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/materialfactory/DefaultTankSkinMaterialsFactory.h"
#include "entrance/projects/tanks/clients/flash/commons/models/coloring/IColoring.h"
#include "entrance/platform/client/fp10/core/resource/ResourceInfo.h"
#include "entrance/platform/client/fp10/core/resource/types/TextureResource.h"
#include "flash/display/BitmapData.h"
#include "SkinMaterials.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "TrackSkin.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "hardware/alternativa/engine3d/core/Wrapper.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include <QDebug>
#include "entrance/platform/client/fp10/core/resource/types/MultiframeTextureResource.h"
#include "file.h"
#include "path.h"


static TankPartsRegistry tankPartsRegistry;
extern BattleService *battleService;


class TankColor : public IColoring
{
	TextureResource *res;
	MultiframeTextureResource *multiframe;

public:
	TankColor()
	{
		ResourceInfo *resourceInfo = new ResourceInfo(10, Long(10, 1), Long(1, 1), false);

		res = new TextureResource(resourceInfo);
		res->data = std::make_shared<BitmapData>(1, 1, false, 0x00FF00);

		multiframe = new MultiframeTextureResource(resourceInfo);

		QByteArray data = File::read(path::getResources() + "/MULTIFRAME_IMAGE/0000016A 68806019 0000016B 9EB89A54/image.tara");

		multiframe->buildFrames(data);
	}


	TextureResource* getColoring() override
	{
		return res;
	}

	MultiframeTextureResource* getAnimatedColoring()
	{
		return multiframe;
	}

	bool isAnimated()
	{
		return true;
	}
};



TankSkin::TankSkin(IGameObject *param1, IGameObject *param2, IGameObject *)
{
	turretSkin = nullptr;

	//this->colorTransform1 = new ColorTransform();
	//this->targetColorTransform = new ColorTransform();
	//this->desiredColorTransform = new ColorTransform();
	//super();
	//var _loc4_:HullCommon = HullCommon(param1.adapt(HullCommon));
	//var _loc5_:TextureResource = _loc4_.getDeadColoring();


	this->tankSkinMaterialFactory = new DefaultTankSkinMaterialsFactory();


	{
		IObject3DS *_IObject3DS = param1->adapt<IObject3DS>().get();
		Tanks3DSResource *resource = _IObject3DS->getResource3DS();


		this->hullDescriptor = tankPartsRegistry.getHullDescriptor(resource);
	}

	Tanks3DSResource *turret_resource;
	{
		IObject3DS *_IObject3DS = param2->adapt<IObject3DS>().get();
		turret_resource = _IObject3DS->getResource3DS();
		this->turretDescriptor = tankPartsRegistry.getTurretDescriptor(turret_resource);
	}


	this->hullMesh = this->cloneMesh(this->hullDescriptor->mesh);

	if (param2->hasModel<CustomTurretSkin>())
	{
		CustomTurretSkin *customTurretSkin = param2->adapt<CustomTurretSkin>().get();
		this->turretSkin = customTurretSkin->createSkin(turret_resource);
	}
	else
	{
		this->turretSkin = new TurretSkin(turret_resource);
	}

	//this->turretSkin->setColorTransform(this->targetColorTransform);

	this->createTrackSkins(this->hullMesh.get());


	//IColoring *_loc8_ = IColoring(param3.adapt(IColoring));


	IColoring *_loc8_ = new TankColor();
	this->normalMaterials = this->tankSkinMaterialFactory->createSkinMaterials(this, _loc8_);
	//this->deadMaterials = this->tankSkinMaterialFactory.createDeadSkinMaterials(this, _loc5_);
	//this->heatColorTransform = colorTransformService.getHeatColorAdjust();
	//this->frostColorTransform = colorTransformService.getFrostColorAdjust();
	//this->createAmbientShadow();

	setNormalState();
}


TankHullSkinCacheItem* TankSkin::getHullDescriptor()
{
	return this->hullDescriptor;
}


std::shared_ptr<Mesh> TankSkin::getHullMesh()
{
	return this->hullMesh;
}


QVector<TurretGeometryItem*>* TankSkin::getTurretGeometry()
{
	return this->turretDescriptor->getGeometry();
}


void TankSkin::addToScene()
{
	BattleScene3D *battleScene3D = battleService->getBattleScene3D();
	battleScene3D->addObject(this->hullMesh);
	battleScene3D->addTurret(this->turretSkin);
	//battleScene3D->addAmbientShadow(this->ambientShadow);
}


std::shared_ptr<Mesh> TankSkin::cloneMesh(std::shared_ptr<Mesh> src)
{
	std::shared_ptr<Mesh> m = std::dynamic_pointer_cast<Mesh>(src->clone());
	m->name = Object3DNames::TANK_PART;
	//m->colorTransform = this->targetColorTransform;
	//m->shadowMapAlphaThreshold = 0.1;
	//m->calculateVerticesNormalsBySmoothingGroups(0.01);
	return m;
}


void TankSkin::updateHullTransform(const Vector3 *param1, const Vector3 *param2)
{
	if (this->hullDescriptor != nullptr)
	{
		this->hullMesh->x = param1->getX();
		this->hullMesh->y = param1->getY();
		this->hullMesh->z = param1->getZ();
		this->hullMesh->rotationX = param2->getX();
		this->hullMesh->rotationY = param2->getY();
		this->hullMesh->rotationZ = param2->getZ();
	}
}


void TankSkin::setNormalState()
{
	this->hullMesh->setMaterialToAllFaces(this->normalMaterials->hullMaterial);
	this->turretSkin->setMaterials(this->normalMaterials->turretMaterial);
	this->setTracksMaterial(this->normalMaterials->leftTrackMaterial, this->normalMaterials->rightTrackMaterial);
}


void TankSkin::setTracksMaterial(std::shared_ptr<Material> left, std::shared_ptr<Material> right)
{
   this->leftTrackSkin->setMaterial(left);
   this->rightTrackSkin->setMaterial(right);
}


void TankSkin::updateTracks(double leftValue, double rightValue)
{
	this->leftTrackSkin->move(leftValue);
	this->rightTrackSkin->move(rightValue);
}


void TankSkin::createTrackSkins(Mesh *mesh)
{
	this->leftTrackSkin = new TrackSkin();
	this->rightTrackSkin = new TrackSkin();

	QVector<Shared<Face>> faces = mesh->getFaces();

	for (Shared<Face> face : faces)
	{
		if (face->material->name == "tracks")
		{
			this->addFaceToTrackSkin(face);
		}
	}

	this->leftTrackSkin->init();
	this->rightTrackSkin->init();

}


void TankSkin::addFaceToTrackSkin(Shared<Face> face)
{
	Vertex *vertex = face->wrapper->vertex.get();
	if (vertex->x < 0)
	{
		this->leftTrackSkin->addFace(face);
	}
	else
	{
		this->rightTrackSkin->addFace(face);
	}
}


std::shared_ptr<Object3D> TankSkin::getTurret3D()
{
   return this->turretSkin->getTurret3D();
}


std::shared_ptr<Object3D> TankSkin::getBarrel3D()
{
	return this->turretSkin->getBarrel3D();
}


TurretSkinCacheItem* TankSkin::getTurretDescriptor()
{
   return this->turretDescriptor;
}


void TankSkin::updateTurretTransform(double param1, double param2)
{
	if (this->hullDescriptor != nullptr && this->turretDescriptor != nullptr)
	{
		Matrix4 hullMatrix;
		hullMatrix.setMatrix(this->hullMesh->x,
							 this->hullMesh->y,
							 this->hullMesh->z,
							 this->hullMesh->rotationX,
							 this->hullMesh->rotationY,
							 this->hullMesh->rotationZ);
		this->turretSkin->updateTurretTransform(&hullMatrix, this->hullDescriptor, param1, param2);
	}
}


void TankSkin::removeFromScene()
{
	BattleScene3D *battleScene3D = battleService->getBattleScene3D();
	battleScene3D->removeObject(this->hullMesh);
	battleScene3D->removeTurret(this->turretSkin);
	//battleScene3D->removeAmbientShadow(this->ambientShadow);
}
