#ifndef PHYSICSINTERPOLATOR_H
#define PHYSICSINTERPOLATOR_H

class PhysicsInterpolator
{
public:
	virtual void interpolatePhysicsState(double, int) = 0;
};

#endif // PHYSICSINTERPOLATOR_H
