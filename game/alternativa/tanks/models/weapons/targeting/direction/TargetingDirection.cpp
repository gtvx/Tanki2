#include "TargetingDirection.h"


TargetingDirection::TargetingDirection(Vector3 *direction, double angle, double maxPriority, double bonusPriority)
{
	this->direction.copy(direction);
	this->angle = angle;
	this->maxPriority = maxPriority;
	this->bonusPriority = bonusPriority;
}


void TargetingDirection::set(Vector3 *direction, double angle, double maxPriority, double bonusPriority)
{
	this->direction.copy(direction);
	this->angle = angle;
	this->maxPriority = maxPriority;
	this->bonusPriority = bonusPriority;
}


double TargetingDirection::comparator(TargetingDirection *a, TargetingDirection *b)
{
	return b->maxPriority - a->maxPriority;
}


Vector3* TargetingDirection::getDirection()
{
	return &this->direction;
}


double TargetingDirection::getAngle() const
{
	return this->angle;
}


double TargetingDirection::getMaxPriority() const
{
	return this->maxPriority;
}


double TargetingDirection::getBonusPriority() const
{
	return this->bonusPriority;
}
