#include "VK_createInstance.h"
#include <vulkan/vulkan.h>
#include "VulkanFunctions.h"
#include <QVector>
#include <QDebug>


#ifdef linux
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>

#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#endif


VkInstance VK_createInstance(VulkanFunctions *functions)
{
#if 0
	uint32_t layerCount;
	functions->vkEnumerateInstanceLayerProperties(&layerCount, NULL);
	VkLayerProperties* layers = new VkLayerProperties[layerCount];
	functions->vkEnumerateInstanceLayerProperties(&layerCount, layers);

	qDebug() << "layerCount" << layerCount;

	for (uint32_t i = 0; i < layerCount; i++)
	{
		qDebug() << layers[i].layerName;
	}

	uint32_t extensionCount;
	functions->vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	VkExtensionProperties* extensions = new VkExtensionProperties[extensionCount];
	functions->vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);


	qDebug() << "extensionCount" << extensionCount;

	for (uint32_t i = 0; i < extensionCount; i++)
	{
		qDebug() << extensions[i].extensionName;
	}
#endif

	VkApplicationInfo appInfo = {};
	memset(&appInfo, 0, sizeof(appInfo));
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	memset(&createInfo, 0, sizeof(createInfo));
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	//createInfo.pApplicationInfo = &appInfo;

	QVector<const char*> enabledLayers;

	//enabledLayers << "VK_LAYER_MESA_overlay";
	//enabledLayers << "VK_LAYER_LUNARG_api_dump";
	//enabledLayers << "VK_LAYER_KHRONOS_validation";
	createInfo.ppEnabledLayerNames = enabledLayers.constData();
	createInfo.enabledLayerCount = enabledLayers.count();



	QVector<const char*> enabledExtensions;

#ifdef linux
	enabledExtensions << VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
	enabledExtensions << VK_KHR_XCB_SURFACE_EXTENSION_NAME;
#endif

#ifdef WIN32
	enabledExtensions << VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#endif

	enabledExtensions << VK_KHR_SURFACE_EXTENSION_NAME;


	createInfo.enabledExtensionCount = enabledExtensions.count();
	createInfo.ppEnabledExtensionNames = enabledExtensions.constData();

	VkResult err;


	VkInstance instance;

	err = functions->vkCreateInstance(&createInfo, nullptr, &instance);
	if (err != VK_SUCCESS)
	{
		qDebug() << "Failed to create Vulkan instance!" << err;
		exit(1);
	}

#if 0

	 uint32_t count = 10;

	 VkPhysicalDevice devs[count];
	 VkPhysicalDeviceProperties properties[10];

	 err = functions->vkEnumeratePhysicalDevices(instance, &count, devs);

	 if (err != VK_SUCCESS) {
		 qWarning("QVulkanWindow: Failed to enumerate physical devices: %d", err);
		 return VK_NULL_HANDLE;
	 }

	 for (uint32_t i = 0; i < count; ++i)
	 {
		 VkPhysicalDeviceProperties *p = &properties[i];
		 functions->vkGetPhysicalDeviceProperties(devs[i], p);
		 qDebug("Physical device [%d]: name '%s' version %d.%d.%d", i, p->deviceName,
				 VK_VERSION_MAJOR(p->driverVersion), VK_VERSION_MINOR(p->driverVersion),
				 VK_VERSION_PATCH(p->driverVersion));

		 qDebug() << "VkPhysicalDeviceLimits.maxPushConstantsSize" << p->limits.maxPushConstantsSize;

		 qDebug() << "VkPhysicalDeviceLimits.maxDescriptorSetUniformBuffers" << p->limits.maxDescriptorSetUniformBuffers;
	 }
#endif

	 return instance;
}

