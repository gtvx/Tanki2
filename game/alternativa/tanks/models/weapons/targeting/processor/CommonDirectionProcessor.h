#ifndef COMMONDIRECTIONPROCESSOR_H
#define COMMONDIRECTIONPROCESSOR_H

class IRayCollisionFilter;
class Vector3;
class RayHit;
class AllGlobalGunParams;
class TanksCollisionDetector;

class CommonDirectionProcessor
{
	double maxDistance;
	IRayCollisionFilter *filter;
	bool shotFromMuzzle;
	TanksCollisionDetector *collisionDetector;

public:
	CommonDirectionProcessor(double maxDistance, IRayCollisionFilter *filter);
	virtual void setShotFromMuzzle();

protected:
	bool raycast(const Vector3*, const Vector3*, RayHit*);
	Vector3 *getOrigin(AllGlobalGunParams*, Vector3*);
};

#endif // COMMONDIRECTIONPROCESSOR_H
