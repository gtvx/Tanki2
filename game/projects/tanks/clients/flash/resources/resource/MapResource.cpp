#include "MapResource.h"
#include "Error.h"
#include "game/alternativa/proplib/PropLibRegistry.h"
#include "game/projects/tanks/clients/flash/resources/resource/PropLibResource.h"
#include "entrance/platform/client/fp10/core/registry/ResourceRegistry.h"
#include <QDomElement>
#include <QDebug>


ResourceRegistry *MapResource::resourceRegistry;


MapResource::MapResource(ResourceInfo *resourceInfo) :
	Resource(resourceInfo)
{
	libRegistry = nullptr;
	libIds = nullptr;
}


QString MapResource::getDescription()
{
	return "Map";
}


void MapResource::load(const QByteArray &data)
{
	this->proplibsData = data;
	this->buildMap();
}


void MapResource::parsePropLibsInfo(const QDomElement &xml)
{
	this->libRegistry = new PropLibRegistry();
	this->libIds = new QVector<Long*>();

	QDomNodeList library = xml.elementsByTagName("library");

	for (int i = 0; i < library.count(); i++)
	{
		const QDomNode &node = library.at(i);
		QDomElement element = node.toElement();

		Long resource_id = Long::fromString(element.attribute("resource-id"));


		this->libIds->append(new Long(resource_id));

		PropLibResource *propLibResource = (PropLibResource*)resourceRegistry->getResource(&resource_id);

		if (propLibResource != nullptr)
		{
			this->libRegistry->addLibrary(propLibResource->lib);
		}
		else
		{
			qDebug() << "Prop library resource [id=" + resource_id.toString() + "] not found";
			//throw Error("Prop library resource [id=" + id->toString() + "] not found");
		}
	}
}


void MapResource::buildMap()
{
	QDomDocument doc;


	doc.setContent(this->proplibsData);

	if (!doc.isDocument())
	{
		throw Error("MapResource::buildMap");
	}

	QDomElement xml = doc.documentElement();
	this->parsePropLibsInfo(xml);
}

