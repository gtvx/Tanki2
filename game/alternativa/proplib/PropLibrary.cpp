#include "PropLibrary.h"
#include "utils/ByteArrayMap.h"
#include "types/PropGroup.h"
#include "game/alternativa/utils/XMLUtils.h"
#include "utils/TextureByteDataMap.h"
#include "game/alternativa/utils/textureutils/TextureByteData.h"
#include "types/PropData.h"
#include "types/PropState.h"
#include "objects/PropMesh.h"
#include "objects/PropSprite.h"
#include "Error.h"
#include <QString>
#include <QDomDocument>
#include <QDebug>


static const QString LIB_FILE_NAME = "library.xml";
static const QString IMG_FILE_NAME = "images.xml";


PropLibrary::PropLibrary(ByteArrayMap *files)
{
	if (files == nullptr)
	{
		throw ArgumentError("Parameter files is null");
	}

	this->files = files;
	QByteArray imageMapData = files->getValue(IMG_FILE_NAME);
	if (imageMapData != nullptr)
	{
		QDomDocument doc;
		doc.setContent(imageMapData);

		if (!doc.isDocument())
		{
			throw Error("PropLibrary::PropLibrary 1");
		}

		QDomElement xml = doc.documentElement();

		this->imageMap = this->parseImageMap(xml);
	}

	{
		QByteArray data = files->getValue(LIB_FILE_NAME);

		QDomDocument doc;
		doc.setContent(data);

		if (!doc.isDocument())
		{
			throw Error("PropLibrary::PropLibrary 2");
		}

		QDomElement xml = doc.documentElement();

		this->_rootGroup = this->parseGroup(xml);
	}
}


QString PropLibrary::getName()
{
	return (this->_rootGroup == nullptr) ? QString() : this->_rootGroup->name;
}


PropGroup* PropLibrary::getRootGroup()
{
	return this->_rootGroup;
}


TextureByteDataMap* PropLibrary::parseImageMap(const QDomElement &imagesXml)
{
	TextureByteDataMap *imageFiles = new TextureByteDataMap();

	QDomNodeList list = imagesXml.elementsByTagName("image");

	for (int i = 0; i < list.count(); i++)
	{
		QDomElement image = list.at(i).toElement();

		QString originalTextureFileName = image.attribute("name");
		QString diffuseName = image.attribute("new-name").toLower();
		QString opacityName = XMLUtils::getAttributeAsString(image, "alpha", QString());

		if (!opacityName.isNull())
		{
			opacityName = opacityName.toLower();
		}

		imageFiles->putValue(originalTextureFileName, new TextureByteData(this->files->getValue(diffuseName), this->files->getValue(opacityName)));
	}

	return imageFiles;
}


PropObject* PropLibrary::parsePropObject(const QDomElement &parentXmlElement)
{
	QDomElement mesh = parentXmlElement.firstChildElement("mesh");

	if (!mesh.isNull())
	{
		return this->parsePropMesh(mesh);
	}

	QDomElement sprite = parentXmlElement.firstChildElement("sprite");

	if (!parentXmlElement.isNull())
	{
		return this->parsePropSprite(sprite);
	}

	throw Error("Unknown prop type");
}


PropState* PropLibrary::parseState(const QDomElement &stateXml)
{

	PropState *state = new PropState();

	QDomNodeList lods = stateXml.elementsByTagName("lod");

	if (lods.length() > 0)
	{
		for (int i = 0; i < lods.count(); i++)
		{
			QDomElement lodXml = lods.at(i).toElement();
			state->addLOD(parsePropObject(lodXml), lodXml.attribute("distance").toDouble());
		}
	}
	else
	{
		state->addLOD(parsePropObject(stateXml), 0);
	}

	return state;
}


PropData* PropLibrary::parseProp(const QDomElement &propXml)
{
	PropData *prop = new PropData(XMLUtils::copyXMLString(propXml.attribute("name")));

	QDomNodeList states = propXml.elementsByTagName("state");

	if (states.length() > 0)
	{
		for (int i = 0; i < states.count(); i++)
		{
			QDomElement stateXml = states.at(i).toElement();

			prop->addState(XMLUtils::copyXMLString(stateXml.attribute("name")), parseState(stateXml));
		}
	}
	else
	{
		prop->addState(PropState::DEFAULT_NAME, parseState(propXml));
	}

	return prop;
}


PropGroup* PropLibrary::parseGroup(const QDomElement &groupXML)
{
	PropGroup *group = new PropGroup(XMLUtils::copyXMLString(groupXML.attribute("name")));


	QDomNodeList list;

	list = groupXML.elementsByTagName("prop");

	for (int i = 0; i < list.count(); i++)
	{
		QDomElement element = list.at(i).toElement();
		group->addProp(this->parseProp(element));
	}

	list = groupXML.elementsByTagName("prop-group");

	for (int i = 0; i < list.count(); i++)
	{
		QDomElement element = list.at(i).toElement();
		group->addGroup(this->parseGroup(element));
	}

	return group;
}


PropMesh* PropLibrary::parsePropMesh(const QDomElement &propXml)
{
	QString file = propXml.attribute("file").toLower();
	QByteArray modelData = this->files->getValue(file);

	QDomNodeList list = propXml.elementsByTagName("texture");

	QMap<QString, QString> *textureFiles = new  QMap<QString, QString>();

	if (list.length() > 0)
	{
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement textureXml = list.at(i).toElement();
			textureFiles->insert(XMLUtils::copyXMLString(textureXml.attribute("name")), textureXml.attribute("diffuse-map").toLower());
		}
	}

	QString objectName = XMLUtils::getAttributeAsString(propXml, "object", QString());

	return new PropMesh(modelData, objectName, textureFiles, this->files, this->imageMap, file);
}


PropSprite* PropLibrary::parsePropSprite(const QDomElement &propXml)
{
	QString textureFile = propXml.attribute("file").toLower();

	TextureByteData *textureData = ((this->imageMap == nullptr) ? new TextureByteData(this->files->getValue(textureFile)) : this->imageMap->getValue(textureFile));

	double originX = XMLUtils::getAttributeAsNumber(propXml, "origin-x", 0.5);
	double originY = XMLUtils::getAttributeAsNumber(propXml, "origin-y", 0.5);
	double scale = XMLUtils::getAttributeAsNumber(propXml, "scale", 1);

	return new PropSprite(textureData, originX, originY, scale);
}
