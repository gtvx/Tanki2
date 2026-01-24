#ifndef POLYGONSINTERSECTIONUTILS_H
#define POLYGONSINTERSECTIONUTILS_H

#include "MyList.h"

class CollisionShape;
class CollidersVertex;
class Matrix4;
class ShapeContact;

class PolygonsIntersectionUtils
{
public:
	static void findContacts(CollisionShape*, CollidersVertex*,
							 int, CollisionShape*, CollidersVertex*, int, Matrix4*, MyList<ShapeContact*>*);
};

#endif // POLYGONSINTERSECTIONUTILS_H
