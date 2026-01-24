#ifndef HITINFO_H
#define HITINFO_H

#include "game/alternativa/math/Vector3.h"

class Body;
class AllGlobalGunParams;
class TargetingResult;
class RayHit;

class HitInfo
{
public:
	HitInfo()
	{
		this->distance = 0;
		this->body = nullptr;
	}

	double distance;
	Body *body;
	Vector3 position;
	Vector3 direction;
	Vector3 normal;

	void setResult(AllGlobalGunParams *allGlobalGunParams, TargetingResult *targetingResult);

private:
	void setHit(const Vector3 *a, const Vector3 *b, const RayHit *rayHit);
	void setMiss(const Vector3 *a);
};

#endif // HITINFO_H
