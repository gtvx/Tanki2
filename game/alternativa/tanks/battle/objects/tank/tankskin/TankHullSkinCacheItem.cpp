#include "TankHullSkinCacheItem.h"
#include <QVector>
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "game/projects/tanks/clients/flash/resources/resource/Tanks3DSResource.h"
#include "game/alternativa/tanks/utils/EncryptedNumberImpl.h"
#include "game/alternativa/tanks/battle/objects/tank/HullNotFoundError.h"
#include "game/alternativa/tanks/battle/objects/tank/TurretMountPointNotFoundError.h"
#include "game/alternativa/math/Vector3.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"


TankHullSkinCacheItem::TankHullSkinCacheItem(Tanks3DSResource *param1) :
	TankSkinPartCacheItem(param1)
{
	QVector<std::shared_ptr<Object3D>> *list = param1->getObjectsByName("/mount/i", "mount");
	if (list == nullptr)
		throw TurretMountPointNotFoundError();

	Object3D *object = list->at(0).get();
	this->turretMountPoint = new Vector3(object->x, object->y, object->z);
	this->encTurretMountPointX = new EncryptedNumberImpl(object->x);
	this->encTurretMountPointY = new EncryptedNumberImpl(object->y);
	this->encTurretMountPointZ = new EncryptedNumberImpl(object->z);
	this->mesh = this->initMesh(this->getMesh(param1));
}


double TankHullSkinCacheItem::getTurretMountPointX()
{
   return this->encTurretMountPointX->getNumber();
}


double TankHullSkinCacheItem::getTurretMountPointY()
{
   return this->encTurretMountPointY->getNumber();
}


double TankHullSkinCacheItem::getTurretMountPointZ()
{
   return this->encTurretMountPointZ->getNumber();
}


std::shared_ptr<Mesh> TankHullSkinCacheItem::getMesh(Tanks3DSResource *param1)
{
	QVector<std::shared_ptr<Object3D>> *vector = param1->getObjectsByName("/hull/i", "hull");
	if (vector == nullptr)
		throw HullNotFoundError();

	return std::dynamic_pointer_cast<Mesh>(vector->at(0));
}
