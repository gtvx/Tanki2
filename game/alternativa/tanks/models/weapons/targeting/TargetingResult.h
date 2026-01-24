#ifndef TARGETINGRESULT_H
#define TARGETINGRESULT_H

#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include <QVector>

class TargetingResult
{
	Vector3 direction;
	bool staticHit_e;
	RayHit staticHit;
	QVector<RayHit> hits;

public:
	TargetingResult();
	bool hasStaticHit();
	RayHit *getStaticHit();
	Vector3 *getDirection();
	QVector<RayHit> *getHits();
	bool hasTankHit();
	bool hasAnyHit();
	void setData(Vector3 *a, QVector<RayHit> *b);
	RayHit *getSingleHit();
};

#endif // TARGETINGRESULT_H
