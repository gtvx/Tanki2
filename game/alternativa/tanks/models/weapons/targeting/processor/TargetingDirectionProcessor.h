#ifndef TARGETINGDIRECTIONPROCESSOR_H
#define TARGETINGDIRECTIONPROCESSOR_H

#include <QVector>

class RayHit;
class AllGlobalGunParams;
class Vector3;

class TargetingDirectionProcessor
{
public:
	virtual QVector<RayHit>* process(AllGlobalGunParams*, Vector3*) = 0;
	virtual void setShotFromMuzzle() = 0;
};

#endif // TARGETINGDIRECTIONPROCESSOR_H
