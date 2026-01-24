#include "PhysicsMaterial.h"

const PhysicsMaterial *PhysicsMaterial::DEFAULT_MATERIAL = new PhysicsMaterial;


PhysicsMaterial::PhysicsMaterial(double restitution, double friction)
{
	this->restitution = restitution;
	this->friction = friction;
}
