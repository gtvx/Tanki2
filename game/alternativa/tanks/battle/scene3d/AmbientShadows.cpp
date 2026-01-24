#include "AmbientShadows.h"
#include "hardware/alternativa/engine3d/core/Shadow.h"


AmbientShadows::AmbientShadows(Camera3D *camera)
{
	color = 0;
	alpha = 0.85;
	nearDistance = 4000;
	farDistance = 7000;
	this->camera = camera;
	this->registerConsoleCommands();
}


void AmbientShadows::registerConsoleCommands()
{

}


void AmbientShadows::add(Shadow *)
{
	/*
   shadow->color = this->color;
   shadow->alpha = this->alpha;
   shadow->nearDistance = this->nearDistance;
   shadow->farDistance = this->farDistance;
   this->shadows[param1] = true;
   if (this->enabled)
   {
	  this->camera->addShadow(param1);
   }
   */
}
