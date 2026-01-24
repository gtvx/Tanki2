#ifndef VK_INDEXBUFFER3D_H
#define VK_INDEXBUFFER3D_H

#include <QVector>
#include <vulkan/vulkan.h>

class VulkanFunctions;

class VK_IndexBuffer3D
{
    VulkanFunctions *m_devFuncs;
    VkDevice dev;
    VkDeviceMemory memory;
    VkBuffer buffer;

public:
	VK_IndexBuffer3D();

    VkBuffer get() { return buffer; }

	bool init(VulkanFunctions *m_devFuncs, VkDevice dev, uint32_t hostVisibleMemoryIndex, int numIndices);

    bool uploadFromVector(const QVector<uint32_t> &data, int startOffset, int count);
    bool uploadFromVector(const uint16_t *data, int startOffset, int count);
};

#endif // VK_INDEXBUFFER3D_H
