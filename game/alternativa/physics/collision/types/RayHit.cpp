#include "RayHit.h"


void RayHit::copy(const RayHit *rayHit)
{
	this->shape = rayHit->shape;
	this->position.copy(&rayHit->position);
	this->normal.copy(&rayHit->normal);
	this->t = rayHit->t;
	this->staticHit = rayHit->staticHit;
}


void RayHit::clear()
{
	this->shape = nullptr;
}
