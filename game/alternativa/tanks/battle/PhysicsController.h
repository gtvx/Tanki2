#ifndef PHYSICSCONTROLLER_H
#define PHYSICSCONTROLLER_H

class PhysicsController
{
public:
	virtual void runBeforePhysicsUpdate(double) = 0;
};

#endif // PHYSICSCONTROLLER_H
