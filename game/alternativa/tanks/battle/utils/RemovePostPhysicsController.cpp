#include "RemovePostPhysicsController.h"
#include "getBattleRunner.h"


RemovePostPhysicsController::RemovePostPhysicsController(PostPhysicsController *controller)
{
	this->controller = controller;
}

void RemovePostPhysicsController::execute()
{
	getBattleRunner()->removePostPhysicsController(this->controller);
}
