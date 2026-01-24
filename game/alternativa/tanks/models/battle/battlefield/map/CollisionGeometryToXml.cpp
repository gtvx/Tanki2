#include "CollisionGeometryToXml.h"
#include "game/alternativa/physics/PhysicsMaterial.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/math/Matrix3.h"
#include "game/alternativa/physics/collision/primitives/CollisionRect.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/physics/collision/primitives/CollisionTriangle.h"
#include <QDomDocument>


static void setValue(const char *name, double value, QDomDocument &doc, QDomElement &xml)
{
	QDomElement element = doc.createElement(name);
	element.appendChild(doc.createTextNode(QString::number(value)));
	xml.appendChild(element);
}


static void setVector3(const Vector3 &v, const QString &name, QDomDocument &doc, QDomElement &_xml)
{
	QDomElement xml = doc.createElement(name);
	setValue("x", v.getX(), doc, xml);
	setValue("y", v.getY(), doc, xml);
	setValue("z", v.getZ(), doc, xml);
	_xml.appendChild(xml);
}


static void setRotationAndPosition(const CollisionShape *shape, QDomDocument &doc, QDomElement &xml)
{
	Vector3 position;
	shape->transform.getPosition(&position);
	setVector3(position, "position", doc, xml);

	Vector3 rotation;
	shape->transform.getEulerAngles(&rotation);
	setVector3(rotation, "rotation", doc, xml);
}


static void copyCollisionPlanes(const CollisionShape *shape, QDomDocument &doc, QDomElement &xml)
{
	const CollisionRect *rect = (CollisionRect*)shape;
	const Vector3 &hs = rect->hs;
	QDomElement collision_plane = doc.createElement("collision-plane");
	setValue("width", hs.getX() * 2, doc, collision_plane);
	setValue("length", hs.getY() * 2, doc, collision_plane);
	setRotationAndPosition(shape, doc, collision_plane);
	xml.appendChild(collision_plane);
}


static void copyCollisionTrinagles(const CollisionShape *shape, QDomDocument &doc, QDomElement &xml)
{
	const CollisionTriangle *triangle = (CollisionTriangle*)shape;
	QDomElement collision_triangle = doc.createElement("collision-triangle");
	setVector3(triangle->v0, "v0", doc, collision_triangle);
	setVector3(triangle->v1, "v1", doc, collision_triangle);
	setVector3(triangle->v2, "v2", doc, collision_triangle);
	setRotationAndPosition(shape, doc, collision_triangle);
	xml.appendChild(collision_triangle);
}


static void copyCollisionBox(const CollisionShape *shape, QDomDocument &doc, QDomElement &xml)
{
	const CollisionBox *box = (CollisionBox*)shape;
	QDomElement collision_box = doc.createElement("collision-box");
	Vector3 halfSize;
	halfSize.copy(box->constHS());
	halfSize.scale(2);
	setVector3(halfSize, "size", doc, collision_box);
	setRotationAndPosition(shape, doc, collision_box);
	xml.appendChild(collision_box);
}


static void copyCollision(const MyList<CollisionShape*> *list, QDomDocument &doc, QDomElement &xml)
{
	const int count = list->count();

	for (int i = 0; i < count; i++)
	{
		const CollisionShape *shape = list->at(i);
		switch(shape->type)
		{
			case CollisionShape::Type::BOX:
				copyCollisionBox(shape, doc, xml);
				break;
			case CollisionShape::Type::SPHERE:
				break;
			case CollisionShape::Type::RECT:
				copyCollisionPlanes(shape, doc, xml);
				break;
			case CollisionShape::Type::TRIANGLE:
				copyCollisionTrinagles(shape, doc, xml);
				break;
		}
	}
}


void CollisionGeometryToXml::copy(const MyList<CollisionShape *> *list, QDomDocument &doc, QDomElement &xml)
{
	QDomElement collision_geometry = doc.createElement("collision-geometry");
	copyCollision(list, doc, collision_geometry);
	xml.appendChild(collision_geometry);
}
