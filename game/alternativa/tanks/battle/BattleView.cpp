#include "BattleView.h"
#include "game/alternativa/tanks/display/AxisIndicator.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/scene3d/BattleScene3D.h"
#include "hardware/alternativa/engine3d/core/Camera3D.h"
#include "game/alternativa/tanks/camera/GameCamera.h"
#include "FPSService.h"
#include "Canvas.h"
#include "Graphics.h"
#include <QString>

extern BattleService *battleService;


BattleView::BattleView()
{
	this->showAxisIndicator = false;
	this->last_fps = -1;

	setShowAxisIndicator(true);
	//this->container = this->createContainer();
	//this->view = new View(1, 1, GPUCapabilities.constrained);
	//this->view.mouseEnabled = false;
	//this->view.hideLogo();
	//this->container.addChild(this->view);
	//battleService.getBattleScene3D().setCameraView(this->view);
	//this->overlay = new Sprite();
	//this->overlay.mouseEnabled = false;
	//this->container.addChild(this->overlay);
	//this->viewportBorderLayer = new Shape();
	//this->container.addChild(this->viewportBorderLayer);
	//this->setSize(storageService.getStorage().data[SCREEN_SIZE_KEY]);
}


void BattleView::setShowAxisIndicator(bool enable)
{
	if (this->showAxisIndicator != enable)
	{
		this->showAxisIndicator = enable;
		if (enable)
		{
			this->axisIndicator = new AxisIndicator(100);
			//this->container.addChild(this->axisIndicator);
			this->setAxisIndicatorPosition();
		}
		else
		{
			//this->container.removeChild(this->axisIndicator);
			//this->axisIndicator = null;
		}
	}
}


void BattleView::setShowFPSIndicator(bool enable)
{
	//Camera3D *camera = battleService->getBattleScene3D()->getCamera();
	//if (enable != this->container.contains(camera.diagram))
	{
		if (enable)
		{
			//this->container.addChild(camera.diagram);
		}
		else
		{
			//this->container.removeChild(camera.diagram);
		}
	}
}


void BattleView::setAxisIndicatorPosition()
{
	if (this->showAxisIndicator)
	{
		//this->axisIndicator.y = ((this->view.height - this->axisIndicator.size) - 50);
	}
}

void BattleView::update(Canvas *canvas, FPSService *fpsService)
{
	if (this->showAxisIndicator)
		this->axisIndicator->update(canvas, battleService->getBattleScene3D()->getCamera());

	char fps = fpsService->getFps();
	if (fps != last_fps) {
		last_fps = fps;
		fps_str = QString::number(fps);
	}
	canvas->gfx->drawString(fps_str, 5, 100);
	canvas->gfx->end(0xFF0000);
}
