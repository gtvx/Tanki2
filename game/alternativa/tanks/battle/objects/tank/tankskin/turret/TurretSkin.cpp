#include "TurretSkin.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "game/projects/tanks/clients/flash/resources/resource/Tanks3DSResource.h"
#include "game/projects/tanks/clients/flash/resources/tanks/Tank3D.h"
#include "hardware/alternativa/engine3d/core/Object3DContainer.h"
#include "game/alternativa/tanks/battle/scene3d/Object3DNames.h"
#include "game/alternativa/math/Vector3.h"
#include "game/projects/tanks/clients/flash/resources/tanks/Tank3D.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankHullSkinCacheItem.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "Object3D_isType.h"
#include <QMap>


static void setPosition(Object3D *param1, const Vector3 *param2)
{
   param1->x = param2->getX();
   param1->y = param2->getY();
   param1->x = param2->getZ();
}


static void copyPosition(Object3D *param1, const Object3D *param2)
{
   param1->x = param2->x;
   param1->y = param2->y;
   param1->z = param2->z;
}


TurretSkin::TurretSkin(Tanks3DSResource *param1)
{
	for (int i = 0; i < param1->objects.count(); i++)
	{
		std::shared_ptr<Object3D> object = param1->objects.at(i);

		if (Object3D_isType<Mesh>(object.get()) && !Tank3D::EXCLUDED_test(object->name))
		{
			this->turretMeshes.append(std::dynamic_pointer_cast<Mesh>(object));
		}
	}

	QMap<std::shared_ptr<Mesh>, std::shared_ptr<Object3DContainer>> map;

	int index = this->turretMeshes.count() - 1;
	while (index >= 0)
	{
		std::shared_ptr<Mesh> mesh = this->turretMeshes.at(index);
		std::shared_ptr<Mesh> clone_mesh = Tank3D::cloneMesh(mesh);
		clone_mesh->mouseEnabled = false;

		if (map.contains(mesh))
		{
			std::shared_ptr<Object3DContainer> container = map[mesh];
			copyPosition(container.get(), clone_mesh.get());
			container->addChild(clone_mesh);
			setPosition(clone_mesh.get(), Vector3::ZERO);
		}

		std::shared_ptr<Mesh> parent = std::dynamic_pointer_cast<Mesh>(param1->parents[param1->objects.indexOf(mesh)]);
		if (parent != nullptr)
		{
			std::shared_ptr<Object3DContainer> container;

			if (map.contains(parent)) {
				container = map[parent];
			} else {
				container = new_Object3DContainer();
				map[parent] = container;
			}

			if (map.contains(mesh))
				container->addChild(map[mesh]);
			else
				container->addChild(clone_mesh);

			container->mouseEnabled = false;
		}
		int m = this->turretMeshes.indexOf(mesh);
		this->turretMeshes.replace(m, clone_mesh);
		index--;
	}


	std::shared_ptr<Object3D> p1 = this->turretMeshes.at(0)->getParent();
	std::shared_ptr<Object3D> p2 = this->turretMeshes.at(0);
	this->rootObject = p1 != nullptr ? p1 : p2;
	this->rootObject->name = Object3DNames::TANK_PART;
	this->rootObject->mouseEnabled = true;
}


std::shared_ptr<Object3D> TurretSkin::getTurret3D()
{
	return this->rootObject;
}


std::shared_ptr<Object3D> TurretSkin::getBarrel3D()
{
   return this->rootObject;
}


void TurretSkin::setColorTransform(ColorTransform*)
{
	/*
	var _loc2_:Mesh = null;
	for each(_loc2_ in this->getMeshes())
	{
		_loc2_.colorTransform = param1;
	}
	*/
}


void TurretSkin::updateTurretTransform(Matrix4 *hullMatrix, TankHullSkinCacheItem *param2, double turning_position, double)
{
	Matrix4 turretMatrix;
	turretMatrix.setMatrix(param2->getTurretMountPointX(), //x
						   param2->getTurretMountPointY(), //y
						   param2->getTurretMountPointZ() + 1, //z
						   0, //rotationX
						   0, //rotationY
						   turning_position); //rotationZ

	turretMatrix.append(hullMatrix);

	this->rootObject->x = turretMatrix.x_w;
	this->rootObject->y = turretMatrix.y_w;
	this->rootObject->z = turretMatrix.z_w;

	Vector3 rotation;
	turretMatrix.getEulerAngles(&rotation);
	this->rootObject->rotationX = rotation.getX();
	this->rootObject->rotationY = rotation.getY();
	this->rootObject->rotationZ = rotation.getZ();
}


void TurretSkin::setMaterials(std::shared_ptr<TextureMaterial> material)
{
	for (std::shared_ptr<Mesh> item : turretMeshes)
	{
		item->setMaterialToAllFaces(material);
	}
}
