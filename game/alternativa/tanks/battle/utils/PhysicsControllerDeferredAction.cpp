#include "PhysicsControllerDeferredAction.h"
#include "getBattleRunner.h"

PhysicsControllerDeferredAction::PhysicsControllerDeferredAction(PhysicsController *controller, bool add)
{
	this->controller = controller;
	this->add = add;
}

void PhysicsControllerDeferredAction::execute()
{
	if (this->add)
	{
		getBattleRunner()->addPhysicsController(this->controller);
	}
	else
	{
		getBattleRunner()->removePhysicsController(this->controller);
	}
}
