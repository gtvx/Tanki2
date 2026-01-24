#include "FPSService.h"
#include "getTimer.h"


FPSService::FPSService()
{
	fps = 0;
	frameTime = 0.;
}


void FPSService::start()
{
	//this->setFps(dispay.stage.frameRate);
	//dispay.stage.addEventListener(Event.ENTER_FRAME, this->onEnterFrame);
	//dispay.stage.addEventListener(Event.DEACTIVATE, this->onDeactivate);
	this->deactiave = false;
	this->starting = true;
}


void FPSService::onEnterFrame()
{
	if (this->deactiave)
	{
		return;
	}
	if (this->starting)
	{
		this->lastTime = getTimer();
		this->numFrames = 0;
		this->starting = false;
		return;
	}
	int _local_2 = getTimer();
	this->numFrames++;
	if ((_local_2 - this->lastTime) > 2000)
	{
		this->setFps((1000 * this->numFrames) / (_local_2 - this->lastTime));
		this->lastTime = _local_2;
		this->numFrames = 0;
	}
}


void FPSService::setFps(int v)
{
	this->fps = v;
	this->frameTime = (1000. / v);
}


int FPSService::getFps()
{
	return this->fps;
}


double FPSService::getFrameTimeMS()
{
	return this->frameTime;
}
