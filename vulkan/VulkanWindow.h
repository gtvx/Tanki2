#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H

#include <vulkan/vulkan.h>
#include <QList>

class VulkanFunctions;

class VulkanWindow
{
public:
    virtual VkDevice device() = 0;
    virtual VulkanFunctions* getFunctions() = 0;
    virtual VkPhysicalDevice physicalDevice() = 0;
    virtual uint32_t hostVisibleMemoryIndex() = 0;
    virtual uint32_t deviceLocalMemoryIndex() = 0;
    virtual bool isInit() = 0;
    virtual VkPipelineCache getPipelineCache() = 0;
    virtual int concurrentFrameCount() = 0;
    virtual VkSampleCountFlagBits sampleCountFlagBits() = 0;
    virtual VkRenderPass defaultRenderPass() = 0;
    virtual VkCommandBuffer currentCommandBuffer() = 0;
    virtual int currentFrame() = 0;
    //virtual const QList<int> supportedSampleCounts() = 0;
    virtual bool isDebugEnabled() = 0;

	virtual void render1() = 0;
};

#endif // VULKANWINDOW_H
