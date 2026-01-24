#ifndef COLLISIONGEOMETRYTOXML_H
#define COLLISIONGEOMETRYTOXML_H

#include "MyList.h"

class CollisionShape;
class QDomElement;
class QDomDocument;

class CollisionGeometryToXml
{
public:
	static void copy(const MyList<CollisionShape*>*, QDomDocument &doc, QDomElement &xml);
};

#endif // COLLISIONGEOMETRYTOXML_H
