#ifndef RAYCOLLISIONFILTER_H
#define RAYCOLLISIONFILTER_H

#include "game/alternativa/physics/collision/IRayCollisionFilter.h"

class RayCollisionFilter : public IRayCollisionFilter
{
	Body *body;

public:
	RayCollisionFilter(Body*);
	bool considerBody(Body *body);
	void setBody(Body*);
	Body* getBody();
};

#endif // RAYCOLLISIONFILTER_H
