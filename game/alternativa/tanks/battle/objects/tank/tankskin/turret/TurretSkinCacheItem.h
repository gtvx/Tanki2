#ifndef TURRETSKINCACHEITEM_H
#define TURRETSKINCACHEITEM_H

#include <QVector>
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankSkinPartCacheItem.h"

class TurretGeometryItem;
class Tanks3DSResource;
class Mesh;
class Vector3;

//extends TankSkinPartCacheItem implements DataUnitValidator
class TurretSkinCacheItem : public TankSkinPartCacheItem
{
public:
	TurretSkinCacheItem(Tanks3DSResource*);
	QVector<TurretGeometryItem *> *getGeometry();
	QVector<std::shared_ptr<Mesh>> meshes;
	QVector<Vector3*> *muzzles;
	Vector3 *laserPoint;
	Vector3 *flagMountPoint;

	void initMeshes(Tanks3DSResource*);

private:
	QVector<TurretGeometryItem *> *parseGeometry(Tanks3DSResource*);
	QVector<TurretGeometryItem*> *collisionGeometry;
	QVector<Vector3 *> *parseMuzzles(Tanks3DSResource *resource);
	Vector3 *parseLaserPoint(Tanks3DSResource *resource);
};

#endif // TURRETSKINCACHEITEM_H
