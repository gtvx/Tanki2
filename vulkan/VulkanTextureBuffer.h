#ifndef VULKANTEXTUREBUFFER_H
#define VULKANTEXTUREBUFFER_H

#include <vulkan/vulkan.h>
#include <QSize>

class VulkanFunctions;
class VulkanWindow;
class QImage;


class VulkanTextureBuffer
{
    VkFormat m_texFormat;

    VkImage m_texImage;

    VkDeviceMemory m_texMem;

    bool m_texLayoutPending;

    QSize m_texSize;

    VkImage m_texStaging;

    VkDeviceMemory m_texStagingMem;

    bool m_texStagingPending;

    VkImageView m_texView;

    VulkanFunctions *m_devFuncs;

    VulkanWindow *vulkanWindow;


public:

    VulkanTextureBuffer();

    VkImage getTexStaging();
    VkImage getTexImageg();
    QSize size();
    VkImageView texView();

    bool createTexture(VulkanWindow *vulkanWindow, const QImage &image);


    bool createTextureImage(const QSize &size,
                            VkImage *image,
                            VkDeviceMemory *mem,
                            VkImageTiling tiling,
                            VkImageUsageFlags usage,
                            uint32_t memIndex);


    bool writeLinearImage(const QImage &img, VkImage image, VkDeviceMemory memory);

    void ensureTexture(VkCommandBuffer cb);
};

#endif // VULKANTEXTUREBUFFER_H
