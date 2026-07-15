#ifndef VULKANFUNCTIONS_H
#define VULKANFUNCTIONS_H

#include <vulkan/vulkan.h>

#ifdef linux
class VkXlibSurfaceCreateInfoKHR;
class VkXcbSurfaceCreateInfoKHR;
#endif

#ifdef WIN32
class VkWin32SurfaceCreateInfoKHR;
#endif


class VulkanFunctions
{
public:
    VulkanFunctions();

	PFN_vkVoidFunction VKAPI_CALL (*vkGetInstanceProcAddr)(VkInstance instance, const char* pName);


    VkResult VKAPI_CALL (*vkEnumerateInstanceLayerProperties)(uint32_t* pPropertyCount, VkLayerProperties* pProperties);

    VkResult VKAPI_CALL (*vkEnumerateInstanceExtensionProperties)(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);



    VkResult VKAPI_CALL (*vkCreateInstance)(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);

	VkResult VKAPI_CALL (*vkEnumeratePhysicalDevices)(VkInstance instance, uint32_t *pPhysicalDeviceCount, VkPhysicalDevice *pPhysicalDevices);

#ifdef linux
    VkResult VKAPI_CALL (*vkCreateXlibSurfaceKHR)(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);

    VkResult VKAPI_CALL (*vkCreateXcbSurfaceKHR)(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif

#ifdef WIN32
    VkResult VKAPI_CALL (*vkCreateWin32SurfaceKHR)(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
#endif


	void VKAPI_CALL (*vkGetPhysicalDeviceFormatProperties)(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties *pFormatProperties);

	void VKAPI_CALL (*vkGetPhysicalDeviceMemoryProperties)(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties *pMemoryProperties);

	void VKAPI_CALL (*vkGetPhysicalDeviceProperties)(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties *pProperties);

	void VKAPI_CALL (*vkGetPhysicalDeviceFeatures)(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures *pFeatures);

	VkResult VKAPI_CALL (*vkCreateDevice)(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDevice *pDevice);

	VkResult VKAPI_CALL (*vkEnumerateDeviceLayerProperties)(VkPhysicalDevice physicalDevice, uint32_t *pPropertyCount, VkLayerProperties *pProperties);

	VkResult VKAPI_CALL (*vkEnumerateDeviceExtensionProperties)(VkPhysicalDevice physicalDevice, const char *pLayerName, uint32_t *pPropertyCount, VkExtensionProperties *pProperties);

    VkResult VKAPI_CALL (*vkGetPhysicalDeviceSurfaceFormatsKHR)(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);

    VkResult VKAPI_CALL (*vkGetPhysicalDeviceSurfaceCapabilitiesKHR)(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);

    VkResult VKAPI_CALL (*vkGetPhysicalDeviceSurfaceSupportKHR)(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);

    void VKAPI_CALL (*vkGetPhysicalDeviceQueueFamilyProperties)(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);

	VkResult VKAPI_CALL (*vkGetPhysicalDeviceSurfacePresentModesKHR)(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);

	void VKAPI_CALL (*vkCmdBeginRenderPass)(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo *pRenderPassBegin, VkSubpassContents contents);

	void VKAPI_CALL (*vkCmdEndRenderPass)(VkCommandBuffer commandBuffer);

	void VKAPI_CALL (*vkCmdSetViewport)(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport *pViewports);

	void VKAPI_CALL (*vkCmdSetScissor)(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D *pScissors);

	void VKAPI_CALL (*vkCmdPipelineBarrier)(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier *pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier *pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier *pImageMemoryBarriers);

	void VKAPI_CALL (*vkCmdBindPipeline)(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);

	void VKAPI_CALL (*vkCmdBindVertexBuffers)(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer *pBuffers, const VkDeviceSize *pOffsets);

	void VKAPI_CALL (*vkCmdBindIndexBuffer)(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);

	void VKAPI_CALL (*vkCmdCopyImage)(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy *pRegions);

	void VKAPI_CALL (*vkCmdPushConstants)(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void *pValues);

	void VKAPI_CALL (*vkCmdDrawIndexed)(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);

	VkResult VKAPI_CALL (*vkBeginCommandBuffer)(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo *pBeginInfo);

	VkResult VKAPI_CALL (*vkEndCommandBuffer)(VkCommandBuffer commandBuffer);

	VkResult VKAPI_CALL (*vkResetCommandBuffer)(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);

	VkResult VKAPI_CALL (*vkQueueSubmit)(VkQueue queue, uint32_t submitCount, const VkSubmitInfo *pSubmits, VkFence fence);

    VkResult VKAPI_CALL (*vkQueuePresentKHR)(VkQueue queue, const VkPresentInfoKHR* pPresentInfo);




	void VKAPI_CALL (*vkFreeMemory)(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator);

	VkResult VKAPI_CALL (*vkMapMemory)(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void **ppData);

	void VKAPI_CALL (*vkUnmapMemory)(VkDevice device, VkDeviceMemory memory);

	VkResult VKAPI_CALL (*vkCreatePipelineCache)(VkDevice device, const VkPipelineCacheCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkPipelineCache *pPipelineCache);

	void VKAPI_CALL (*vkDestroyBuffer)(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks *pAllocator);

	void VKAPI_CALL (*vkDestroyPipelineCache)(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks *pAllocator);

	VkResult VKAPI_CALL (*vkCreateShaderModule)(VkDevice device, const VkShaderModuleCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkShaderModule *pShaderModule);

	VkResult VKAPI_CALL (*vkCreateSampler)(VkDevice device, const VkSamplerCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkSampler *pSampler);

	VkResult VKAPI_CALL (*vkCreateDescriptorPool)(VkDevice device, const VkDescriptorPoolCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDescriptorPool *pDescriptorPool);

	VkResult VKAPI_CALL (*vkCreateDescriptorSetLayout)(VkDevice device, const VkDescriptorSetLayoutCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDescriptorSetLayout *pSetLayout);

	VkResult VKAPI_CALL (*vkAllocateDescriptorSets)(VkDevice device, const VkDescriptorSetAllocateInfo *pAllocateInfo, VkDescriptorSet *pDescriptorSets);

	void VKAPI_CALL (*vkUpdateDescriptorSets)(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet *pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet *pDescriptorCopies);

	VkResult VKAPI_CALL (*vkCreatePipelineLayout)(VkDevice device, const VkPipelineLayoutCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkPipelineLayout *pPipelineLayout);

	VkResult VKAPI_CALL (*vkCreateGraphicsPipelines)(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo *pCreateInfos, const VkAllocationCallbacks *pAllocator, VkPipeline *pPipelines);

	VkResult VKAPI_CALL (*vkCreateBuffer)(VkDevice device, const VkBufferCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkBuffer *pBuffer);

	VkResult VKAPI_CALL (*vkAllocateMemory)(VkDevice device, const VkMemoryAllocateInfo *pAllocateInfo, const VkAllocationCallbacks *pAllocator, VkDeviceMemory *pMemory);

	VkResult VKAPI_CALL (*vkBindBufferMemory)(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);

	VkResult VKAPI_CALL (*vkCreateImageView)(VkDevice device, const VkImageViewCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkImageView *pView);

	VkResult VKAPI_CALL (*vkCreateImage)(VkDevice device, const VkImageCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkImage *pImage);

	void VKAPI_CALL (*vkGetImageMemoryRequirements)(VkDevice device, VkImage image, VkMemoryRequirements *pMemoryRequirements);

	VkResult VKAPI_CALL (*vkBindImageMemory)(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);

	void VKAPI_CALL (*vkGetImageSubresourceLayout)(VkDevice device, VkImage image, const VkImageSubresource *pSubresource, VkSubresourceLayout *pLayout);

	VkResult VKAPI_CALL (*vkDeviceWaitIdle)(VkDevice device);

    VkResult VKAPI_CALL (*vkGetSwapchainImagesKHR)( VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);

	VkResult VKAPI_CALL (*vkCreateFence)(VkDevice device, const VkFenceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkFence *pFence);

	VkResult VKAPI_CALL (*vkCreateSemaphore)(VkDevice device, const VkSemaphoreCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkSemaphore *pSemaphore);

	void VKAPI_CALL (*vkDestroyFence)(VkDevice device, VkFence fence, const VkAllocationCallbacks *pAllocator);

	void VKAPI_CALL (*vkDestroySemaphore)(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks *pAllocator);

	void VKAPI_CALL (*vkDestroyFramebuffer)(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks *pAllocator);

	void VKAPI_CALL (*vkDestroyImageView)(VkDevice device, VkImageView imageView, const VkAllocationCallbacks *pAllocator);

	void VKAPI_CALL (*vkFreeCommandBuffers)(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer *pCommandBuffers);

	void VKAPI_CALL (*vkDestroyImage)(VkDevice device, VkImage image, const VkAllocationCallbacks *pAllocator);

    void VKAPI_CALL (*vkDestroySwapchainKHR)(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);

	VkResult VKAPI_CALL (*vkCreateRenderPass)(VkDevice device, const VkRenderPassCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkRenderPass *pRenderPass);

	VkResult VKAPI_CALL (*vkWaitForFences)(VkDevice device, uint32_t fenceCount, const VkFence *pFences, VkBool32 waitAll, uint64_t timeout);

	VkResult VKAPI_CALL (*vkResetFences)(VkDevice device, uint32_t fenceCount, const VkFence *pFences);

	VkResult VKAPI_CALL (*vkAllocateCommandBuffers)(VkDevice device, const VkCommandBufferAllocateInfo *pAllocateInfo, VkCommandBuffer *pCommandBuffers);

	void VKAPI_CALL (*vkDestroyRenderPass)(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks *pAllocator);

	void VKAPI_CALL (*vkDestroyCommandPool)(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks *pAllocator);

	void VKAPI_CALL (*vkDestroyDevice)(VkDevice device, const VkAllocationCallbacks *pAllocator);

    VkResult VKAPI_CALL (*vkCreateSwapchainKHR)(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);

	void VKAPI_CALL (*vkGetDeviceQueue)(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue *pQueue);

	VkResult VKAPI_CALL (*vkCreateCommandPool)(VkDevice device, const VkCommandPoolCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkCommandPool *pCommandPool);

	VkResult VKAPI_CALL (*vkCreateFramebuffer)(VkDevice device, const VkFramebufferCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkFramebuffer *pFramebuffer);

    VkResult VKAPI_CALL (*vkAcquireNextImageKHR)(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);

	void VKAPI_CALL (*vkGetBufferMemoryRequirements)(VkDevice device, VkBuffer buffer, VkMemoryRequirements *pMemoryRequirements);

	void VKAPI_CALL (*vkCmdBindDescriptorSets)(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet *pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t *pDynamicOffsets);

	void VKAPI_CALL (*vkCmdDraw)(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

	void VKAPI_CALL (*vkCmdBlitImage)(
		VkCommandBuffer                             commandBuffer,
		VkImage                                     srcImage,
		VkImageLayout                               srcImageLayout,
		VkImage                                     dstImage,
		VkImageLayout                               dstImageLayout,
		uint32_t                                    regionCount,
		const VkImageBlit*                          pRegions,
		VkFilter                                    filter);

    bool load();
};

#endif // VULKANFUNCTIONS_H
