#ifndef TANKBODY_H
#define TANKBODY_H

#include "game/alternativa/math/Vector3.h"
#include <QVector>

class Vector3;
class Body;
class CollisionShape;
class CollisionBox;
class SoaringChecker;

class TankBody
{
	SoaringChecker *soaringChecker;

public:
	TankBody(Body *body);

	void updateSoaring();
	bool hasContactsWithOtherBodies();
	bool isSoaring();
	void onTankInited();
	void destroy();

	bool hasContactsWithStatic;
	bool wasContactWithStatic;
	double previousUpVelocity;
	Vector3 additionForcesSum;
	Body *body;
	QVector<Body*> penetratedBodies;
	QVector<CollisionShape*> staticShapes;
	CollisionBox *tankCollisionBox;
	int id;
};

#endif // TANKBODY_H
