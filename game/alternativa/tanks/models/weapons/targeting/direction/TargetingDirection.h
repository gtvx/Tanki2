#ifndef TARGETINGDIRECTION_H
#define TARGETINGDIRECTION_H

#include "game/alternativa/math/Vector3.h"

class TargetingDirection
{
	Vector3 direction;
	double angle;
	double maxPriority;
	double bonusPriority;

public:
	TargetingDirection(Vector3 *direction, double angle, double maxPriority, double bonusPriority = 0);
	void set(Vector3 *direction, double angle, double maxPriority, double bonusPriority);
	double comparator(TargetingDirection*, TargetingDirection*);
	Vector3 *getDirection();
	double getAngle() const;
	double getMaxPriority() const;
	double getBonusPriority() const;
};

#endif // TARGETINGDIRECTION_H
