#ifndef ADDPOSTPHYSICSCONTROLLERACTION_H
#define ADDPOSTPHYSICSCONTROLLERACTION_H

#include "../DeferredAction.h"

class PostPhysicsController;

// BattleRunnerProvider implements DeferredAction
class AddPostPhysicsControllerAction : public DeferredAction
{
	PostPhysicsController *controller;

public:
	AddPostPhysicsControllerAction(PostPhysicsController *controller);

private:
	void execute() override;
};

#endif // ADDPOSTPHYSICSCONTROLLERACTION_H
