#ifndef POSTPHYSICSCONTROLLER_H
#define POSTPHYSICSCONTROLLER_H

class PostPhysicsController
{
public:
	virtual void runAfterPhysicsUpdate(double) = 0;
};

#endif // POSTPHYSICSCONTROLLER_H
