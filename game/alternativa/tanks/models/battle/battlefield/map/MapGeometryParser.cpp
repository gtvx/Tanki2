#include "MapGeometryParser.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/proplib/objects/PropObject.h"
#include "TexturedPropsRegistry.h"
#include "game/alternativa/proplib/types/PropGroup.h"
#include "game/alternativa/proplib/types/PropData.h"
#include "game/alternativa/proplib/types/PropState.h"
#include "game/alternativa/proplib/PropLibrary.h"
#include "game/alternativa/proplib/PropLibRegistry.h"
#include "hardware/alternativa/engine3d/objects/Sprite3D.h"
#include "game/alternativa/proplib/objects/PropMesh.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "hardware/alternativa/engine3d/core/Sorting.h"
#include "game/alternativa/tanks/battle/scene3d/Object3DNames.h"
#include "hardware/alternativa/engine3d/objects/Occluder.h"
#include "flash/geom/Vector3D.h"
#include "flash/geom/Matrix3D.h"
#include "NormalsCalculator.h"
#include "Object3D_isType.h"
#include "hardware/alternativa/engine3d/objects/BSP.h"
#include "game/alternativa/proplib/objects/PropSprite.h"
#include "MyMath.h"
#include "flash/geom/Matrix3D.h"
#include <QString>
#include <QDomElement>
#include <QDebug>


MapGeometryParser::MapGeometryParser(PropLibRegistry *propLibRegistry)
{
	this->propLibRegistry = propLibRegistry;

	this->texturedPropsRegistry = new TexturedPropsRegistry();


	//this->occluders = new QVector<Occluder*>();
	//this->billboards = new QVector<Mesh*>();
	//this->meshProps = new QVector<Object3D*>();
	//this->propObjectByMesh = new Dictionary();
	//this->textureNameByMesh = new Dictionary();
	//this->mapBounds = new AABB();
	this->normalsCalculator = new NormalsCalculator();
	//this->christmasTreeToys = new QVector<Object3D*>();


	this->mapBounds.infinity();
}


static QString getPropTextureName(const QDomElement &xml)
{
	QString _local_2;
	QDomNodeList _local_3 = xml.elementsByTagName("texture-name");

	if (_local_3.length() > 0)
	{
		_local_2 = _local_3.at(0).toElement().text();
	}

	if (!_local_2.isEmpty())
	{
		return _local_2;
	}
	else
	{
		return PropMesh::DEFAULT_TEXTURE;
	}
}


static double getDouble(const QDomNode &a, const char *name)
{
	bool ok;
	double value = a.firstChildElement(name).text().toDouble(&ok);
	if (ok == false) {
		throw 21;
	}
	return value;
}


static void readVector3D(const QDomElement &xml, Vector3D *result)
{
	if (xml.isNull() == false)
	{
		result->x = getDouble(xml, "x");
		result->y = getDouble(xml, "y");
		result->z = getDouble(xml, "z");
	}
	else
	{
		result->reset(0, 0, 0);
	}
}


void MapGeometryParser::parse(const QDomElement &xml)
{
	this->parseProps(xml);

	this->normalsCalculator->calculateNormals(this->meshProps);

	if (this->christmasTree != nullptr)
	{
		std::dynamic_pointer_cast<Mesh>(this->christmasTree)->calculateVerticesNormalsByAngle(MyMath::PI / 3, 1);

	}

	this->makeBSPs();
}


void MapGeometryParser::parseProps(const QDomElement &element)
{
	QDomElement static_geometry = element.firstChildElement("static-geometry");

	QDomNodeList list = static_geometry.elementsByTagName("prop");

	for (int i = 0; i < list.count(); i++)
	{
		 this->parseProp(list.item(i).toElement());
	}
}


void MapGeometryParser::parseProp(const QDomElement &element)
{
	PropObject *propObject = this->getPropObject(element);

	if (propObject != nullptr)
	{
		if (propObject->type() == PropObjectType::MESH)
		{
			this->parseMesh(element, (PropMesh*)propObject);
		}
		else if (propObject->type() == PropObjectType::SPRITE)
		{
			this->parseSprite(element, (PropSprite*)propObject);
		}
	}
}


PropObject* MapGeometryParser::getPropObject(const QDomElement &element)
{
	QString library_name = element.attribute("library-name");
	QString group_name = element.attribute("group-name");
	QString name = element.attribute("name");
	PropLibrary *propLibrary = this->propLibRegistry->getLibrary(library_name);

	if (propLibrary == nullptr)
	{
		return nullptr;
	}

	PropGroup *propGroup = propLibrary->getRootGroup()->getGroupByName(group_name);
	if (propGroup == nullptr)
	{
		return nullptr;
	}

	PropData *propData = propGroup->getPropByName(name);
	if (propData == nullptr)
	{
		return nullptr;
	}

	return propData->getDefaultState()->getDefaultObject();
}


void MapGeometryParser::parseMesh(const QDomElement &element, PropMesh *propMesh)
{
	if (element.attribute("name") == "Billboard")
	{
		this->parseBillboard(element, propMesh);
	}
	else
	{
		this->parseMeshProp(element, propMesh);
	}
}


void MapGeometryParser::parseBillboard(const QDomElement &element, PropMesh *propMesh)
{
	std::shared_ptr<Mesh> _local_3 = std::dynamic_pointer_cast<Mesh>(propMesh->object->clone());

	_local_3->calculateFacesNormals();
	_local_3->calculateVerticesNormalsByAngle(0);
	_local_3->sorting = Sorting::DYNAMIC_BSP;
	_local_3->name = Object3DNames::STATIC;

	this->billboards.append(_local_3);

	QString _local_4 = getPropTextureName(element);

	this->texturedPropsRegistry->addMesh(propMesh, _local_4, _local_3, "display");

	Vector3D _local_5;

	readVector3D(element.firstChildElement("position"), &_local_5);

	this->mapBounds.addPoint(_local_5.x, _local_5.y, _local_5.z);

	Vector3D _local_6;

	readVector3D(element.firstChildElement("rotation"), &_local_6);
	_local_3->x = _local_5.x;
	_local_3->y = _local_5.y;
	_local_3->z = _local_5.z;
	_local_3->rotationZ = _local_6.z;

	this->objects.append(_local_3);
}


void MapGeometryParser::parseMeshProp(const QDomElement &element, PropMesh *propMesh)
{
	std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(propMesh->object->clone());


	Matrix3DComposeParams components;

	Vector3D &_local_4 = components.translation;
	readVector3D(element.firstChildElement("position"), &_local_4);
	mesh->x = _local_4.x;
	mesh->y = _local_4.y;
	mesh->z = _local_4.z;

	this->mapBounds.addPoint(_local_4.x, _local_4.y, _local_4.z);

	Vector3D &rotation = components.rotation;

	readVector3D(element.firstChildElement("rotation"), &rotation);

	mesh->rotationZ = rotation.z;

	components.scaling.reset(1, 1, 1);

	QString name = getPropTextureName(element);

	this->meshProps.append(mesh);
	this->propObjectByMesh[mesh] = propMesh;
	this->textureNameByMesh[mesh] = name;
	this->createOccluders(propMesh, components);

	if (element.attribute("name") == "Elka")
	{
		this->christmasTree = mesh;
	}
}


void MapGeometryParser::createOccluders(PropMesh *propMesh, const Matrix3DComposeParams &components)
{
	if (propMesh->occluders != nullptr)
	{
		Matrix3D objectMatrix;

		objectMatrix.recompose(components, Matrix3DOrientation::EULER_ANGLES);

		for (std::shared_ptr<Occluder> item : *propMesh->occluders)
		{
			Matrix3D matrix3D;
			item->getMatrix(&matrix3D);
			matrix3D.append(objectMatrix);
			std::shared_ptr<Occluder> occluder = std::dynamic_pointer_cast<Occluder>(item->clone());
			occluder->setMatrix(&matrix3D);
			this->occluders.append(occluder);
		}
	}
}


void MapGeometryParser::parseSprite(const QDomElement &element, PropSprite *propSprite)
{
	std::shared_ptr<Sprite3D> _local_3 = std::dynamic_pointer_cast<Sprite3D>(propSprite->object->clone());


	if (element.attribute("name").indexOf("Shar") >= 0)
	{
		this->christmasTreeToys.append(_local_3);
		Vector3D _local_5;
		readVector3D(element.firstChildElement("position"), &_local_5);
		_local_3->x = _local_5.x;
		_local_3->y = _local_5.y;
		_local_3->z = _local_5.z;
		_local_3->name = element.attribute("name").toStdString();
		return;
	}

	_local_3->shadowMapAlphaThreshold = 0;
	_local_3->softAttenuation = 80;
	this->sprites.append(_local_3);
	Vector3D _local_4;
	readVector3D(element.firstChildElement("position"), &_local_4);
	_local_3->x = _local_4.x;
	_local_3->y = _local_4.y;
	_local_3->z = _local_4.z;
	_local_3->width = propSprite->scale;
	this->texturedPropsRegistry->addSprite3D(propSprite, _local_3);
}


void MapGeometryParser::makeBSPs()
{
	for (std::shared_ptr<Object3D> _local_1 : this->meshProps)
	{
		std::shared_ptr<Mesh> _local_2 = std::dynamic_pointer_cast<Mesh>(_local_1);
		std::shared_ptr<BSP> _local_3 = new_BSP();
		_local_3->name = Object3DNames::STATIC;
		_local_3->createTree(_local_2, true);
		_local_3->x = _local_2->x;
		_local_3->y = _local_2->y;
		_local_3->z = _local_2->z;
		_local_3->rotationZ = _local_2->rotationZ;
		this->texturedPropsRegistry->addBSP(this->propObjectByMesh[_local_2], this->textureNameByMesh[_local_2], _local_3);
		this->objects.append(_local_3);
		if (_local_2 == this->christmasTree)
		{
			this->christmasTree = _local_3;
		}
	}

	//this->meshProps->length = 0;
	//clearDictionary(this.propObjectByMesh);
	//clearDictionary(this.textureNameByMesh);
}


void MapGeometryParser::clear()
{
	//this->propLibRegistry = nullptr;
	this->texturedPropsRegistry->clear();
	this->objects.clear();
	this->sprites.clear();
	this->occluders.clear();
	this->billboards.clear();
	this->christmasTree = nullptr;
	this->christmasTreeToys.clear();
}


QVector<std::shared_ptr<Object3D>> MapGeometryParser::getObjects()
{
	return this->objects;
}


QVector<std::shared_ptr<Object3D>> MapGeometryParser::getSprites()
{
	return this->sprites;
}


QVector<std::shared_ptr<Occluder>> MapGeometryParser::getOccluders()
{
	return this->occluders;
}


QVector<std::shared_ptr<Mesh>> MapGeometryParser::getBillboards()
{
	return this->billboards;
}


QVector<TexturedPropsCollection*> MapGeometryParser::getTexturedPropsCollections()
{
	return this->texturedPropsRegistry->getCollections();
}


AABB* MapGeometryParser::getMapBounds()
{
	return &this->mapBounds;
}


std::shared_ptr<Object3D> MapGeometryParser::getChristmasTree()
{
	return this->christmasTree;
}


QVector<std::shared_ptr<Object3D>> MapGeometryParser::getChristmasTreeToys()
{
	return this->christmasTreeToys;
}

