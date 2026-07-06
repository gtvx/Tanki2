#ifndef EVULKANWINDOW_H
#define EVULKANWINDOW_H

#include <QWindow>
#include "VulkanWindow.h"

class TankPreview;

class EVulkanWindow : public QWindow, public VulkanWindow
{
protected:

	virtual void initVulkan(VulkanWindow *vulkanWindow) = 0;
	virtual void render(VulkanWindow *vulkanWindow) = 0;

public:

    enum MStatus
    {
        StatusFail,
        StatusDeviceReady,
        StatusUninitialized,
        StatusFailRetry,
        StatusReady
    };

    MStatus status;


	EVulkanWindow();

    void setVulkanInstance(VkInstance instance);

    bool isInit() override;

    bool isDebugEnabled() override;

    void setFunctions(VulkanFunctions *functions);
    VulkanFunctions* getFunctions();

    VkPipelineCache getPipelineCache();

    VkDevice device() override;

    //const QList<int> supportedSampleCounts() override;

    VkCommandBuffer currentCommandBuffer() override;

    VkRenderPass defaultRenderPass() override;

    VkPhysicalDevice physicalDevice() override;

    uint32_t hostVisibleMemoryIndex() override;

    uint32_t deviceLocalMemoryIndex() override;

    int concurrentFrameCount() override;

    VkSampleCountFlagBits sampleCountFlagBits() override;

    int currentFrame() override;


    bool init();

	void render1() override;

	uint32_t maxPushConstantsSize()
	{
		return m_maxPushConstantsSize;
	}

private:

    VkPhysicalDevice physDev;
    uint32_t gfxQueueFamilyIdx;
    uint32_t presQueueFamilyIdx;
    VkRenderPass m_defaultRenderPass;
    VkDevice dev;
    VkCommandPool presCmdPool;
    VkCommandPool cmdPool;

    VkFormat dsFormat;
    VkFormat colorFormat;
    VkColorSpaceKHR colorSpace;

    uint32_t hostVisibleMemIndex;
    VkSampleCountFlagBits sampleCount;

    VkQueue presQueue;
    VkQueue gfxQueue;

    struct FrameResources {
          VkFence fence = VK_NULL_HANDLE;
          bool fenceWaitable = false;
          VkSemaphore imageSem = VK_NULL_HANDLE;
          VkSemaphore drawSem = VK_NULL_HANDLE;
          VkSemaphore presTransSem = VK_NULL_HANDLE;
          bool imageAcquired = false;
          bool imageSemWaitable = false;
    } frameRes;

    static const int MAX_SWAPCHAIN_BUFFER_COUNT = 5;

    struct ImageResources {
        VkImage image = VK_NULL_HANDLE;
        VkImageView imageView = VK_NULL_HANDLE;
        VkCommandBuffer cmdBuf = VK_NULL_HANDLE;
        VkFence cmdFence = VK_NULL_HANDLE;
        bool cmdFenceWaitable = false;
        VkFramebuffer fb = VK_NULL_HANDLE;
        VkCommandBuffer presTransCmdBuf = VK_NULL_HANDLE;
        VkImage msaaImage = VK_NULL_HANDLE;
        VkImageView msaaImageView = VK_NULL_HANDLE;
    } imageRes[MAX_SWAPCHAIN_BUFFER_COUNT];

    bool framePending;
    bool frameGrabbing;
    uint32_t m_currentImage;
    VkSwapchainKHR swapChain;
    QSize m_swapChainImageSize;
    VkImage dsImage;
    VkImageView dsView;
    VkDeviceMemory dsMem;
    VkDeviceMemory msaaImageMem;
    uint32_t swapChainSupportsReadBack;
    uint32_t swapChainBufferCount;
	uint32_t m_maxPushConstantsSize;
	VkPipelineCache m_pipelineCache;
	VulkanFunctions *m_devFuncs;
	VkInstance instance;
	VkSurfaceKHR surface;

	bool createDefaultRenderPass();
	void initResources();
    void beginFrame();
    void recreateSwapChain();
    void releaseSwapChain();
    bool createTransientImage(VkFormat format, VkImageUsageFlags usage, VkImageAspectFlags aspectMask, VkImage *images, VkDeviceMemory *mem, VkImageView *views, int count);
    uint32_t chooseTransientImageMemType(VkImage img, uint32_t startIndex);
    void startNextFrame();
    void endFrame();
    bool checkDeviceLost(VkResult err, int id);
    void reset();
    void ensureStarted();
    QSize surfacePixelSize() const;
    VkFramebuffer currentFramebuffer();
	bool event(QEvent *e) override;
	void exposeEvent(QExposeEvent *) override;
};

#endif // EVULKANWINDOW_H
