#include "PlaneMuzzleFlashEffect.h"
#include "game/alternativa/tanks/sfx/SimplePlane.h"
#include "game/alternativa/math/Vector3.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "game/alternativa/tanks/battle/scene3d/scene3dcontainer/Scene3DContainer.h"
#include "game/alternativa/tanks/camera/GameCamera.h"
#include "SFXUtils.h"
#include <QDebug>


PlaneMuzzleFlashEffect::PlaneMuzzleFlashEffect(Pool *pool) :
	PooledObject(pool)
{
	localMuzzlePosition = new Vector3;

	this->plane = new_SimplePlane(1, 1, 0.5, 0);
	this->plane->setUVs(0, 0, 0, 1, 1, 1, 1, 0);
	this->plane->shadowMapAlphaThreshold = 2;
	this->plane->depthMapAlphaThreshold = 2;
	//this->plane->useShadowMap = false;
	this->plane->useLight = false;
}


void PlaneMuzzleFlashEffect::init(const Vector3 *position,
								  std::shared_ptr<Object3D> turret,
								  std::shared_ptr<TextureMaterial> material,
								  int timetoLive,
								  double width,
								  double length)
{
	this->localMuzzlePosition->copy(position);
	this->turret = turret;
	this->timetoLive = timetoLive;
	this->plane->setMaterialToAllFaces(material);
	this->plane->setWidth(width);
	this->plane->setLength(length);
}


bool PlaneMuzzleFlashEffect::play(int time, GameCamera *camera)
{
	if (this->timetoLive < 0)
	{
		return false;
	}

	this->timetoLive = this->timetoLive - time;

	Matrix4 turretMatrix;
	Vector3 globalMuzzlePosition;
	Vector3 gunDirection;


	turretMatrix.setMatrix(this->turret->x,
						   this->turret->y,
						   this->turret->z,
						   this->turret->rotationX,
						   this->turret->rotationY,
						   this->turret->rotationZ);



	turretMatrix.transformVector(this->localMuzzlePosition, &globalMuzzlePosition);

	turretMatrix.getAxis(1, &gunDirection);

	SFXUtils::alignObjectPlaneToView(this->plane.get(), &globalMuzzlePosition, &gunDirection, &camera->position);

	return true;
}


void PlaneMuzzleFlashEffect::destroy()
{
	this->container->removeChild(this->plane);
	this->container = nullptr;
	this->turret = nullptr;
	recycle();
}


void PlaneMuzzleFlashEffect::kill()
{
	this->timetoLive = -1;
}


void PlaneMuzzleFlashEffect::addedToScene(Scene3DContainer *container)
{
	this->container = container;
	container->addChild(this->plane);
}
