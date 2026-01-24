#ifndef MARGINALCOLLIDER_H
#define MARGINALCOLLIDER_H

class Vector3;

class MarginalCollider
{
public:
	MarginalCollider();

	static bool segmentWithStaticIntersection(const Vector3*, const Vector3*);
	static void setupCollisionBox(const Vector3*, const Vector3*);
};

#endif // MARGINALCOLLIDER_H
