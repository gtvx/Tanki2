#include "VulkanWindow2.h"
#include "game/alternativa/tanks/services/battleinput/BattleInputService.h"
#include "game/alternativa/tanks/camera/GameCamera.h"
#include "hardware/alternativa/engine3d/core/View.h"
#include <QKeyEvent>



VulkanWindow2::VulkanWindow2()
{
	battlefieldModel = nullptr;
	camera = nullptr;
	battleInputService = nullptr;
}


void VulkanWindow2::render(VulkanWindow *vulkanWindow)
{
	camera->renderVulkan(vulkanWindow);
}


void VulkanWindow2::keyReleaseEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
		return;

	int key = event->nativeScanCode();

	battleInputService->onKeyUp(key);
}


void VulkanWindow2::keyPressEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
		return;

	int key = event->nativeScanCode();

	battleInputService->onKeyDown(key);
}


void VulkanWindow2::resizeEvent(QResizeEvent *event)
{
	QSize size = event->size();
	camera->view->width = size.width();
	camera->view->height = size.height();
}
