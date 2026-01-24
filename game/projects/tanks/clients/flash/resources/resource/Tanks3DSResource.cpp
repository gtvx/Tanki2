#include "Tanks3DSResource.h"
#include "hardware/alternativa/engine3d/loaders/Parser3DS.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "hardware/alternativa/engine3d/core/Sorting.h"
#include "hardware/alternativa/engine3d/core/Clipping.h"
#include "ResourceParams.h"
#include "Object3D_isType.h"


Tanks3DSResource::Tanks3DSResource(ResourceInfo *resourceInfo) :
	Resource(resourceInfo)
{

}


QVector<std::shared_ptr<Object3D>>* Tanks3DSResource::getObjectsByName(const char*, const char* name)
{

	QString Name(name);

	QVector<std::shared_ptr<Object3D>> *vector = nullptr;
	const int count = this->objects.count();
	for (int i = 0; i < count; i++)
	{
		std::shared_ptr<Object3D> object = this->objects[i];
		if (object->name.empty())
			continue;

		if (QString::fromStdString(object->name).toLower().contains(Name)) {

			if (vector == nullptr)
				vector = new QVector<std::shared_ptr<Object3D>>;

			vector->append(object);
		}
	}

	return vector;
}


bool Tanks3DSResource::buildResourceData(const QByteArray &data, const QString &name)
{
	bool ok = false;

	Parser3DSResult result = Parser3DS("anks3DSResource::buildResourceData " + name, data);

	if (!result.ok)
		return false;

	this->objects = result.objects;
	this->parents = result.parents;

	for (int i = 0; i < this->objects.count(); i++) {
		std::shared_ptr<Object3D> object = this->objects[i];
		if (Object3D_isType<Mesh>(object.get())) {
			this->initMesh((Mesh*)object.get());
			ok = true;
		}
	}

	/*
	if (this->resourceData->imageData.length > 0)
	{
		this->resourceData->batchConstructor->addEventListener(Event.COMPLETE,this->onTexturesComplete);
		this->resourceData->batchConstructor->run(4,this->resourceData.imageData);
	}
	else
	{
		this->completeResourceLoading(false);
	}
	*/
	return ok;
}


void Tanks3DSResource::initMesh(Mesh *mesh)
{
	mesh->weldVertices(0.001, 0.0001);
	mesh->weldFaces(0.01, 0.001, 0.01);
	mesh->calculateVerticesNormalsByAngle(ResourceParams::ANGLE_THRESHOLD, 0.01);
	mesh->clipping = Clipping::FACE_CLIPPING;
	mesh->sorting = Sorting::AVERAGE_Z;
	mesh->calculateBounds();
}
