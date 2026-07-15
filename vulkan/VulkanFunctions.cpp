#include "VulkanFunctions.h"
#include <QLibrary>
#include <QDebug>


class Library
{
    QLibrary library;

public:

    Library()
    {
#ifdef WIN32
        library.setFileName("vulkan-1.dll");
#endif

#ifdef linux
        library.setFileName("libvulkan.so");
#endif

    }

    bool load()
    {
        return library.load();
    }

    void resolve(const char *symbol, void *fun)
    {
        quintptr *ptr = (quintptr*)fun;
        (void)fun;
        *ptr = (quintptr)library.resolve(symbol);
        if (*ptr == 0)
        {
            qDebug() << "error load" << symbol;
        }
    }
};


VulkanFunctions::VulkanFunctions()
{

}


bool VulkanFunctions::load()
{
    Library library;

    if (!library.load())
    {
        qDebug() << "error load vulkan";
        return false;
    }

	library.resolve("vkGetInstanceProcAddr", &vkGetInstanceProcAddr);


    library.resolve("vkEnumerateInstanceLayerProperties", &vkEnumerateInstanceLayerProperties);
    library.resolve("vkEnumerateInstanceExtensionProperties", &vkEnumerateInstanceExtensionProperties);


    library.resolve("vkCreateInstance", &vkCreateInstance);

    library.resolve("vkEnumeratePhysicalDevices", &vkEnumeratePhysicalDevices);

#ifdef linux
    library.resolve("vkCreateXlibSurfaceKHR", &vkCreateXlibSurfaceKHR);
    library.resolve("vkCreateXcbSurfaceKHR", &vkCreateXcbSurfaceKHR);
#endif

#ifdef WIN32
    library.resolve("vkCreateWin32SurfaceKHR", &vkCreateWin32SurfaceKHR);
#endif

    library.resolve("vkGetPhysicalDeviceFormatProperties", &vkGetPhysicalDeviceFormatProperties);
    library.resolve("vkGetPhysicalDeviceMemoryProperties", &vkGetPhysicalDeviceMemoryProperties);
    library.resolve("vkGetPhysicalDeviceProperties", &vkGetPhysicalDeviceProperties);    
    library.resolve("vkGetPhysicalDeviceFeatures", &vkGetPhysicalDeviceFeatures);
    library.resolve("vkCreateDevice", &vkCreateDevice);
    library.resolve("vkEnumerateDeviceLayerProperties", &vkEnumerateDeviceLayerProperties);
    library.resolve("vkEnumerateDeviceExtensionProperties", &vkEnumerateDeviceExtensionProperties);
    library.resolve("vkGetPhysicalDeviceSurfaceFormatsKHR", &vkGetPhysicalDeviceSurfaceFormatsKHR);
    library.resolve("vkGetPhysicalDeviceSurfaceCapabilitiesKHR", &vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
    library.resolve("vkGetPhysicalDeviceSurfaceSupportKHR", &vkGetPhysicalDeviceSurfaceSupportKHR);
    library.resolve("vkGetPhysicalDeviceQueueFamilyProperties", &vkGetPhysicalDeviceQueueFamilyProperties);
	library.resolve("vkGetPhysicalDeviceSurfacePresentModesKHR", &vkGetPhysicalDeviceSurfacePresentModesKHR);

    library.resolve("vkCmdBeginRenderPass", &vkCmdBeginRenderPass);
    library.resolve("vkCmdEndRenderPass", &vkCmdEndRenderPass);
    library.resolve("vkCmdSetViewport", &vkCmdSetViewport);
    library.resolve("vkCmdSetScissor", &vkCmdSetScissor);
    library.resolve("vkCmdPipelineBarrier", &vkCmdPipelineBarrier);
    library.resolve("vkCmdBindPipeline", &vkCmdBindPipeline);
    library.resolve("vkCmdBindVertexBuffers", &vkCmdBindVertexBuffers);
    library.resolve("vkCmdBindIndexBuffer", &vkCmdBindIndexBuffer);
    library.resolve("vkCmdCopyImage", &vkCmdCopyImage);
    library.resolve("vkCmdPushConstants", &vkCmdPushConstants);
    library.resolve("vkCmdDrawIndexed", &vkCmdDrawIndexed);
    library.resolve("vkBeginCommandBuffer", &vkBeginCommandBuffer);
    library.resolve("vkEndCommandBuffer", &vkEndCommandBuffer);
	library.resolve("vkResetCommandBuffer", &vkResetCommandBuffer);

    library.resolve("vkQueueSubmit", &vkQueueSubmit);
    library.resolve("vkQueuePresentKHR", &vkQueuePresentKHR);


    library.resolve("vkFreeMemory", &vkFreeMemory);
    library.resolve("vkMapMemory", &vkMapMemory);
    library.resolve("vkUnmapMemory", &vkUnmapMemory);
    library.resolve("vkCreatePipelineCache", &vkCreatePipelineCache);
    library.resolve("vkDestroyBuffer", &vkDestroyBuffer);
    library.resolve("vkDestroyPipelineCache", &vkDestroyPipelineCache);
    library.resolve("vkCreateShaderModule", &vkCreateShaderModule);
    library.resolve("vkCreateSampler", &vkCreateSampler);
    library.resolve("vkCreateDescriptorPool", &vkCreateDescriptorPool);
    library.resolve("vkCreateDescriptorSetLayout", &vkCreateDescriptorSetLayout);
    library.resolve("vkAllocateDescriptorSets", &vkAllocateDescriptorSets);
    library.resolve("vkUpdateDescriptorSets", &vkUpdateDescriptorSets);
    library.resolve("vkCreatePipelineLayout", &vkCreatePipelineLayout);
    library.resolve("vkCreateGraphicsPipelines", &vkCreateGraphicsPipelines);
    library.resolve("vkCreateBuffer", &vkCreateBuffer);
    library.resolve("vkAllocateMemory", &vkAllocateMemory);
    library.resolve("vkBindBufferMemory", &vkBindBufferMemory);
    library.resolve("vkCreateImageView", &vkCreateImageView);
    library.resolve("vkCreateImage", &vkCreateImage);
    library.resolve("vkGetImageMemoryRequirements", &vkGetImageMemoryRequirements);
    library.resolve("vkBindImageMemory", &vkBindImageMemory);
    library.resolve("vkGetImageSubresourceLayout", &vkGetImageSubresourceLayout);
    library.resolve("vkDeviceWaitIdle", &vkDeviceWaitIdle);
    library.resolve("vkGetSwapchainImagesKHR", &vkGetSwapchainImagesKHR);
    library.resolve("vkCreateFence", &vkCreateFence);
    library.resolve("vkCreateSemaphore", &vkCreateSemaphore);
    library.resolve("vkDestroyFence", &vkDestroyFence);
    library.resolve("vkDestroySemaphore", &vkDestroySemaphore);
    library.resolve("vkDestroyFramebuffer", &vkDestroyFramebuffer);
    library.resolve("vkDestroyImageView", &vkDestroyImageView);
    library.resolve("vkFreeCommandBuffers", &vkFreeCommandBuffers);
    library.resolve("vkDestroyImage", &vkDestroyImage);
    library.resolve("vkDestroySwapchainKHR", &vkDestroySwapchainKHR);
    library.resolve("vkCreateRenderPass", &vkCreateRenderPass);
    library.resolve("vkWaitForFences", &vkWaitForFences);
    library.resolve("vkResetFences", &vkResetFences);
    library.resolve("vkAllocateCommandBuffers", &vkAllocateCommandBuffers);
    library.resolve("vkDestroyRenderPass", &vkDestroyRenderPass);
    library.resolve("vkDestroyCommandPool", &vkDestroyCommandPool);
    library.resolve("vkDestroyDevice", &vkDestroyDevice);
    library.resolve("vkCreateSwapchainKHR", &vkCreateSwapchainKHR);
    library.resolve("vkGetDeviceQueue", &vkGetDeviceQueue);
    library.resolve("vkCreateCommandPool", &vkCreateCommandPool);
    library.resolve("vkCreateFramebuffer", &vkCreateFramebuffer);
    library.resolve("vkAcquireNextImageKHR", &vkAcquireNextImageKHR);
    library.resolve("vkGetBufferMemoryRequirements", &vkGetBufferMemoryRequirements);
    library.resolve("vkCmdBindDescriptorSets", &vkCmdBindDescriptorSets);
    library.resolve("vkCmdDraw", &vkCmdDraw);
	library.resolve("vkCmdBlitImage", &vkCmdBlitImage);


    return true;
}
