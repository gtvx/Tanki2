#ifndef COLLISIONGEOMETRYPARSER_H
#define COLLISIONGEOMETRYPARSER_H

#include "MyList.h"

class CollisionShape;
class QDomElement;

class CollisionGeometryParser
{
public:
	static MyList<CollisionShape*> *parse(const QDomElement &xml);
};

#endif // COLLISIONGEOMETRYPARSER_H
