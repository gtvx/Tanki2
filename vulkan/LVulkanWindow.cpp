#include "LVulkanWindow.h"
#include "VulkanFunctions.h"
#include <QDebug>

#include <set>

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

static const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	//VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};

static const int MAX_FRAMES_IN_FLIGHT = 2;



LVulkanWindow::LVulkanWindow()
{
	m_funcs = nullptr;
	instance = VK_NULL_HANDLE;
	imageAvailableSemaphore = VK_NULL_HANDLE;
	renderFinishedSemaphore = VK_NULL_HANDLE;
	inFlightFence = VK_NULL_HANDLE;
	//commandBuffer = VK_NULL_HANDLE;
	renderPass = VK_NULL_HANDLE;
	graphicsPipeline = VK_NULL_HANDLE;
	graphicsQueue = VK_NULL_HANDLE;
	presentQueue = VK_NULL_HANDLE;
	commandPool = VK_NULL_HANDLE;
	surface = VK_NULL_HANDLE;
	m_physicalDevice = VK_NULL_HANDLE;
	dev = VK_NULL_HANDLE;
	swapChain = VK_NULL_HANDLE;
	m_pipelineCache = VK_NULL_HANDLE;
	hostVisibleMemIndex = 0;

	depthImage = VK_NULL_HANDLE;
	depthImageMemory = VK_NULL_HANDLE;
	depthImageView = VK_NULL_HANDLE;

	last_width = 0;
	last_height = 0;

	m_renderEnable = false;
}


void LVulkanWindow::setVulkanInstance(VkInstance instance)
{
	this->instance = instance;
}


void LVulkanWindow::init()
{
	this->sampleCount = VK_SAMPLE_COUNT_4_BIT;

	this->msaaSamples = sampleCount;


	if (dev != VK_NULL_HANDLE)
		return;



	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();

	//setupDebugMessenger();

	hostVisibleMemIndex = 0;
	VkPhysicalDeviceMemoryProperties physDevMemProps;
	bool hostVisibleMemIndexSet = false;

	m_funcs->vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &physDevMemProps);

	for (uint32_t i = 0; i < physDevMemProps.memoryTypeCount; ++i) {
		const VkMemoryType *memType = physDevMemProps.memoryTypes;
		//qDebug("memtype %d: flags=0x%x", i, memType[i].propertyFlags);
		// Find a host visible, host coherent memtype. If there is one that is
		// cached as well (in addition to being coherent), prefer that.
		const int hostVisibleAndCoherent = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		if ((memType[i].propertyFlags & hostVisibleAndCoherent) == hostVisibleAndCoherent) {
			if (!hostVisibleMemIndexSet || (memType[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)) {
				hostVisibleMemIndexSet = true;
				hostVisibleMemIndex = i;
			}
		}
	}


	createSwapChain();

	createImageViews();


	createRenderPass();
	//createGraphicsPipeline();

	createCommandPool();
	createColorResources();
	createDepthResources();
	createFramebuffers();
	createCommandBuffer();
	createSyncObjects();
	initResources();

	initVulkan(this);
}


void LVulkanWindow::recreateSwapChain()
{
	m_funcs->vkDeviceWaitIdle(dev);

	cleanupSwapChain();

	createSwapChain();
	createImageViews();
	createColorResources();
	createDepthResources();
	createFramebuffers();
}


void LVulkanWindow::cleanupSwapChain()
{
	m_funcs->vkDestroyImageView(dev, depthImageView, nullptr);
	m_funcs->vkDestroyImage(dev, depthImage, nullptr);
	m_funcs->vkFreeMemory(dev, depthImageMemory, nullptr);

	m_funcs->vkDestroyImageView(dev, colorImageView, nullptr);
	m_funcs->vkDestroyImage(dev, colorImage, nullptr);
	m_funcs->vkFreeMemory(dev, colorImageMemory, nullptr);

	for (auto framebuffer : swapChainFramebuffers) {
		m_funcs->vkDestroyFramebuffer(dev, framebuffer, nullptr);
	}

	for (auto imageView : swapChainImageViews) {
		m_funcs->vkDestroyImageView(dev, imageView, nullptr);
	}

	m_funcs->vkDestroySwapchainKHR(dev, swapChain, nullptr);
}


void LVulkanWindow::createColorResources()
{
	VkFormat colorFormat = swapChainImageFormat;

	createImage(swapChainExtent.width,
				swapChainExtent.height,
				1,
				msaaSamples,
				colorFormat,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				colorImage,
				colorImageMemory);

	colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}


void LVulkanWindow::createDepthResources()
{
	VkFormat depthFormat = findDepthFormat();

	createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
}


void LVulkanWindow::createRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = msaaSamples;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = msaaSamples;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription colorAttachmentResolve{};
	colorAttachmentResolve.format = swapChainImageFormat;
	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentResolveRef{};
	colorAttachmentResolveRef.attachment = 2;
	colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments = &colorAttachmentResolveRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve };
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (m_funcs->vkCreateRenderPass(dev, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}


void LVulkanWindow::createSurface()
{

	WId id = this->winId();


#ifdef WIN32
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = 0;
	surfaceCreateInfo.hwnd = (HWND)id;

	if (m_funcs->vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
		qDebug() << "Failed to create Vulkan surface! vkCreateWin32SurfaceKHR";
		exit(1);
	}
#endif


#ifdef linux
#if 0
	Display *display = XOpenDisplay(nullptr);

	VkXlibSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.dpy = display;
	surfaceCreateInfo.window = id;

	if (m_funcs->vkCreateXlibSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
		qDebug() << "Failed to create Vulkan surface! vkCreateXlibSurfaceKHR";
		exit(1);
	}
#endif

	xcb_connection_t *connection = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(connection)) {
		qDebug() << "ERROR xcb_connect";
		return;
	}

	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.connection = connection;
	surfaceCreateInfo.window = id;

	if (m_funcs->vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
		qDebug() << "Failed to create Vulkan surface! vkCreateXlibSurfaceKHR";
		return;
	}

#endif
}


void LVulkanWindow::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	deviceFeatures.samplerAnisotropy = VK_TRUE;

	//if (enableValidationLayers) {
	//	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	//	createInfo.ppEnabledLayerNames = validationLayers.data();
	//} else {
		createInfo.enabledLayerCount = 0;
	//}

	if (m_funcs->vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &dev) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	m_funcs->vkGetDeviceQueue(dev, indices.graphicsFamily.value(), 0, &graphicsQueue);
	m_funcs->vkGetDeviceQueue(dev, indices.presentFamily.value(), 0, &presentQueue);
}


VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}


VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

#if 0
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}
#endif


void LVulkanWindow::createSwapChain()
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent;

	extent.width = this->width();
	extent.height = this->height();

	last_width = extent.width;
	last_height = extent.height;



	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (m_funcs->vkCreateSwapchainKHR(dev, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	m_funcs->vkGetSwapchainImagesKHR(dev, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	m_funcs->vkGetSwapchainImagesKHR(dev, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}


bool LVulkanWindow::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	m_funcs->vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	m_funcs->vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}


SwapChainSupportDetails LVulkanWindow::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	m_funcs->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	m_funcs->vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		m_funcs->vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	m_funcs->vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		m_funcs->vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}


bool LVulkanWindow::isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}


void LVulkanWindow::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	m_funcs->vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	m_funcs->vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			m_physicalDevice = device;

			VkPhysicalDeviceProperties p;
			m_funcs->vkGetPhysicalDeviceProperties(device, &p);

			qDebug("Physical device name '%s' version %d.%d.%d", p.deviceName,
					VK_VERSION_MAJOR(p.driverVersion), VK_VERSION_MINOR(p.driverVersion),
					VK_VERSION_PATCH(p.driverVersion));

			break;
		}
	}

	if (m_physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}


void LVulkanWindow::setFunctions(VulkanFunctions *functions)
{
	m_funcs = functions;
}


VulkanFunctions *LVulkanWindow::getFunctions()
{
	return m_funcs;
}


VkDevice LVulkanWindow::device()
{
	return dev;
}


bool LVulkanWindow::isInit()
{
	return dev != VK_NULL_HANDLE;
}


bool LVulkanWindow::isDebugEnabled()
{
	return false;
}


VkPipelineCache LVulkanWindow::getPipelineCache()
{
	return m_pipelineCache;
}


VkCommandBuffer LVulkanWindow::currentCommandBuffer()
{
	return commandBuffers[m_currentFrame];
}


VkRenderPass LVulkanWindow::defaultRenderPass()
{
	return renderPass;
}


VkPhysicalDevice LVulkanWindow::physicalDevice()
{
	return m_physicalDevice;
}


uint32_t LVulkanWindow::hostVisibleMemoryIndex()
{
	return hostVisibleMemIndex;
}


uint32_t LVulkanWindow::deviceLocalMemoryIndex()
{
	return hostVisibleMemIndex;
}


int LVulkanWindow::concurrentFrameCount()
{
	int frameLag = 1;
	return frameLag;
}


VkSampleCountFlagBits LVulkanWindow::sampleCountFlagBits()
{
	return sampleCount;
}


int LVulkanWindow::currentFrame()
{
	return 0;
}


void LVulkanWindow::renderEnable(bool enable)
{
	m_renderEnable = enable;
}


bool LVulkanWindow::isRenderEnable()
{
	return m_renderEnable;
}


QueueFamilyIndices LVulkanWindow::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	m_funcs->vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	m_funcs->vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		m_funcs->vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}


void LVulkanWindow::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (m_funcs->vkCreateCommandPool(dev, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}


void LVulkanWindow::createCommandBuffer()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

	if (m_funcs->vkAllocateCommandBuffers(dev, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}


void LVulkanWindow::createSyncObjects()
{
	uint32_t imageCount = static_cast<uint32_t>(swapChainImages.size());

	imageAvailableSemaphores.resize(imageCount);
	renderFinishedSemaphores.resize(imageCount);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (uint32_t i = 0; i < imageCount; ++i) {
		m_funcs->vkCreateSemaphore(dev, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
		m_funcs->vkCreateSemaphore(dev, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
	}

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		m_funcs->vkCreateFence(dev, &fenceInfo, nullptr, &inFlightFences[i]);
	}
}


void LVulkanWindow::createImageViews()
{
	swapChainImageViews.resize(swapChainImages.size());

	for (uint32_t i = 0; i < swapChainImages.size(); i++) {
		swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
}


VkFormat LVulkanWindow::findDepthFormat()
{
	std::vector<VkFormat> candidates = {
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT
	};
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		m_funcs->vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);
		if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
			return format;
	}
	throw std::runtime_error("No suitable depth format found!");
}


uint32_t LVulkanWindow::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	m_funcs->vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}


VkImageView LVulkanWindow::createImageView(VkImage image,
										   VkFormat format,
										   VkImageAspectFlags aspectFlags,
										   uint32_t mipLevels)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (m_funcs->vkCreateImageView(dev, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image view!");
	}

	return imageView;
}


void LVulkanWindow::createImage(uint32_t width,
								uint32_t height,
								uint32_t mipLevels,
								VkSampleCountFlagBits numSamples,
								VkFormat format,
								VkImageTiling tiling,
								VkImageUsageFlags usage,
								VkMemoryPropertyFlags properties,
								VkImage& image,
								VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = numSamples;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (m_funcs->vkCreateImage(dev, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	m_funcs->vkGetImageMemoryRequirements(dev, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (m_funcs->vkAllocateMemory(dev, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	m_funcs->vkBindImageMemory(dev, image, imageMemory, 0);
}


void LVulkanWindow::createFramebuffers()
{
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {

		std::array<VkImageView, 3> attachments = {
			colorImageView,
			depthImageView,
			swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());;
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (m_funcs->vkCreateFramebuffer(dev, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}


void LVulkanWindow::initResources()
{
	{
		VkPipelineCacheCreateInfo pipelineCacheInfo;
		memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));
		pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		VkResult err = m_funcs->vkCreatePipelineCache(dev, &pipelineCacheInfo, nullptr, &m_pipelineCache);
		if (err != VK_SUCCESS)
			qFatal("Failed to create pipeline cache: %d", err);

	}
}

#if 0
void LVulkanWindow::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (m_funcs->vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	m_funcs->vkCmdSetViewport(commandBuffer, 0, 1, &viewport);


	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;
	m_funcs->vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapChainExtent;


	VkClearColorValue clearColor = {{ 0.67f, 0.84f, 0.9f, 1.0f }};
	VkClearDepthStencilValue clearDS = { 1, 0 };
	VkClearValue clearValues[3];
	memset(clearValues, 0, sizeof(clearValues));
	clearValues[0].color = clearValues[2].color = clearColor;
	clearValues[1].depthStencil = clearDS;

	renderPassInfo.clearValueCount = 3;
	renderPassInfo.pClearValues = clearValues;


	m_funcs->vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	this->render(this);

	m_funcs->vkCmdEndRenderPass(commandBuffer);

	if (m_funcs->vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

}


void LVulkanWindow::render1()
{
	// Проверка изменения размера окна
	int w = this->width();
	int h = this->height();
	if (last_width != w || last_height != h) {
		last_width = w;
		last_height = h;
		recreateSwapChain();
		return;
	}

	uint32_t frameIndex = m_currentFrame;

	// Ждём завершения предыдущей работы на этом frame slot
	m_funcs->vkWaitForFences(dev, 1, &inFlightFences[frameIndex], VK_TRUE, UINT64_MAX);
	m_funcs->vkResetFences(dev, 1, &inFlightFences[frameIndex]);

	// Приобретаем следующее изображение swapchain
	uint32_t imageIndex;
	VkResult result = m_funcs->vkAcquireNextImageKHR(dev, swapChain, UINT64_MAX,
													 imageAvailableSemaphores[frameIndex],
													 VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		recreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS) {
		qDebug() << "vkAcquireNextImageKHR failed";
		return;
	}

	// Записываем команды рендера
	m_funcs->vkResetCommandBuffer(commandBuffers[frameIndex], 0);
	recordCommandBuffer(commandBuffers[frameIndex], imageIndex);

	// Submit to queue
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageAvailableSemaphores[frameIndex];
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[frameIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinishedSemaphores[imageIndex];

	if (m_funcs->vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[frameIndex]) != VK_SUCCESS) {
		qDebug() << "vkQueueSubmit failed";
		return;
	}

	// Present
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinishedSemaphores[imageIndex];
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain;
	presentInfo.pImageIndices = &imageIndex;

	result = m_funcs->vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		recreateSwapChain();
	} else if (result != VK_SUCCESS) {
		qDebug() << "vkQueuePresentKHR failed";
	}

	m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
#endif


bool LVulkanWindow::renderBegin1()
{
	// Проверка изменения размера окна
	int w = this->width();
	int h = this->height();
	if (last_width != w || last_height != h) {
		last_width = w;
		last_height = h;
		recreateSwapChain();
	}

	uint32_t frameIndex = m_currentFrame;

	// Ждём завершения предыдущей работы на этом frame slot
	m_funcs->vkWaitForFences(dev, 1, &inFlightFences[frameIndex], VK_TRUE, UINT64_MAX);
	m_funcs->vkResetFences(dev, 1, &inFlightFences[frameIndex]);

	// Приобретаем следующее изображение swapchain
	uint32_t imageIndex;
	VkResult result = m_funcs->vkAcquireNextImageKHR(dev, swapChain, UINT64_MAX,
													 imageAvailableSemaphores[frameIndex],
													 VK_NULL_HANDLE, &imageIndex);

	this->imageIndex = imageIndex;

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		recreateSwapChain();
		return false;
	}
	if (result != VK_SUCCESS) {
		qDebug() << "vkAcquireNextImageKHR failed";
		return false;
	}

	// Записываем команды рендера
	m_funcs->vkResetCommandBuffer(commandBuffers[frameIndex], 0);

	////////
	VkCommandBuffer commandBuffer = commandBuffers[frameIndex];

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (m_funcs->vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	return true;
}


bool LVulkanWindow::renderBegin2()
{
	uint32_t frameIndex = m_currentFrame;
	VkCommandBuffer commandBuffer = commandBuffers[frameIndex];

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	m_funcs->vkCmdSetViewport(commandBuffer, 0, 1, &viewport);


	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;
	m_funcs->vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapChainExtent;


	VkClearColorValue clearColor = {{ 0.67f, 0.84f, 0.9f, 1.0f }};
	VkClearDepthStencilValue clearDS = { 1, 0 };
	VkClearValue clearValues[3];
	memset(clearValues, 0, sizeof(clearValues));
	clearValues[0].color = clearValues[2].color = clearColor;
	clearValues[1].depthStencil = clearDS;

	renderPassInfo.clearValueCount = 3;
	renderPassInfo.pClearValues = clearValues;


	m_funcs->vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	return true;
}


void LVulkanWindow::renderEnd()
{
	uint32_t frameIndex = m_currentFrame;
	VkCommandBuffer commandBuffer = commandBuffers[frameIndex];

	m_funcs->vkCmdEndRenderPass(commandBuffer);

	if (m_funcs->vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

	////////

	// Submit to queue
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageAvailableSemaphores[frameIndex];
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[frameIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinishedSemaphores[imageIndex];

	if (m_funcs->vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[frameIndex]) != VK_SUCCESS) {
		qDebug() << "vkQueueSubmit failed";
		return;
	}

	// Present
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinishedSemaphores[imageIndex];
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain;
	presentInfo.pImageIndices = &imageIndex;

	VkResult result = m_funcs->vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		recreateSwapChain();
	} else if (result != VK_SUCCESS) {
		qDebug() << "vkQueuePresentKHR failed";
	}

	m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
