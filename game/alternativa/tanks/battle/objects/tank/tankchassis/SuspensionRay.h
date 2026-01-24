#ifndef SUSPENSIONRAY_H
#define SUSPENSIONRAY_H

#include "game/alternativa/physics/collision/types/RayHit.h"

class SuspensionParams;
class Body;
class RayCollisionFilter;

class SuspensionRay
{
	Vector3 origin;
	Vector3 direction;
	Vector3 globalOrigin;
	Vector3 globalDirection;
	Body *body;
	SuspensionParams *suspensionParams;
	RayCollisionFilter *collisionFilter;
	double prevCompression;

public:
	SuspensionRay(Body *param1, Vector3 *param2, Vector3 *param3, SuspensionParams *param4);
	Vector3 *getOrigin();
	void update(double param1);
	void calculateSpringForce(double param1);
	Vector3 *getGlobalOrigin();
	Vector3 *getGlobalDirection();

	RayHit *rayHit;
	Vector3 contactVelocity;
	double springForce;
	double speed;
	int collisionGroup;
	bool hasCollision;

private:
	void raycast();
	void calculateContactVelocity();
};

#endif // SUSPENSIONRAY_H
