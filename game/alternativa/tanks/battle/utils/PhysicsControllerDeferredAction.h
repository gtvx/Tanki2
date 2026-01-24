#ifndef PHYSICSCONTROLLERDEFERREDACTION_H
#define PHYSICSCONTROLLERDEFERREDACTION_H

#include "../DeferredAction.h"

class PhysicsController;

//BattleRunnerProvider implements DeferredAction

class PhysicsControllerDeferredAction : public DeferredAction
{
	PhysicsController *controller;
	bool add;

public:
	PhysicsControllerDeferredAction(PhysicsController *controller, bool add);

private:
	void execute() override;
};

#endif // PHYSICSCONTROLLERDEFERREDACTION_H
