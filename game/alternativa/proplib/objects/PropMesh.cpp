#include "PropMesh.h"
#include "../utils/TextureByteDataMap.h"
#include "game/alternativa/utils/textureutils/TextureByteData.h"
#include "../utils/ByteArrayMap.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "hardware/alternativa/engine3d/loaders/Parser3DS.h"
#include "Error.h"
#include "hardware/alternativa/engine3d/objects/Occluder.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include <QDebug>

const QString PropMesh::DEFAULT_TEXTURE = "$$$_DEFAULT_TEXTURE_$$$";


static const double threshold = 0.01;
static const double occluderDistanceThreshold = 0.01;
static const double occluderAngleThreshold = 0.01;
static const double occluderConvexThreshold = 0.01;
static const int occluderUvThreshold = 1;
static const double meshDistanceThreshold = 0.001;
static const double meshUvThreshold = 0.001;
static const double meshAngleThreshold = 0.001;
static const double meshConvexThreshold = 0.01;


static QString getTextureFileName(Mesh *mesh)
{
	Weak<Face> face = mesh->faceList;
	while (face != nullptr)
	{
		std::shared_ptr<TextureMaterial> material = std::dynamic_pointer_cast<TextureMaterial>(face->material);

		if (material != nullptr)
		{
			return material->diffuseMapURL.toLower();
		}

		face = face->next;
	}

	return {};
}


static void initMesh(Mesh *mesh)
{
	mesh->weldVertices(meshDistanceThreshold, meshUvThreshold);
	mesh->weldFaces(meshAngleThreshold, meshUvThreshold, meshConvexThreshold);
	mesh->threshold = threshold;
}


PropMesh::PropMesh(const QByteArray &modelData,
				   const QString &objectName,
				   QMap<QString, QString> *textureFiles,
				   ByteArrayMap *files,
				   TextureByteDataMap *imageMap,
				   const QString &name) :
	PropObject(PropObjectType::MESH)
{

	this->occluders = nullptr;
	this->parseModel(modelData, objectName, textureFiles, files, imageMap, name);
}


void PropMesh::parseModel(const QByteArray &modelData,
						  const QString &objectName,
						  QMap<QString, QString> *textureFiles,
						  ByteArrayMap *files,
						  TextureByteDataMap *imageMap,
						  const QString &name)
{
	std::shared_ptr<Mesh> mesh = this->processObjects(modelData, objectName, name);

	initMesh(mesh.get());

	this->object = mesh;
	QString defaultTextureFileName = getTextureFileName(mesh.get());

	if (defaultTextureFileName.isNull() && textureFiles == nullptr)
	{
		throw Error("PropMesh: no textures found");
	}

	if (textureFiles == nullptr)
	{
		textureFiles = new  QMap<QString, QString>;
	}

	if (!defaultTextureFileName.isNull())
	{
		textureFiles->insert(DEFAULT_TEXTURE, defaultTextureFileName);
	}

	this->textures = new TextureByteDataMap();

	QMapIterator<QString, QString> i(*textureFiles);
	while (i.hasNext())
	{
		i.next();

		QString textureName = i.key();
		QString textureFileName = i.value();

		TextureByteData *textureByteData;

		if (imageMap == nullptr)
		{
			textureByteData = new TextureByteData(files->getValue(textureFileName));
		}
		else
		{
			textureByteData = imageMap->getValue(textureFileName);
		}

		this->textures->putValue(textureName, textureByteData);
	}
}


std::shared_ptr<Mesh> PropMesh::processObjects(const QByteArray &modelData, const QString &objectName, const QString &name)
{

	Parser3DSResult result = Parser3DS("PropMesh::processObjects " + name, modelData);

	if (!result.ok)
	{
		qDebug() << "error PropMesh::processObjects" << name;
		return nullptr;
	}

	QVector<std::shared_ptr<Object3D>> objects = result.objects;

	int numObjects = objects.length();

	std::shared_ptr<Mesh> mesh;

	for (int i = 0; i < numObjects; i++)
	{
		std::shared_ptr<Object3D> currObject = objects[i];

		QString currObjectName = QString::fromStdString(currObject->name).toLower();

		if (currObjectName.indexOf("occl") == 0)
		{
			this->addOccluder(std::dynamic_pointer_cast<Mesh>(currObject));
		}
		else if (objectName == currObjectName)
		{
			mesh = std::dynamic_pointer_cast<Mesh>(currObject);
		}
	}

	return (mesh != nullptr) ? mesh : std::dynamic_pointer_cast<Mesh>(objects[0]);
}


void PropMesh::addOccluder(std::shared_ptr<Mesh> mesh)
{
	mesh->weldVertices(occluderDistanceThreshold, occluderUvThreshold);
	mesh->weldFaces(occluderAngleThreshold, occluderUvThreshold, occluderConvexThreshold);

	std::shared_ptr<Occluder> occluder= new_Occluder();
	occluder->createForm(mesh, true);
	occluder->x = mesh->x;
	occluder->y = mesh->y;
	occluder->z = mesh->z;
	occluder->rotationX = mesh->rotationX;
	occluder->rotationY = mesh->rotationY;
	occluder->rotationZ = mesh->rotationZ;

	if (this->occluders == nullptr)
	{
		this->occluders = new QVector<std::shared_ptr<Occluder>>();
	}

	this->occluders->append(occluder);
}

