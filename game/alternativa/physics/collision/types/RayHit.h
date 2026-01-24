#ifndef RAYHIT_H
#define RAYHIT_H

#include "game/alternativa/math/Vector3.h"

class CollisionShape;

class RayHit
{
public:
	RayHit()
	{
		shape = nullptr;
		t = 0;
		staticHit = false;
	}

	CollisionShape *shape;
	Vector3 position;
	Vector3 normal;
	double t;
	bool staticHit;

	void copy(const RayHit*);
	void clear();
};

#endif // RAYHIT_H
