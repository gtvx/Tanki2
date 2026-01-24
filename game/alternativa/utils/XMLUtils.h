#ifndef XMLUTILS_H
#define XMLUTILS_H

#include <QString>

class QDomElement;

class XMLUtils
{
public:
	static QString copyXMLString(QString s);
	static QString getAttributeAsString(const QDomElement &element, const QString &attrName, const QString &defValue = QString());
	static double getAttributeAsNumber(const QDomElement &element, const QString &attrName, double defValue);
};

#endif // XMLUTILS_H
