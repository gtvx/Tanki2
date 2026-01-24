#ifndef VK_CREATEINSTANCE_H
#define VK_CREATEINSTANCE_H

#include <vulkan/vulkan.h>

class VulkanFunctions;

VkInstance VK_createInstance(VulkanFunctions *functions);

#endif // VK_CREATEINSTANCE_H
