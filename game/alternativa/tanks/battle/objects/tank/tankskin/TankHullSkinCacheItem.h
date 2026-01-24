#ifndef TANKHULLSKINCACHEITEM_H
#define TANKHULLSKINCACHEITEM_H

#include "TankSkinPartCacheItem.h"
#include <memory>

class Mesh;
class Tanks3DSResource;
class Vector3;
class EncryptedNumber;

class TankHullSkinCacheItem : public TankSkinPartCacheItem
{
	Vector3 *turretMountPoint;
	EncryptedNumber *encTurretMountPointX;
	EncryptedNumber *encTurretMountPointY;
	EncryptedNumber *encTurretMountPointZ;

public:
	TankHullSkinCacheItem(Tanks3DSResource*);

	double getTurretMountPointX();
	double getTurretMountPointY();
	double getTurretMountPointZ();

	std::shared_ptr<Mesh> mesh;

private:
	std::shared_ptr<Mesh> getMesh(Tanks3DSResource *param1);
};

#endif // TANKHULLSKINCACHEITEM_H
