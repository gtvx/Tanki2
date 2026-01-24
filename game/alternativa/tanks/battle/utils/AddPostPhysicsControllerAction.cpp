#include "AddPostPhysicsControllerAction.h"
#include "getBattleRunner.h"


AddPostPhysicsControllerAction::AddPostPhysicsControllerAction(PostPhysicsController *controller)
{
	this->controller = controller;
}

void AddPostPhysicsControllerAction::execute()
{
	getBattleRunner()->addPostPhysicsController(this->controller);
}
