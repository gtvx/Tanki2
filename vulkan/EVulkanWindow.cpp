#include "EVulkanWindow.h"
#include "VulkanFunctions.h"
#include <QPlatformSurfaceEvent>
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


static inline VkDeviceSize aligned(VkDeviceSize v, VkDeviceSize byteAlign)
{
    return (v + byteAlign - 1) & ~(byteAlign - 1);
}


EVulkanWindow::EVulkanWindow()
{
    this->dsFormat = VK_FORMAT_UNDEFINED;
    this->m_pipelineCache = VK_NULL_HANDLE;
    this->framePending = false;
    this->frameGrabbing = false;
    this->m_currentImage = 0;
    this->swapChain = VK_NULL_HANDLE;
    this->status = StatusFailRetry;
    this->m_defaultRenderPass = VK_NULL_HANDLE;
    this->cmdPool = VK_NULL_HANDLE;
    this->presCmdPool = VK_NULL_HANDLE;
    this->surface = VK_NULL_HANDLE;

    this->sampleCount = VK_SAMPLE_COUNT_4_BIT;

    this->swapChainBufferCount = 0;

    this->gfxQueueFamilyIdx = 0;

    this->presQueueFamilyIdx = 0;

    this->physDev = VK_NULL_HANDLE;
    this->dev = VK_NULL_HANDLE;

    this->hostVisibleMemIndex = 0;

    this->presQueue = VK_NULL_HANDLE;
    this->gfxQueue = VK_NULL_HANDLE;

	this->swapChain = VK_NULL_HANDLE;
    this->dsImage = VK_NULL_HANDLE;
    this->dsView = VK_NULL_HANDLE;
    this->dsMem = VK_NULL_HANDLE;
    this->msaaImageMem = VK_NULL_HANDLE;
    this->gfxQueue = VK_NULL_HANDLE;
    this->gfxQueue = VK_NULL_HANDLE;

    this->m_currentImage = 0;
    this->swapChainSupportsReadBack = 0;
    this->swapChainBufferCount = 0;
	this->m_maxPushConstantsSize = 0;
}


void EVulkanWindow::setVulkanInstance(VkInstance instance)
{
    this->instance = instance;
}


bool EVulkanWindow::isInit()
{
    return true;
}


bool EVulkanWindow::isDebugEnabled()
{
    return false;
}


void EVulkanWindow::setFunctions(VulkanFunctions *functions)
{
    this->m_devFuncs = functions;
}


VulkanFunctions *EVulkanWindow::getFunctions()
{
    return m_devFuncs;
}


VkPipelineCache EVulkanWindow::getPipelineCache()
{
    return m_pipelineCache;
}


VkDevice EVulkanWindow::device()
{
    return dev;
}


VkCommandBuffer EVulkanWindow::currentCommandBuffer()
{
    if (!framePending) {
        qWarning("QVulkanWindow: Attempted to call currentCommandBuffer() without an active frame");
        return VK_NULL_HANDLE;
    }
    return imageRes[m_currentImage].cmdBuf;
}


VkRenderPass EVulkanWindow::defaultRenderPass()
{
    return m_defaultRenderPass;
}


VkPhysicalDevice EVulkanWindow::physicalDevice()
{
    return physDev;
}


uint32_t EVulkanWindow::hostVisibleMemoryIndex()
{
    return hostVisibleMemIndex;
}


uint32_t EVulkanWindow::deviceLocalMemoryIndex()
{
    return hostVisibleMemIndex;
}


int EVulkanWindow::concurrentFrameCount()
{
    int frameLag = 1;
    return frameLag;
}


VkSampleCountFlagBits EVulkanWindow::sampleCountFlagBits()
{
    return sampleCount;
}


int EVulkanWindow::currentFrame()
{
    return 0;
}


static VkSurfaceKHR surfaceForWindow(VulkanFunctions *m_devFuncs, VkInstance instance, WId id)
{
    VkSurfaceKHR surface;

#ifdef WIN32
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance = 0;
    surfaceCreateInfo.hwnd = (HWND)id;

    if (m_devFuncs->vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
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

    if (m_devFuncs->vkCreateXlibSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
        qDebug() << "Failed to create Vulkan surface! vkCreateXlibSurfaceKHR";
        exit(1);
    }
#endif

    xcb_connection_t *connection = xcb_connect(NULL, NULL);
       if (xcb_connection_has_error(connection)) {
           qDebug() << "ERROR xcb_connect";
           return VK_NULL_HANDLE;
       }

    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.connection = connection;
    surfaceCreateInfo.window = id;

    if (m_devFuncs->vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
        qDebug() << "Failed to create Vulkan surface! vkCreateXlibSurfaceKHR";
        exit(1);
    }

#endif

    return surface;
}


static bool f_supportsPresent(VulkanFunctions *m_devFuncs, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t queueFamilyIndex)
{
    VkBool32 supported = false;
    m_devFuncs->vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, &supported);
    return supported;
}


static struct {
    VkSampleCountFlagBits mask;
    int count;
} q_vk_sampleCounts[] = {
    // keep this sorted by 'count'
    { VK_SAMPLE_COUNT_1_BIT, 1 },
    { VK_SAMPLE_COUNT_2_BIT, 2 },
    { VK_SAMPLE_COUNT_4_BIT, 4 },
    { VK_SAMPLE_COUNT_8_BIT, 8 },
    { VK_SAMPLE_COUNT_16_BIT, 16 },
    { VK_SAMPLE_COUNT_32_BIT, 32 },
    { VK_SAMPLE_COUNT_64_BIT, 64 }
};


bool EVulkanWindow::init()
{
	//qDebug("EVulkanWindow::init()");
    surface = surfaceForWindow(m_devFuncs, instance, this->winId());

    if (surface == VK_NULL_HANDLE) {
        qWarning("QVulkanWindow: Failed to retrieve Vulkan surface for window");
        status = StatusFailRetry;
        return false;
    }

    VkResult err;


    {
        int physDevIndex = -1;

        uint32_t count = 10;

        VkPhysicalDevice physDevs[10];
        VkPhysicalDeviceProperties physDevProps[10];

        err = m_devFuncs->vkEnumeratePhysicalDevices(instance, &count, physDevs);

		//qDebug() << err << count;

		(void)err;


        for (uint32_t i = 0; i < count; i++)
        {
            VkPhysicalDeviceProperties *p = &physDevProps[i];

            m_devFuncs->vkGetPhysicalDeviceProperties(physDevs[i], p);

            qDebug("Physical device [%d]: name '%s' version %d.%d.%d", i, p->deviceName,
                    VK_VERSION_MAJOR(p->driverVersion), VK_VERSION_MINOR(p->driverVersion),
                    VK_VERSION_PATCH(p->driverVersion));
        }

        if (count > 0)
        {
			physDevIndex = 0;
        }
        else
        {
            return false;
        }

		qDebug("Using physical device [%d]", physDevIndex);

        physDev = physDevs[physDevIndex];
        VkPhysicalDeviceProperties &physDevProp = physDevProps[physDevIndex];



        const VkPhysicalDeviceLimits *limits = &physDevProp.limits;
        VkSampleCountFlags color = limits->framebufferColorSampleCounts;
        VkSampleCountFlags depth = limits->framebufferDepthSampleCounts;
        VkSampleCountFlags stencil = limits->framebufferStencilSampleCounts;

		m_maxPushConstantsSize = limits->maxPushConstantsSize;

        QList<int> sampleCounts;

        for (const auto &qvk_sampleCount : q_vk_sampleCounts) {
            if ((color & qvk_sampleCount.mask)
                    && (depth & qvk_sampleCount.mask)
                    && (stencil & qvk_sampleCount.mask))
            {
                sampleCounts.append(qvk_sampleCount.count);
            }
        }

		//qDebug() << "Supported sample counts:" << sampleCounts;

        if (sampleCounts.contains(4)) {
			//qDebug("Requesting 4x MSAA");
            //m_window->setSampleCount(4);
        }
    }



    {
        uint32_t queueCount = 0;
        m_devFuncs->vkGetPhysicalDeviceQueueFamilyProperties(physDev, &queueCount, nullptr);
		QVector<VkQueueFamilyProperties> queueFamilyProps(queueCount);
        m_devFuncs->vkGetPhysicalDeviceQueueFamilyProperties(physDev, &queueCount, queueFamilyProps.data());
        gfxQueueFamilyIdx = uint32_t(-1);
        presQueueFamilyIdx = uint32_t(-1);

        for (int i = 0; i < queueFamilyProps.size(); ++i) {
            const bool supportsPresent = f_supportsPresent(m_devFuncs, physDev, surface, i);
            //qDebug("queue family %d: flags=0x%x count=%d supportsPresent=%d", i,
                    //queueFamilyProps[i].queueFlags, queueFamilyProps[i].queueCount, supportsPresent);
            if (gfxQueueFamilyIdx == uint32_t(-1)
                    && (queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                    && supportsPresent)
                gfxQueueFamilyIdx = i;
        }

        if (gfxQueueFamilyIdx != uint32_t(-1)) {
            presQueueFamilyIdx = gfxQueueFamilyIdx;
        } else {
            qDebug("No queue with graphics+present; trying separate queues");
            for (int i = 0; i < queueFamilyProps.size(); ++i) {
                if (gfxQueueFamilyIdx == uint32_t(-1) && (queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
                    gfxQueueFamilyIdx = i;
                if (presQueueFamilyIdx == uint32_t(-1) && f_supportsPresent(m_devFuncs, physDev, surface, i))
                    presQueueFamilyIdx = i;
            }
        }

        if (gfxQueueFamilyIdx == uint32_t(-1)) {
            qWarning("QVulkanWindow: No graphics queue family found");
            status = StatusFail;
            return false;
        }

        if (presQueueFamilyIdx == uint32_t(-1)) {
            qWarning("QVulkanWindow: No present queue family found");
            status = StatusFail;
            return false;
        }
    }




    {
		QVector<VkDeviceQueueCreateInfo> queueInfo;
        queueInfo.reserve(2);

        const float prio[] = { 0 };

        VkDeviceQueueCreateInfo addQueueInfo;
        memset(&addQueueInfo, 0, sizeof(addQueueInfo));
        addQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        addQueueInfo.queueFamilyIndex = gfxQueueFamilyIdx;
        addQueueInfo.queueCount = 1;
        addQueueInfo.pQueuePriorities = prio;
        queueInfo.append(addQueueInfo);

        if (gfxQueueFamilyIdx != presQueueFamilyIdx) {
            addQueueInfo.queueFamilyIndex = presQueueFamilyIdx;
            addQueueInfo.queueCount = 1;
            addQueueInfo.pQueuePriorities = prio;
            queueInfo.append(addQueueInfo);
        }


		QVector<const char *> devExts;

        devExts.append("VK_KHR_swapchain");


        VkDeviceCreateInfo devInfo;
        memset(&devInfo, 0, sizeof(devInfo));
        devInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        devInfo.queueCreateInfoCount = queueInfo.size();
        devInfo.pQueueCreateInfos = queueInfo.constData();
        devInfo.enabledExtensionCount = devExts.size();
        devInfo.ppEnabledExtensionNames = devExts.constData();

        VkPhysicalDeviceFeatures features = {};

        {
            // Enable all supported 1.0 core features, except ones that likely
            // involve a performance penalty.
            m_devFuncs->vkGetPhysicalDeviceFeatures(physDev, &features);
            features.robustBufferAccess = VK_FALSE;
            devInfo.pEnabledFeatures = &features;
        }


        // Device layers are not supported by QVulkanWindow since that's an already deprecated
        // API. However, have a workaround for systems with older API and layers (f.ex. L4T
        // 24.2 for the Jetson TX1 provides API 1.0.13 and crashes when the validation layer
        // is enabled for the instance but not the device).
#if 0
        uint32_t apiVersion = physDevProps[physDevIndex].apiVersion;
        if (VK_VERSION_MAJOR(apiVersion) == 1
            && VK_VERSION_MINOR(apiVersion) == 0
            && VK_VERSION_PATCH(apiVersion) <= 13)
        {
            // Make standard validation work at least.
            const QByteArray stdValName = QByteArrayLiteral("VK_LAYER_KHRONOS_validation");
            const char *stdValNamePtr = stdValName.constData();
            if (layers.contains(stdValName)) {
                uint32_t count = 0;
                VkResult err = m_devFuncs->vkEnumerateDeviceLayerProperties(physDev, &count, nullptr);
                if (err == VK_SUCCESS) {
                    QList<VkLayerProperties> layerProps(count);
                    err = m_devFuncs->vkEnumerateDeviceLayerProperties(physDev, &count, layerProps.data());
                    if (err == VK_SUCCESS) {
                        for (const VkLayerProperties &prop : layerProps) {
                            if (!strncmp(prop.layerName, stdValNamePtr, stdValName.size())) {
                                devInfo.enabledLayerCount = 1;
                                devInfo.ppEnabledLayerNames = &stdValNamePtr;
                                break;
                            }
                        }
                    }
                }
            }
        }
#endif

        VkResult err = m_devFuncs->vkCreateDevice(physDev, &devInfo, nullptr, &dev);
        if (err == VK_ERROR_DEVICE_LOST) {
            qWarning("QVulkanWindow: Physical device lost");
            status = StatusUninitialized;
            return false;
        }


        if (err != VK_SUCCESS) {
            qWarning("QVulkanWindow: Failed to create device: %d", err);
            status = StatusFail;
            return false;
        }

        m_devFuncs->vkGetDeviceQueue(dev, gfxQueueFamilyIdx, 0, &gfxQueue);

        if (gfxQueueFamilyIdx == presQueueFamilyIdx)
        {
            presQueue = gfxQueue;
        }
        else
        {
            m_devFuncs->vkGetDeviceQueue(dev, presQueueFamilyIdx, 0, &presQueue);
        }

        VkCommandPoolCreateInfo poolInfo;
        memset(&poolInfo, 0, sizeof(poolInfo));

        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = gfxQueueFamilyIdx;

        err = m_devFuncs->vkCreateCommandPool(dev, &poolInfo, nullptr, &cmdPool);

        if (err != VK_SUCCESS) {
            qWarning("QVulkanWindow: Failed to create command pool: %d", err);
            status = StatusFail;
            return false;
        }

        if (gfxQueueFamilyIdx != presQueueFamilyIdx) {
            poolInfo.queueFamilyIndex = presQueueFamilyIdx;
            err = m_devFuncs->vkCreateCommandPool(dev, &poolInfo, nullptr, &presCmdPool);
            if (err != VK_SUCCESS) {
                qWarning("QVulkanWindow: Failed to create command pool for present queue: %d", err);
                status = StatusFail;
                return false;
            }
        }

        hostVisibleMemIndex = 0;
        VkPhysicalDeviceMemoryProperties physDevMemProps;
        bool hostVisibleMemIndexSet = false;

        m_devFuncs->vkGetPhysicalDeviceMemoryProperties(physDev, &physDevMemProps);

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


        //qDebug("Picked memtype %d for host visible memory", hostVisibleMemIndex);
#if 0
        deviceLocalMemIndex = 0;

        for (uint32_t i = 0; i < physDevMemProps.memoryTypeCount; ++i) {
            const VkMemoryType *memType = physDevMemProps.memoryTypes;
            // Just pick the first device local memtype.
            if (memType[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
                deviceLocalMemIndex = i;
                break;
            }
        }

        //qDebug("Picked memtype %d for device local memory", deviceLocalMemIndex);
#endif

        // Figure out the color format here. Must not wait until recreateSwapChain()
        // because the renderpass should be available already from initResources (so
        // that apps do not have to defer pipeline creation to
        // initSwapChainResources), but the renderpass needs the final color format.

        uint32_t formatCount = 0;

        m_devFuncs->vkGetPhysicalDeviceSurfaceFormatsKHR(physDev, surface, &formatCount, nullptr);

		QVector<VkSurfaceFormatKHR> formats(formatCount);

        if (formatCount)
            m_devFuncs->vkGetPhysicalDeviceSurfaceFormatsKHR(physDev, surface, &formatCount, formats.data());

        colorFormat = VK_FORMAT_B8G8R8A8_UNORM; // our documented default if all else fails
        colorSpace = VkColorSpaceKHR(0); // this is in fact VK_COLOR_SPACE_SRGB_NONLINEAR_KHR

        // Pick the preferred format, if there is one.
        if (!formats.isEmpty() && formats[0].format != VK_FORMAT_UNDEFINED) {
            colorFormat = formats[0].format;
            colorSpace = formats[0].colorSpace;
        }

        /*
        // Try to honor the user request.
        if (!formats.isEmpty() && !requestedColorFormats.isEmpty()) {
            for (VkFormat reqFmt : std::as_const(requestedColorFormats)) {
                auto r = std::find_if(formats.cbegin(), formats.cend(),
                                      [reqFmt](const VkSurfaceFormatKHR &sfmt) { return sfmt.format == reqFmt; });
                if (r != formats.cend()) {
                    colorFormat = r->format;
                    colorSpace = r->colorSpace;
                    break;
                }
            }
        }
        */

        const VkFormat dsFormatCandidates[] = {
            VK_FORMAT_D24_UNORM_S8_UINT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D16_UNORM_S8_UINT
        };

        const int dsFormatCandidateCount = sizeof(dsFormatCandidates) / sizeof(VkFormat);
        int dsFormatIdx = 0;

        while (dsFormatIdx < dsFormatCandidateCount) {
            dsFormat = dsFormatCandidates[dsFormatIdx];
            VkFormatProperties fmtProp;
            m_devFuncs->vkGetPhysicalDeviceFormatProperties(physDev, dsFormat, &fmtProp);
            if (fmtProp.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
                break;
            ++dsFormatIdx;
        }

        if (dsFormatIdx == dsFormatCandidateCount)
            qWarning("QVulkanWindow: Failed to find an optimal depth-stencil format");

        //qDebug("Color format: %d Depth-stencil format: %d", colorFormat, dsFormat);

        if (!createDefaultRenderPass())
            return false;


        initResources();

        status = StatusDeviceReady;
    }

    return true;
}


void EVulkanWindow::render1()
{
	beginFrame();
}


bool EVulkanWindow::createDefaultRenderPass()
{
    VkAttachmentDescription attDesc[3];
    memset(attDesc, 0, sizeof(attDesc));
    const bool msaa = sampleCount > VK_SAMPLE_COUNT_1_BIT;
    // This is either the non-msaa render target or the resolve target.
    attDesc[0].format = colorFormat;
    attDesc[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attDesc[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // ignored when msaa
    attDesc[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attDesc[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attDesc[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attDesc[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attDesc[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    attDesc[1].format = dsFormat;
    attDesc[1].samples = sampleCount;
    attDesc[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attDesc[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attDesc[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attDesc[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attDesc[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attDesc[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    if (msaa) {
        // msaa render target
        attDesc[2].format = colorFormat;
        attDesc[2].samples = sampleCount;
        attDesc[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attDesc[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attDesc[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attDesc[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attDesc[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attDesc[2].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    VkAttachmentReference colorRef = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkAttachmentReference resolveRef = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkAttachmentReference dsRef = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
    VkSubpassDescription subPassDesc;
    memset(&subPassDesc, 0, sizeof(subPassDesc));
    subPassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPassDesc.colorAttachmentCount = 1;
    subPassDesc.pColorAttachments = &colorRef;
    subPassDesc.pDepthStencilAttachment = &dsRef;
    VkRenderPassCreateInfo rpInfo;
    memset(&rpInfo, 0, sizeof(rpInfo));
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.attachmentCount = 2;
    rpInfo.pAttachments = attDesc;
    rpInfo.subpassCount = 1;
    rpInfo.pSubpasses = &subPassDesc;
    if (msaa) {
        colorRef.attachment = 2;
        subPassDesc.pResolveAttachments = &resolveRef;
        rpInfo.attachmentCount = 3;
    }
    VkResult err = m_devFuncs->vkCreateRenderPass(dev, &rpInfo, nullptr, &m_defaultRenderPass);
    if (err != VK_SUCCESS) {
        qWarning("QVulkanWindow: Failed to create renderpass: %d", err);
        return false;
    }
    return true;
}


void EVulkanWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed()) {
        ensureStarted();
    } else {
#if 0
		if (!m_flags.testFlag(PersistentResources))
        {
            releaseSwapChain();
            reset();
        }
#endif
    }
}


bool EVulkanWindow::event(QEvent *e)
{
	switch (e->type())
	{
    case QEvent::UpdateRequest:
        //qDebug("UpdateRequest");
		//beginFrame();
        //requestUpdate();
        //qDebug("UpdateRequest end");
        break;
    // The swapchain must be destroyed before the surface as per spec. This is
    // not ideal for us because the surface is managed by the QPlatformWindow
    // which may be gone already when the unexpose comes, making the validation
    // layer scream. The solution is to listen to the PlatformSurface events.
    case QEvent::PlatformSurface:
		//qDebug("PlatformSurface");
#if 1
        if (static_cast<QPlatformSurfaceEvent *>(e)->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed)
        {
			//qDebug("PlatformSurface 2");
            releaseSwapChain();
            reset();
        }
#endif
        break;
    default:
        break;
    }

    return QWindow::event(e);
}


QSize EVulkanWindow::surfacePixelSize() const
{
    VkSurfaceCapabilitiesKHR surfaceCaps = {};
    m_devFuncs->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDev, surface, &surfaceCaps);
    VkExtent2D bufferSize = surfaceCaps.currentExtent;
    if (bufferSize.width == uint32_t(-1)) {
        Q_ASSERT(bufferSize.height == uint32_t(-1));
        return size() * devicePixelRatio();
    }
    return QSize(int(bufferSize.width), int(bufferSize.height));
}


void EVulkanWindow::beginFrame()
{
    if (!swapChain || framePending)
        return;

#if 1
    if (m_swapChainImageSize != surfacePixelSize()) {
        recreateSwapChain();
        if (!swapChain)
            return;
    }
#endif

    FrameResources &frame(frameRes);
    if (!frame.imageAcquired) {
        // Wait if we are too far ahead, i.e. the thread gets throttled based on the presentation rate
        // (note that we are using FIFO mode -> vsync)
        if (frame.fenceWaitable) {
            m_devFuncs->vkWaitForFences(dev, 1, &frame.fence, VK_TRUE, UINT64_MAX);
            m_devFuncs->vkResetFences(dev, 1, &frame.fence);
            frame.fenceWaitable = false;
        }

        VkResult err = VK_SUCCESS;

        //m_devFuncs->vkResetFences(dev, 1, &frame.fence);
        //err = m_devFuncs->vkWaitForFences(dev, 1, &frame.fence, VK_TRUE, 1000000000);


        // move on to next swapchain image
        err = m_devFuncs->vkAcquireNextImageKHR(dev,
                                                swapChain,
                                                UINT64_MAX,
                                                frame.imageSem,
                                                frame.fence,
                                                &m_currentImage);

        //m_currentImage = 0;
        if (err == VK_SUCCESS || err == VK_SUBOPTIMAL_KHR) {
            frame.imageSemWaitable = true;
            frame.imageAcquired = true;
            frame.fenceWaitable = true;
        } else if (err == VK_ERROR_OUT_OF_DATE_KHR) {

            qDebug() << "error vkAcquireNextImageKHR" << err;

            recreateSwapChain();
            requestUpdate();
            return;
        } else {

            qDebug() << "error vkAcquireNextImageKHR" << err;


            if (!checkDeviceLost(err, 1))
                qWarning("QVulkanWindow: Failed to acquire next swapchain image: %d", err);
            requestUpdate();
            return;
        }
    }


    // make sure the previous draw for the same image has finished
    ImageResources &image(imageRes[m_currentImage]);
    if (image.cmdFenceWaitable) {
        m_devFuncs->vkWaitForFences(dev, 1, &image.cmdFence, VK_TRUE, UINT64_MAX);
        m_devFuncs->vkResetFences(dev, 1, &image.cmdFence);
        image.cmdFenceWaitable = false;
    }


#if 0
    // build new draw command buffer
    if (image.cmdBuf) {
        m_devFuncs->vkFreeCommandBuffers(dev, cmdPool, 1, &image.cmdBuf);
        image.cmdBuf = nullptr;
    }
#endif

    VkResult err;

    VkCommandBufferAllocateInfo cmdBufInfo = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        nullptr,
        cmdPool,
        VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1
    };

    if (image.cmdBuf == NULL)
    {
        err = m_devFuncs->vkAllocateCommandBuffers(dev, &cmdBufInfo, &image.cmdBuf);

        if (err != VK_SUCCESS) {

            qDebug() << "error vkAllocateCommandBuffers" << err;

            if (!checkDeviceLost(err, 2))
                qWarning("QVulkanWindow: Failed to allocate frame command buffer: %d", err);
            return;
        }
    }

    VkCommandBufferBeginInfo cmdBufBeginInfo = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        nullptr,
        0,
        nullptr
    };

    err = m_devFuncs->vkBeginCommandBuffer(image.cmdBuf, &cmdBufBeginInfo);

    if (err != VK_SUCCESS) {

        qDebug() << "error vkBeginCommandBuffer" << err;

        if (!checkDeviceLost(err, 3))
            qWarning("QVulkanWindow: Failed to begin frame command buffer: %d", err);
        return;
    }

#if 0
    if (frameGrabbing)
    {
        frameGrabTargetImage = QImage(m_swapChainImageSize, QImage::Format_RGBA8888); // the format is as documented
    }
#endif

    if (1) {
        framePending = true;


        startNextFrame();


        endFrame();


        // done for now - endFrame() will get invoked when frameReady() is called back
    } else {

        qDebug("error renderer");

        VkClearColorValue clearColor = { { 0.0f, 0.0f, 0.0f, 1.0f } };
        VkClearDepthStencilValue clearDS = { 1.0f, 0 };
        VkClearValue clearValues[3];
        memset(clearValues, 0, sizeof(clearValues));
        clearValues[0].color = clearValues[2].color = clearColor;
        clearValues[1].depthStencil = clearDS;
        VkRenderPassBeginInfo rpBeginInfo;
        memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
        rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBeginInfo.renderPass = m_defaultRenderPass;
        rpBeginInfo.framebuffer = image.fb;
        rpBeginInfo.renderArea.extent.width = m_swapChainImageSize.width();
        rpBeginInfo.renderArea.extent.height = m_swapChainImageSize.height();
        rpBeginInfo.clearValueCount = sampleCount > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
        rpBeginInfo.pClearValues = clearValues;
        m_devFuncs->vkCmdBeginRenderPass(image.cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        m_devFuncs->vkCmdEndRenderPass(image.cmdBuf);
        endFrame();
    }

}


VkFramebuffer EVulkanWindow::currentFramebuffer()
{
    if (!framePending) {
        qWarning("QVulkanWindow: Attempted to call currentFramebuffer() without an active frame");
        return VK_NULL_HANDLE;
    }
    return imageRes[m_currentImage].fb;
}


void EVulkanWindow::startNextFrame()
{
    VkCommandBuffer cb = currentCommandBuffer();

    const QSize sz = m_swapChainImageSize;

    VkClearColorValue clearColor = {{ 0.67f, 0.84f, 0.9f, 1.0f }};
    VkClearDepthStencilValue clearDS = { 1, 0 };
    VkClearValue clearValues[3];
    memset(clearValues, 0, sizeof(clearValues));
    clearValues[0].color = clearValues[2].color = clearColor;
    clearValues[1].depthStencil = clearDS;

    VkRenderPassBeginInfo rpBeginInfo;
    memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
    rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBeginInfo.renderPass = defaultRenderPass();
    rpBeginInfo.framebuffer = currentFramebuffer();
    rpBeginInfo.renderArea.extent.width = sz.width();
    rpBeginInfo.renderArea.extent.height = sz.height();
    //rpBeginInfo.clearValueCount = m_window->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;

    rpBeginInfo.clearValueCount = 3;
    rpBeginInfo.pClearValues = clearValues;

    VkViewport viewport = {
        0, 0,
        float(sz.width()), float(sz.height()),
        0, 1
    };

    m_devFuncs->vkCmdSetViewport(cb, 0, 1, &viewport);

    VkRect2D scissor = {
        { 0, 0 },
        { uint32_t(sz.width()), uint32_t(sz.height()) }
    };

    m_devFuncs->vkCmdSetScissor(cb, 0, 1, &scissor);

    VkCommandBuffer cmdBuf = currentCommandBuffer();

    m_devFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	this->render(this);

    m_devFuncs->vkCmdEndRenderPass(cmdBuf);

    framePending = false;
    requestUpdate();

}


void EVulkanWindow::endFrame()
{
    FrameResources &frame(frameRes);
    ImageResources &image(imageRes[m_currentImage]);
    if (gfxQueueFamilyIdx != presQueueFamilyIdx && !frameGrabbing) {

        // Add the swapchain image release to the command buffer that will be
        // submitted to the graphics queue.
        VkImageMemoryBarrier presTrans;
        memset(&presTrans, 0, sizeof(presTrans));
        presTrans.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        presTrans.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        presTrans.oldLayout = presTrans.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        presTrans.srcQueueFamilyIndex = gfxQueueFamilyIdx;
        presTrans.dstQueueFamilyIndex = presQueueFamilyIdx;
        presTrans.image = image.image;
        presTrans.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        presTrans.subresourceRange.levelCount = presTrans.subresourceRange.layerCount = 1;

        m_devFuncs->vkCmdPipelineBarrier(image.cmdBuf,
                             VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                             VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                             0, 0, nullptr, 0, nullptr,
                             1, &presTrans);
    }


    // When grabbing a frame, add a readback at the end and skip presenting.
    //if (frameGrabbing)
        //addReadback();

    VkResult err = m_devFuncs->vkEndCommandBuffer(image.cmdBuf);
    if (err != VK_SUCCESS) {
        if (!checkDeviceLost(err, 4))
            qWarning("QVulkanWindow: Failed to end frame command buffer: %d", err);
        return;
    }


    // submit draw calls
    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(submitInfo));
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &image.cmdBuf;

    if (frame.imageSemWaitable) {
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &frame.imageSem;
    }



    if (!frameGrabbing) {
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &frame.drawSem;
    }

    VkPipelineStageFlags psf = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submitInfo.pWaitDstStageMask = &psf;
    Q_ASSERT(!image.cmdFenceWaitable);


    err = m_devFuncs->vkQueueSubmit(gfxQueue, 1, &submitInfo, image.cmdFence);


    if (err == VK_SUCCESS) {
        frame.imageSemWaitable = false;
        image.cmdFenceWaitable = true;
    } else {
        if (!checkDeviceLost(err, 5))
            qWarning("QVulkanWindow: Failed to submit to graphics queue: %d", err);
        return;
    }


#if 0
    // block and then bail out when grabbing
    if (frameGrabbing) {
        finishBlockingReadback();
        frameGrabbing = false;
        // Leave frame.imageAcquired set to true.
        // Do not change currentFrame.

        //emit q->frameGrabbed(frameGrabTargetImage);
        return;
    }
#endif

    if (gfxQueueFamilyIdx != presQueueFamilyIdx) {
        // Submit the swapchain image acquire to the present queue.
        submitInfo.pWaitSemaphores = &frame.drawSem;
        submitInfo.pSignalSemaphores = &frame.presTransSem;
        submitInfo.pCommandBuffers = &image.presTransCmdBuf; // must be USAGE_SIMULTANEOUS
        err = m_devFuncs->vkQueueSubmit(presQueue, 1, &submitInfo, VK_NULL_HANDLE);
        if (err != VK_SUCCESS) {
            if (!checkDeviceLost(err, 6))
                qWarning("QVulkanWindow: Failed to submit to present queue: %d", err);
            return;
        }
    }

    // queue present
    VkPresentInfoKHR presInfo;
    memset(&presInfo, 0, sizeof(presInfo));
    presInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presInfo.swapchainCount = 1;
    presInfo.pSwapchains = &swapChain;
    presInfo.pImageIndices = &m_currentImage;
    presInfo.waitSemaphoreCount = 1;
    presInfo.pWaitSemaphores = gfxQueueFamilyIdx == presQueueFamilyIdx ? &frame.drawSem : &frame.presTransSem;
    // Do platform-specific WM notification. F.ex. essential on Wayland in
    // order to circumvent driver frame callbacks
    //inst->presentAboutToBeQueued(q);

    err = m_devFuncs->vkQueuePresentKHR(presQueue, &presInfo);

    //qDebug() << "m_currentImage 2" << m_currentImage;


    if (err != VK_SUCCESS) {

        qDebug() << "error" << err;

        if (err == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            requestUpdate();
            return;
        } else if (err != VK_SUBOPTIMAL_KHR) {
            if (!checkDeviceLost(err, 7))
                qWarning("QVulkanWindow: Failed to present: %d", err);
            return;
        }
    }

    frame.imageAcquired = false;
    //inst->presentQueued(q);
}


void EVulkanWindow::recreateSwapChain()
{
    m_swapChainImageSize = size() * devicePixelRatio(); // note: may change below due to surfaceCaps

    if (m_swapChainImageSize.isEmpty()) // handle null window size gracefully
        return;

    m_devFuncs->vkDeviceWaitIdle(dev);

    VkPhysicalDevice physDev = this->physDev;
    VkSurfaceCapabilitiesKHR surfaceCaps;
    m_devFuncs->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDev, surface, &surfaceCaps);

    uint32_t reqBufferCount;
    if (surfaceCaps.maxImageCount == 0)
        reqBufferCount = qMax<uint32_t>(2, surfaceCaps.minImageCount);
    else
        reqBufferCount = qMax(qMin<uint32_t>(surfaceCaps.maxImageCount, 3), surfaceCaps.minImageCount);

    VkExtent2D bufferSize = surfaceCaps.currentExtent;
    if (bufferSize.width == uint32_t(-1)) {
        Q_ASSERT(bufferSize.height == uint32_t(-1));
        bufferSize.width = m_swapChainImageSize.width();
        bufferSize.height = m_swapChainImageSize.height();
    } else {
        m_swapChainImageSize = QSize(bufferSize.width, bufferSize.height);
    }

    VkSurfaceTransformFlagBitsKHR preTransform =
        (surfaceCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        ? VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR
        : surfaceCaps.currentTransform;

    VkCompositeAlphaFlagBitsKHR compositeAlpha =
        (surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)
        ? VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
        : VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;


    if (requestedFormat().hasAlpha()) {
        if (surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
            compositeAlpha = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
        else if (surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
            compositeAlpha = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
    }


    VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainSupportsReadBack = (surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
    if (swapChainSupportsReadBack)
        usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    VkSwapchainKHR oldSwapChain = swapChain;
    VkSwapchainCreateInfoKHR swapChainInfo;
    memset(&swapChainInfo, 0, sizeof(swapChainInfo));
    swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainInfo.surface = surface;
    swapChainInfo.minImageCount = reqBufferCount;
    swapChainInfo.imageFormat = colorFormat;
    swapChainInfo.imageColorSpace = colorSpace;
    swapChainInfo.imageExtent = bufferSize;
    swapChainInfo.imageArrayLayers = 1;
    swapChainInfo.imageUsage = usage;
    swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapChainInfo.preTransform = preTransform;
    swapChainInfo.compositeAlpha = compositeAlpha;
    swapChainInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
    swapChainInfo.clipped = true;
    swapChainInfo.oldSwapchain = oldSwapChain;

    //qDebug("Creating new swap chain of %d buffers, size %dx%d", reqBufferCount, bufferSize.width, bufferSize.height);

    VkSwapchainKHR newSwapChain;
    VkResult err = m_devFuncs->vkCreateSwapchainKHR(dev, &swapChainInfo, nullptr, &newSwapChain);
    if (err != VK_SUCCESS) {
        qWarning("QVulkanWindow: Failed to create swap chain: %d", err);
        return;
    }
    if (oldSwapChain)
        releaseSwapChain();

    swapChain = newSwapChain;
    uint32_t actualSwapChainBufferCount = 0;

    err = m_devFuncs->vkGetSwapchainImagesKHR(dev, swapChain, &actualSwapChainBufferCount, nullptr);
    if (err != VK_SUCCESS || actualSwapChainBufferCount < 2) {
        qWarning("QVulkanWindow: Failed to get swapchain images: %d (count=%d)", err, actualSwapChainBufferCount);
        return;
    }
    //qDebug("Actual swap chain buffer count: %d (supportsReadback=%d)",
	//			actualSwapChainBufferCount, swapChainSupportsReadBack);
    if (actualSwapChainBufferCount > MAX_SWAPCHAIN_BUFFER_COUNT) {
        qWarning("QVulkanWindow: Too many swapchain buffers (%d)", actualSwapChainBufferCount);
        return;
    }
    swapChainBufferCount = actualSwapChainBufferCount;
    VkImage swapChainImages[MAX_SWAPCHAIN_BUFFER_COUNT];
    err = m_devFuncs->vkGetSwapchainImagesKHR(dev, swapChain, &actualSwapChainBufferCount, swapChainImages);

    if (err != VK_SUCCESS) {
        qWarning("QVulkanWindow: Failed to get swapchain images: %d", err);
        return;
    }

    if (!createTransientImage(dsFormat,
                              VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                              VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
                              &dsImage,
                              &dsMem,
                              &dsView,
                              1))
    {
        return;
    }

    const bool msaa = sampleCount > VK_SAMPLE_COUNT_1_BIT;
    VkImage msaaImages[MAX_SWAPCHAIN_BUFFER_COUNT];
    VkImageView msaaViews[MAX_SWAPCHAIN_BUFFER_COUNT];

    if (msaa) {
        if (!createTransientImage(colorFormat,
                                  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                  VK_IMAGE_ASPECT_COLOR_BIT,
                                  msaaImages,
                                  &msaaImageMem,
                                  msaaViews,
                                  swapChainBufferCount))
        {
            return;
        }
    }

    VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, nullptr, VK_FENCE_CREATE_SIGNALED_BIT };

    for (uint32_t i = 0; i < swapChainBufferCount; ++i) {
        ImageResources &image(imageRes[i]);
        image.image = swapChainImages[i];
        if (msaa) {
            image.msaaImage = msaaImages[i];
            image.msaaImageView = msaaViews[i];
        }
        VkImageViewCreateInfo imgViewInfo;
        memset(&imgViewInfo, 0, sizeof(imgViewInfo));
        imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imgViewInfo.image = swapChainImages[i];
        imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imgViewInfo.format = colorFormat;
        imgViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        imgViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        imgViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
        imgViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imgViewInfo.subresourceRange.levelCount = imgViewInfo.subresourceRange.layerCount = 1;

        err = m_devFuncs->vkCreateImageView(dev, &imgViewInfo, nullptr, &image.imageView);
        if (err != VK_SUCCESS) {
            qWarning("QVulkanWindow: Failed to create swapchain image view %d: %d", i, err);
            return;
        }

        err = m_devFuncs->vkCreateFence(dev, &fenceInfo, nullptr, &image.cmdFence);
        if (err != VK_SUCCESS) {
            qWarning("QVulkanWindow: Failed to create command buffer fence: %d", err);
            return;
        }
        image.cmdFenceWaitable = true; // fence was created in signaled state
        VkImageView views[3] = { image.imageView,
                                 dsView,
                                 msaa ? image.msaaImageView : VK_NULL_HANDLE };
        VkFramebufferCreateInfo fbInfo;
        memset(&fbInfo, 0, sizeof(fbInfo));
        fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbInfo.renderPass = m_defaultRenderPass;
        fbInfo.attachmentCount = msaa ? 3 : 2;
        fbInfo.pAttachments = views;
        fbInfo.width = m_swapChainImageSize.width();
        fbInfo.height = m_swapChainImageSize.height();
        fbInfo.layers = 1;
        VkResult err = m_devFuncs->vkCreateFramebuffer(dev, &fbInfo, nullptr, &image.fb);
        if (err != VK_SUCCESS) {
            qWarning("QVulkanWindow: Failed to create framebuffer: %d", err);
            return;
        }
        if (gfxQueueFamilyIdx != presQueueFamilyIdx) {
            // pre-build the static image-acquire-on-present-queue command buffer
            VkCommandBufferAllocateInfo cmdBufInfo = {
                VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, nullptr, presCmdPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1 };
            err = m_devFuncs->vkAllocateCommandBuffers(dev, &cmdBufInfo, &image.presTransCmdBuf);
            if (err != VK_SUCCESS) {
                qWarning("QVulkanWindow: Failed to allocate acquire-on-present-queue command buffer: %d", err);
                return;
            }

            VkCommandBufferBeginInfo cmdBufBeginInfo = {
                VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, nullptr,
                VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, nullptr };

            err = m_devFuncs->vkBeginCommandBuffer(image.presTransCmdBuf, &cmdBufBeginInfo);
            if (err != VK_SUCCESS) {
                qWarning("QVulkanWindow: Failed to begin acquire-on-present-queue command buffer: %d", err);
                return;
            }
            VkImageMemoryBarrier presTrans;
            memset(&presTrans, 0, sizeof(presTrans));
            presTrans.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            presTrans.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            presTrans.oldLayout = presTrans.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            presTrans.srcQueueFamilyIndex = gfxQueueFamilyIdx;
            presTrans.dstQueueFamilyIndex = presQueueFamilyIdx;
            presTrans.image = image.image;
            presTrans.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            presTrans.subresourceRange.levelCount = presTrans.subresourceRange.layerCount = 1;
            m_devFuncs->vkCmdPipelineBarrier(image.presTransCmdBuf,
                                           VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                           VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                           0, 0, nullptr, 0, nullptr,
                                           1, &presTrans);
            err = m_devFuncs->vkEndCommandBuffer(image.presTransCmdBuf);
            if (err != VK_SUCCESS) {
                qWarning("QVulkanWindow: Failed to end acquire-on-present-queue command buffer: %d", err);
                return;
            }
        }
    }

    m_currentImage = 0;

    VkSemaphoreCreateInfo semInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, nullptr, 0 };

    {
        FrameResources &frame(frameRes);
        frame.imageAcquired = false;
        frame.imageSemWaitable = false;
        m_devFuncs->vkCreateFence(dev, &fenceInfo, nullptr, &frame.fence);
        frame.fenceWaitable = true; // fence was created in signaled state
        m_devFuncs->vkCreateSemaphore(dev, &semInfo, nullptr, &frame.imageSem);
        m_devFuncs->vkCreateSemaphore(dev, &semInfo, nullptr, &frame.drawSem);
        if (gfxQueueFamilyIdx != presQueueFamilyIdx)
            m_devFuncs->vkCreateSemaphore(dev, &semInfo, nullptr, &frame.presTransSem);
    }


    //if (renderer)
        //renderer->initSwapChainResources();

    status = StatusReady;
}


bool EVulkanWindow::createTransientImage(VkFormat format,
                                          VkImageUsageFlags usage,
                                          VkImageAspectFlags aspectMask,
                                          VkImage *images,
                                          VkDeviceMemory *mem,
                                          VkImageView *views,
                                          int count)
{
    VkMemoryRequirements memReq;
    VkResult err;
    Q_ASSERT(count > 0);

    for (int i = 0; i < count; ++i) {
        VkImageCreateInfo imgInfo;
        memset(&imgInfo, 0, sizeof(imgInfo));
        imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imgInfo.imageType = VK_IMAGE_TYPE_2D;
        imgInfo.format = format;
        imgInfo.extent.width = m_swapChainImageSize.width();
        imgInfo.extent.height = m_swapChainImageSize.height();
        imgInfo.extent.depth = 1;
        imgInfo.mipLevels = imgInfo.arrayLayers = 1;
        imgInfo.samples = sampleCount;
        imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imgInfo.usage = usage | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
        err = m_devFuncs->vkCreateImage(dev, &imgInfo, nullptr, images + i);
        if (err != VK_SUCCESS) {
            qWarning("QVulkanWindow: Failed to create image: %d", err);
            return false;
        }
        // Assume the reqs are the same since the images are same in every way.
        // Still, call GetImageMemReq for every image, in order to prevent the
        // validation layer from complaining.
        m_devFuncs->vkGetImageMemoryRequirements(dev, images[i], &memReq);
    }

    VkMemoryAllocateInfo memInfo;
    memset(&memInfo, 0, sizeof(memInfo));
    memInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memInfo.allocationSize = aligned(memReq.size, memReq.alignment) * count;
    uint32_t startIndex = 0;

    do {
        memInfo.memoryTypeIndex = chooseTransientImageMemType(images[0], startIndex);
        if (memInfo.memoryTypeIndex == uint32_t(-1)) {
            qWarning("QVulkanWindow: No suitable memory type found");
            return false;
        }
        startIndex = memInfo.memoryTypeIndex + 1;
        //qDebug("Allocating %u bytes for transient image (memtype %u)",
                //uint32_t(memInfo.allocationSize), memInfo.memoryTypeIndex);
        err = m_devFuncs->vkAllocateMemory(dev, &memInfo, nullptr, mem);
        if (err != VK_SUCCESS && err != VK_ERROR_OUT_OF_DEVICE_MEMORY) {
            qWarning("QVulkanWindow: Failed to allocate image memory: %d", err);
            return false;
        }
    } while (err != VK_SUCCESS);

    VkDeviceSize ofs = 0;

    for (int i = 0; i < count; ++i) {

        err = m_devFuncs->vkBindImageMemory(dev, images[i], *mem, ofs);

        if (err != VK_SUCCESS) {
            qWarning("QVulkanWindow: Failed to bind image memory: %d", err);
            return false;
        }

        ofs += aligned(memReq.size, memReq.alignment);

        VkImageViewCreateInfo imgViewInfo;
        memset(&imgViewInfo, 0, sizeof(imgViewInfo));
        imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imgViewInfo.image = images[i];
        imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imgViewInfo.format = format;
        imgViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        imgViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        imgViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
        imgViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        imgViewInfo.subresourceRange.aspectMask = aspectMask;
        imgViewInfo.subresourceRange.levelCount = imgViewInfo.subresourceRange.layerCount = 1;
        err = m_devFuncs->vkCreateImageView(dev, &imgViewInfo, nullptr, views + i);
        if (err != VK_SUCCESS) {
            qWarning("QVulkanWindow: Failed to create image view: %d", err);
            return false;
        }
    }
    return true;
}


void EVulkanWindow::releaseSwapChain()
{
    if (!dev || !swapChain) // do not rely on 'status', a half done init must be cleaned properly too
        return;

    m_devFuncs->vkDeviceWaitIdle(dev);

    //if (renderer)
    {
        //renderer->releaseSwapChainResources();
        m_devFuncs->vkDeviceWaitIdle(dev);
    }

    {
        FrameResources &frame(frameRes);
        if (frame.fence) {
            if (frame.fenceWaitable)
                m_devFuncs->vkWaitForFences(dev, 1, &frame.fence, VK_TRUE, UINT64_MAX);
            m_devFuncs->vkDestroyFence(dev, frame.fence, nullptr);
            frame.fence = VK_NULL_HANDLE;
            frame.fenceWaitable = false;
        }
        if (frame.imageSem) {
            m_devFuncs->vkDestroySemaphore(dev, frame.imageSem, nullptr);
            frame.imageSem = VK_NULL_HANDLE;
        }
        if (frame.drawSem) {
            m_devFuncs->vkDestroySemaphore(dev, frame.drawSem, nullptr);
            frame.drawSem = VK_NULL_HANDLE;
        }
        if (frame.presTransSem) {
            m_devFuncs->vkDestroySemaphore(dev, frame.presTransSem, nullptr);
            frame.presTransSem = VK_NULL_HANDLE;
        }
    }

    for (uint32_t i = 0; i < swapChainBufferCount; ++i) {
        ImageResources &image(imageRes[i]);
        if (image.cmdFence) {
            if (image.cmdFenceWaitable)
                m_devFuncs->vkWaitForFences(dev, 1, &image.cmdFence, VK_TRUE, UINT64_MAX);
            m_devFuncs->vkDestroyFence(dev, image.cmdFence, nullptr);
            image.cmdFence = VK_NULL_HANDLE;
            image.cmdFenceWaitable = false;
        }
        if (image.fb) {
            m_devFuncs->vkDestroyFramebuffer(dev, image.fb, nullptr);
            image.fb = VK_NULL_HANDLE;
        }
        if (image.imageView) {
            m_devFuncs->vkDestroyImageView(dev, image.imageView, nullptr);
            image.imageView = VK_NULL_HANDLE;
        }
        if (image.cmdBuf) {
            m_devFuncs->vkFreeCommandBuffers(dev, cmdPool, 1, &image.cmdBuf);
            image.cmdBuf = VK_NULL_HANDLE;
        }
        if (image.presTransCmdBuf) {
            m_devFuncs->vkFreeCommandBuffers(dev, presCmdPool, 1, &image.presTransCmdBuf);
            image.presTransCmdBuf = VK_NULL_HANDLE;
        }
        if (image.msaaImageView) {
            m_devFuncs->vkDestroyImageView(dev, image.msaaImageView, nullptr);
            image.msaaImageView = VK_NULL_HANDLE;
        }
        if (image.msaaImage) {
            m_devFuncs->vkDestroyImage(dev, image.msaaImage, nullptr);
            image.msaaImage = VK_NULL_HANDLE;
        }
    }
    if (msaaImageMem) {
        m_devFuncs->vkFreeMemory(dev, msaaImageMem, nullptr);
        msaaImageMem = VK_NULL_HANDLE;
    }
    if (dsView) {
        m_devFuncs->vkDestroyImageView(dev, dsView, nullptr);
        dsView = VK_NULL_HANDLE;
    }
    if (dsImage) {
        m_devFuncs->vkDestroyImage(dev, dsImage, nullptr);
        dsImage = VK_NULL_HANDLE;
    }
    if (dsMem) {
        m_devFuncs->vkFreeMemory(dev, dsMem, nullptr);
        dsMem = VK_NULL_HANDLE;
    }
    if (swapChain) {
        m_devFuncs->vkDestroySwapchainKHR(dev, swapChain, nullptr);
        swapChain = VK_NULL_HANDLE;
    }

    if (status == StatusReady)
        status = StatusDeviceReady;
}


uint32_t EVulkanWindow::chooseTransientImageMemType(VkImage img, uint32_t startIndex)
{
    VkPhysicalDeviceMemoryProperties physDevMemProps;

    m_devFuncs->vkGetPhysicalDeviceMemoryProperties(physDev, &physDevMemProps);

    VkMemoryRequirements memReq;
    m_devFuncs->vkGetImageMemoryRequirements(dev, img, &memReq);
    uint32_t memTypeIndex = uint32_t(-1);
    if (memReq.memoryTypeBits) {
        // Find a device local + lazily allocated, or at least device local memtype.
        const VkMemoryType *memType = physDevMemProps.memoryTypes;
        bool foundDevLocal = false;
        for (uint32_t i = startIndex; i < physDevMemProps.memoryTypeCount; ++i) {
            if (memReq.memoryTypeBits & (1 << i)) {
                if (memType[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
                    if (!foundDevLocal) {
                        foundDevLocal = true;
                        memTypeIndex = i;
                    }
                    if (memType[i].propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) {
                        memTypeIndex = i;
                        break;
                    }
                }
            }
        }
    }
    return memTypeIndex;
}


bool EVulkanWindow::checkDeviceLost(VkResult err, int id)
{
	(void)id;
    if (err == VK_ERROR_DEVICE_LOST) {
        qWarning("QVulkanWindow: Device lost");
        //if (renderer)
            //renderer->logicalDeviceLost();
        qDebug("Releasing all resources due to device lost");
        releaseSwapChain();
        reset();
        qDebug("Restarting");
        ensureStarted();
        return true;
    }
    return false;
}


void EVulkanWindow::ensureStarted()
{
    if (status == StatusFailRetry)
        status = StatusUninitialized;

    if (status == StatusUninitialized)
    {
        init();
        if (status == StatusDeviceReady)
        {
            recreateSwapChain();
        }
    }

    if (status == StatusReady)
    {
        requestUpdate();
    }
}


void EVulkanWindow::reset()
{
    if (!dev) // do not rely on 'status', a half done init must be cleaned properly too
        return;

    m_devFuncs->vkDeviceWaitIdle(dev);

    //if (renderer)
    {
        //renderer->releaseResources();
        m_devFuncs->vkDeviceWaitIdle(dev);
    }

    if (m_defaultRenderPass) {
        m_devFuncs->vkDestroyRenderPass(dev, m_defaultRenderPass, nullptr);
        m_defaultRenderPass = VK_NULL_HANDLE;
    }
    if (cmdPool) {
        m_devFuncs->vkDestroyCommandPool(dev, cmdPool, nullptr);
        cmdPool = VK_NULL_HANDLE;
    }
    if (presCmdPool) {
        m_devFuncs->vkDestroyCommandPool(dev, presCmdPool, nullptr);
        presCmdPool = VK_NULL_HANDLE;
    }

    /*
    if (frameGrabImage) {
        m_devFuncs->vkDestroyImage(dev, frameGrabImage, nullptr);
        frameGrabImage = VK_NULL_HANDLE;
    }
    if (frameGrabImageMem) {
        m_devFuncs->vkFreeMemory(dev, frameGrabImageMem, nullptr);
        frameGrabImageMem = VK_NULL_HANDLE;
    }
    */

    if (dev) {
        m_devFuncs->vkDestroyDevice(dev, nullptr);
        dev = VK_NULL_HANDLE;
        //vkCreateSwapchainKHR = nullptr; // re-resolve swapchain funcs later on since some come via the device
    }

    surface = VK_NULL_HANDLE;
    status = StatusUninitialized;
}


void EVulkanWindow::initResources()
{
    {
        VkPipelineCacheCreateInfo pipelineCacheInfo;
        memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));
        pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        VkResult err = m_devFuncs->vkCreatePipelineCache(dev, &pipelineCacheInfo, nullptr, &m_pipelineCache);
        if (err != VK_SUCCESS)
            qFatal("Failed to create pipeline cache: %d", err);

    }
}
