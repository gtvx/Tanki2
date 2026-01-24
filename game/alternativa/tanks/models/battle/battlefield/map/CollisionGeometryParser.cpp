#include "CollisionGeometryParser.h"
#include "game/alternativa/physics/PhysicsMaterial.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/math/Matrix3.h"
#include "game/alternativa/physics/collision/primitives/CollisionRect.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/physics/collision/primitives/CollisionTriangle.h"
#include "Error.h"
#include <QDomDocument>
#include <QDebug>

static const PhysicsMaterial MATERIAL(0, 1);
//static const int STATIC_COLLISION_GROUP = 255;


static void putAll(MyList<CollisionShape*> *param1, MyList<CollisionShape*> *param2)
{
	for (int i = 0; i < param1->count(); i++)
		param2->append(param1->at(i));
	delete param1;
}


static double getDouble(const QDomNode &a, const char *name)
{
	bool ok;
	double value = a.firstChildElement(name).text().toDouble(&ok);
	if (ok == false) {
		throw Error(21);
	}
	return value;
}


static double getDouble2(const QDomNode &a, const char *name, bool *ok)
{
	return a.firstChildElement(name).text().toDouble(ok);
}


static void readVector3(const QDomElement &xml, Vector3 *vector3)
{
	if (xml.isNull() == false)
	{
		vector3->setX(getDouble(xml, "x"));
		vector3->setY(getDouble(xml, "y"));
		vector3->setZ(getDouble(xml, "z"));
	}
	else
	{
		throw Error(20);
		vector3->reset(0, 0, 0);
	}
}


static void setCollisionPrimitiveOrientation(CollisionShape *param1, const QDomNode &xml)
{
	Vector3 position;
	Vector3 rotation;
	readVector3(xml.firstChildElement("position"), &position);
	readVector3(xml.firstChildElement("rotation"), &rotation);
	param1->transform.setMatrix(position.getX(), position.getY(), position.getZ(),
								rotation.getX(), rotation.getY(), rotation.getZ());
}


static MyList<CollisionShape*>* parseCollisionPlanes(const QDomNodeList &xml)
{
	const int count = xml.count();
	MyList<CollisionShape*> *vector = new MyList<CollisionShape*>;

	vector->reserve(count);
	for (int i = 0; i < count; i++)
	{
		QDomNode item = xml.at(i);
		Vector3 halfSize;
		halfSize.setX(0.5 * getDouble(item, "width"));
		halfSize.setY(0.5 * getDouble(item, "length"));
		halfSize.setZ(0);
		CollisionShape *collisionShape = new CollisionRect(&halfSize, CollisionGroup::STATIC_GROUP, &MATERIAL);
		setCollisionPrimitiveOrientation(collisionShape, item);
		vector->append(collisionShape);
	}
	return vector;
}


static MyList<CollisionShape*>* parseCollisionBoxes(const QDomNodeList &xml)
{
	const int count = xml.count();
	MyList<CollisionShape*> * vector = new MyList<CollisionShape*>;
	vector->reserve(count);
	for (int i = 0; i < count; i++)
	{
		QDomNode item = xml.at(i);
		Vector3 halfSize;
		readVector3(item.firstChildElement("size"), &halfSize);
		halfSize.scale(0.5);
		bool ok;
		int type = getDouble2(item, "type", &ok);
		CollisionShape *collisionShape = new CollisionBox(&halfSize, CollisionGroup::STATIC_GROUP, &MATERIAL);
		setCollisionPrimitiveOrientation(collisionShape, item);
		if (ok == true) {
			CollisionBox *box = (CollisionBox*)collisionShape;
			box->t = type;
		}

		vector->append(collisionShape);
	}
	return vector;
}


static MyList<CollisionShape*>* parseCollisionTrinagles(const QDomNodeList &xml)
{
	const int count = xml.count();
	MyList<CollisionShape*> *vector= new MyList<CollisionShape*>;

	vector->reserve(count);
	for (int i = 0; i < count; i++)
	{
		QDomNode item = xml.at(i);
		Vector3 v0, v1, v2;
		readVector3(item.firstChildElement("v0"), &v0);
		readVector3(item.firstChildElement("v1"), &v1);
		readVector3(item.firstChildElement("v2"), &v2);
		CollisionShape *collisionShape = new CollisionTriangle(&v0, &v1, &v2, CollisionGroup::STATIC_GROUP, &MATERIAL);
		setCollisionPrimitiveOrientation(collisionShape, item);
		vector->append(collisionShape);
	}
	return vector;
}


MyList<CollisionShape*>* CollisionGeometryParser::parse(const QDomElement &xml)
{
	QDomElement collision_geometry = xml.firstChildElement("collision-geometry");
	QDomNodeList collision_box = collision_geometry.elementsByTagName("collision-box");
	QDomNodeList collision_plane = collision_geometry.elementsByTagName("collision-plane");
	QDomNodeList collision_triangle = collision_geometry.elementsByTagName("collision-triangle");

	MyList<CollisionShape*> *v = new MyList<CollisionShape*>;
	putAll(parseCollisionPlanes(collision_plane), v);
	putAll(parseCollisionBoxes(collision_box), v);
	putAll(parseCollisionTrinagles(collision_triangle), v);
	return v;
}
