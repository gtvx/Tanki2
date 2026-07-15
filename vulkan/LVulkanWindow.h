#ifndef LVULKANWINDOW_H
#define LVULKANWINDOW_H

#include <QWindow>
#include "VulkanWindow.h"


struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


class LVulkanWindow : public QWindow, public VulkanWindow
{
	VulkanFunctions *m_funcs;
	VkInstance instance;


	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	std::vector<VkCommandBuffer> commandBuffers;
	VkRenderPass renderPass;


	VkExtent2D swapChainExtent;
	VkPipeline graphicsPipeline;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkCommandPool commandPool;

	VkSurfaceKHR surface;

	VkPhysicalDevice m_physicalDevice;
	VkDevice dev;

	VkSwapchainKHR swapChain;

	VkSampleCountFlagBits sampleCount;

	VkPipelineCache m_pipelineCache;

	uint32_t hostVisibleMemIndex;

	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;


	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;


	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	uint32_t m_currentFrame = 0;

	bool framebufferResized = false;

	VkDebugUtilsMessengerEXT debugMessenger;



	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;


	VkSampleCountFlagBits msaaSamples;

	int last_width, last_height;

	bool m_renderEnable;


	uint32_t imageIndex;

	void createCommandBuffer();
	void createSyncObjects();
	void createCommandPool();

protected:

	virtual void initVulkan(VulkanWindow *vulkanWindow) = 0;
	virtual void render(VulkanWindow *vulkanWindow) = 0;

public:
	LVulkanWindow();

	void setVulkanInstance(VkInstance instance);

	void init();

	void setFunctions(VulkanFunctions *functions);
	VulkanFunctions* getFunctions();

	VkDevice device() override;


	bool isInit() override;

	bool isDebugEnabled() override;

	VkPipelineCache getPipelineCache();

	VkCommandBuffer currentCommandBuffer() override;

	VkRenderPass defaultRenderPass() override;

	VkPhysicalDevice physicalDevice() override;

	uint32_t hostVisibleMemoryIndex() override;

	uint32_t deviceLocalMemoryIndex() override;

	int concurrentFrameCount() override;

	VkSampleCountFlagBits sampleCountFlagBits() override;

	int currentFrame() override;

	void renderEnable(bool enable) override;
	bool isRenderEnable() override;

	bool renderBegin1() override;
	bool renderBegin2() override;
	void renderEnd() override;

private:
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void initResources();
	void pickPhysicalDevice();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	void createSurface();
	void createLogicalDevice();
	void createSwapChain();
	void createFramebuffers();
	void createImageViews();
	void createRenderPass();
	void setupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
	VkFormat findDepthFormat();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createDepthResources();
	void createColorResources();
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void cleanupSwapChain();
	void recreateSwapChain();
};

#endif // LVULKANWINDOW_H
