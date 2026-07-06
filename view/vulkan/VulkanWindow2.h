#ifndef VULKANWINDOW2_H
#define VULKANWINDOW2_H

#include "vulkan/EVulkanWindow.h"

class BattlefieldModel;
class GameCamera;
class BattleInputService;

class VulkanWindow2 : public EVulkanWindow
{
	BattlefieldModel *battlefieldModel;
	GameCamera *camera;
	BattleInputService *battleInputService;

	void initVulkan(VulkanWindow *vulkanWindow) override;
	void render(VulkanWindow *vulkanWindow) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

public:

	VulkanWindow2();

	void setBattlefieldModel(BattlefieldModel *battlefieldModel)
	{
		this->battlefieldModel = battlefieldModel;
	}

	void setGameCamera(GameCamera *camera)
	{
		this->camera = camera;
	}

	void setBattleInputService(BattleInputService *battleInputService)
	{
		this->battleInputService = battleInputService;
	}
};

#endif // VULKANWINDOW2_H
