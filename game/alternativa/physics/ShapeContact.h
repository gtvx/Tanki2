#ifndef SHAPECONTACT_H
#define SHAPECONTACT_H

#include "game/alternativa/math/Vector3.h"

class CollisionShape;
class Matrix3;

class ShapeContact
{
public:
	ShapeContact();
	void calculatePersistentFrameData();
	void calcualteDynamicFrameData(double param1, double param2, double param3, double param4);
	void dispose();
	double getSeparationVelocity();
	static ShapeContact *create();

	CollisionShape *shape1;
	CollisionShape *shape2;
	double normalSpeedDelta;
	double tangentSpeedDelta1;
	double tangentSpeedDelta2;
	double contactSeparationSpeed;
	double penetration;
	double tangentAngularInertiaTerm21;
	double normalAngularInertiaTerm2;
	double tangentAngularInertiaTerm22;
	double normalAngularInertiaTerm1;
	double tangentAngularInertiaTerm11;
	double tangentAngularInertiaTerm12;
	double restitution;
	double friction;
	Vector3 position;
	Vector3 normal;
	Vector3 tangent1;
	Vector3 tangent2;
	Vector3 r1;
	Vector3 r2;
	double normalImpulse;
	double tangentImpulse1;
	double tangentImpulse2;
	double collisionSpeed;
	bool satisfied;

private:
	double calculateAngularInertiaTerm(Vector3 *param1, Vector3 *param2, Matrix3 *param3);
};

#endif // SHAPECONTACT_H

