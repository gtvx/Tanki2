#include "XMLUtils.h"
#include <QDomElement>


QString XMLUtils::copyXMLString(QString s)
{
	s.detach();
	return s;
}


QString XMLUtils::getAttributeAsString(const QDomElement &element, const QString &attrName, const QString &defValue)
{
	QString attributes = element.attribute(attrName);

	if (!attributes.isNull())
	{
		return attributes;
	}

	return defValue;
}


double XMLUtils::getAttributeAsNumber(const QDomElement &element, const QString &attrName, double defValue)
{
	QString attributes = element.attribute(attrName);

	if (!attributes.isNull())
	{
		return attributes.toDouble();
	}

	return defValue;
}
