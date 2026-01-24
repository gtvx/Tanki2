#ifndef REMOVEPOSTPHYSICSCONTROLLER_H
#define REMOVEPOSTPHYSICSCONTROLLER_H

#include "../DeferredAction.h"

class PostPhysicsController;

//BattleRunnerProvider implements DeferredAction
class RemovePostPhysicsController : public DeferredAction
{
	PostPhysicsController *controller;

public:
	RemovePostPhysicsController(PostPhysicsController *controller);

private:
	void execute() override;
};

#endif // REMOVEPOSTPHYSICSCONTROLLER_H
