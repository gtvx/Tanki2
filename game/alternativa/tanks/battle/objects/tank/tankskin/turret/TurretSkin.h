#ifndef TURRETSKIN_H
#define TURRETSKIN_H

#include <QVector>
#include <memory>

class Tanks3DSResource;
class Object3D;
class ColorTransform;
class Mesh;
class TankHullSkinCacheItem;
class Matrix4;
class TextureMaterial;


class TurretSkin
{
	std::shared_ptr<Object3D> rootObject;
	QVector<std::shared_ptr<Mesh>> turretMeshes;

public:
	TurretSkin(Tanks3DSResource*);
	std::shared_ptr<Object3D> getTurret3D();
	std::shared_ptr<Object3D> getBarrel3D();
	void setColorTransform(ColorTransform*);

	void updateTurretTransform(Matrix4*, TankHullSkinCacheItem*, double, double);

	void setMaterials(std::shared_ptr<TextureMaterial> material);

protected:
	//QVector<std::shared_ptr<Mesh>> *turretMeshes;
};

#endif // TURRETSKIN_H
