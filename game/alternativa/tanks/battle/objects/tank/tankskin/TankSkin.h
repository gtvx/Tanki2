#ifndef TANKSKIN_H
#define TANKSKIN_H

#include <QVector>
#include <memory>
#include "shared.h"

class TankHullSkinCacheItem;
class Mesh;
class TurretGeometryItem;
class TurretSkinCacheItem;
class IGameObject;
class TurretSkin;
class Vector3;
class SkinMaterials;
class Face;
class Object3D;
class TankSkinMaterialsFactory;
class Material;
class TrackSkin;


class TankSkin
{
	std::shared_ptr<Mesh> hullMesh;
	TurretSkinCacheItem *turretDescriptor;
	TurretSkin *turretSkin;
	SkinMaterials *normalMaterials;
	TankSkinMaterialsFactory *tankSkinMaterialFactory;
	TrackSkin *leftTrackSkin;
	TrackSkin *rightTrackSkin;

public:
	TankSkin(IGameObject*, IGameObject*, IGameObject*);
	TankHullSkinCacheItem *getHullDescriptor();
	TankHullSkinCacheItem *hullDescriptor;
	std::shared_ptr<Mesh> getHullMesh();
	QVector<TurretGeometryItem *> *getTurretGeometry();
	void addToScene();
	void updateHullTransform(const Vector3*, const Vector3*);
	void setNormalState();
	std::shared_ptr<Object3D> getTurret3D();
	std::shared_ptr<Object3D> getBarrel3D();
	TurretSkinCacheItem *getTurretDescriptor();
	void updateTurretTransform(double, double);
	void removeFromScene();
	void updateTracks(double leftValue, double rightValue);

private:
	std::shared_ptr<Mesh> cloneMesh(std::shared_ptr<Mesh> mesh);
	void createTrackSkins(Mesh *mesh);
	void addFaceToTrackSkin(Shared<Face> face);
	void setTracksMaterial(std::shared_ptr<Material> left, std::shared_ptr<Material> right);
};

#endif // TANKSKIN_H
