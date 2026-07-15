#ifndef VULKANUNIFORM_H
#define VULKANUNIFORM_H

class VulkanWindow;

#include <vulkan/vulkan.h>

class VulkanUniform
{
	VulkanWindow *vulkanWindow;
	VkDeviceMemory m_bufMem;
	VkBuffer m_uniBuf;
	uint32_t m_binding;

public:
	VulkanUniform();

	bool init(VulkanWindow *vulkanWindow, uint32_t binding);
	bool upload(void *data, int offset, int size);

	VkBuffer buffer() { return m_uniBuf; }
	uint32_t binding() { return m_binding; }
};

#endif // VULKANUNIFORM_H
