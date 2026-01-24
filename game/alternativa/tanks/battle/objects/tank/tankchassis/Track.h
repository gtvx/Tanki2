#ifndef TRACK_H
#define TRACK_H

#include <QVector>
#include "game/alternativa/math/Vector3.h"

class SuspensionRay;
class SuspensionParams;
class Body;

class Track
{
public:
	Track(Body *body, int numRays, Vector3 *vector, double value, SuspensionParams *suspensionParams, int side);
	bool hasContactsWithStatic();
	void calculateSuspensionContacts(double);
	void setCollisionGroup(int);
	void setAnimationSpeed(double _arg_1, double _arg_2);

	int numRays;
	SuspensionParams *suspensionParams;
	Body *body;
	QVector<SuspensionRay*> *rays;
	int side;
	int numContacts;
	Vector3 averageSurfaceVelocity;
	double animationSpeed;

private:
	void setTrackParams(int numRays, Vector3 *vector, double value, SuspensionParams *suspensionParams);
};

#endif // TRACK_H
