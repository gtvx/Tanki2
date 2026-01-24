#include "TurretSkinCacheItem.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "game/projects/tanks/clients/flash/resources/resource/Tanks3DSResource.h"
#include "TurretGeometryItem.h"
#include "Error.h"
#include "Object3D_isType.h"
#include "game/projects/tanks/clients/flash/resources/tanks/Tank3D.h"


static Vector3* parseFlagMountPoint(Tanks3DSResource *resourc)
{
	QVector<std::shared_ptr<Object3D>> *vector = resourc->getObjectsByName("/fmnt.*/i", "fmnt");
	if (vector != nullptr)
	{
		std::shared_ptr<Object3D> o = vector->at(0);
		return new Vector3(o->x, o->y, o->z);
	}
	throw Error(1566);
}


TurretSkinCacheItem::TurretSkinCacheItem(Tanks3DSResource *resource) :
	TankSkinPartCacheItem(resource)
{
	this->collisionGeometry = this->parseGeometry(resource);
	this->initMeshes(resource);
	this->flagMountPoint = parseFlagMountPoint(resource);
	this->muzzles = this->parseMuzzles(resource);
	this->laserPoint = this->parseLaserPoint(resource);
	this->collisionGeometry = this->parseGeometry(resource);
	//this->createMuzzleValidators();
}


static bool PARENT_RELATIVE(const std::string &n)
{
	return n == "launcher_" ||  n == "barrel_";
}


QVector<Vector3*>* TurretSkinCacheItem::parseMuzzles(Tanks3DSResource *resource)
{
	QVector<Vector3*> *muzzles = new QVector<Vector3*>;
	QVector<std::shared_ptr<Object3D>> *objects = resource->getObjectsByName("/muzzle.*/", "muzzle");
	if (objects != nullptr)
	{
		//::sort(objects);
		for (int i = 0; i < objects->count(); i++)
		{
			std::shared_ptr<Object3D> object = objects->at(i);
			Vector3 *muzzle = new Vector3(object->x, object->y, object->z);
			std::shared_ptr<Object3D> parent = resource->parents[resource->objects.indexOf(object)];
			if (parent != nullptr && PARENT_RELATIVE(parent->name))
			{
				muzzle->setX(muzzle->getX() + parent->x);
				muzzle->setY(muzzle->getY() + parent->y);
				muzzle->setZ(muzzle->getZ() + parent->z);
			}
			muzzles->append(muzzle);
		}
		return muzzles;
	}
	throw Error(55);
}


QVector<TurretGeometryItem*>* TurretSkinCacheItem::getGeometry()
{
	return this->collisionGeometry;
}


QVector<TurretGeometryItem*>* TurretSkinCacheItem::parseGeometry(Tanks3DSResource *param1)
{
	QVector<std::shared_ptr<Object3D>> *v = param1->getObjectsByName("/box.*/i", "box");
	QVector<TurretGeometryItem*> *vector = new QVector<TurretGeometryItem*>;
	if (v != nullptr && v->count() != 0)
	{
		for (int i = 0; i < v->count(); i++)
		{
			vector->append(new TurretGeometryItem(v->at(i).get()));
		}
	}
	else
	{
		for (int i = 0; i < meshes.count(); i++)
		{
			vector->append(new TurretGeometryItem(meshes.at(i).get()));
		}
	}
	return vector;
}


void TurretSkinCacheItem::initMeshes(Tanks3DSResource *resource)
{
	for (int i = 0; i < resource->objects.count(); i++) {
		std::shared_ptr<Object3D> o = resource->objects.at(i);

		if (Object3D_isType<Mesh>(o.get()) && !Tank3D::EXCLUDED_test(o->name)) {
			this->meshes.append(initMesh(std::dynamic_pointer_cast<Mesh>(o)));
		}
	}
}


Vector3* TurretSkinCacheItem::parseLaserPoint(Tanks3DSResource *resource)
{
	QVector<std::shared_ptr<Object3D>> *vector = resource->getObjectsByName("/laser/i", "laser");
	if (vector != nullptr && vector->count() != 0) {
		std::shared_ptr<Object3D> o = vector->at(0);
		return new Vector3(o->x, o->y, o->z);
	}
	return this->muzzles->at(0);
}
