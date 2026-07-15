#include "VulkanTextureBuffer.h"
#include "VulkanFunctions.h"
#include "VulkanWindow.h"
#include <QImage>


VulkanTextureBuffer::VulkanTextureBuffer()
{
    m_texFormat = VK_FORMAT_UNDEFINED;
    m_texImage = VK_NULL_HANDLE;
    m_texMem = VK_NULL_HANDLE;
    m_texLayoutPending = false;
    m_texStaging = VK_NULL_HANDLE;
    m_texStagingMem = VK_NULL_HANDLE;
    m_texView = VK_NULL_HANDLE;
    vulkanWindow = nullptr;
    m_devFuncs = nullptr;
    m_texStagingPending = false;
}


VkImage VulkanTextureBuffer::getTexStaging()
{
    return m_texStaging;
}


VkImage VulkanTextureBuffer::getTexImageg()
{
    return m_texImage;
}


QSize VulkanTextureBuffer::size()
{
    return m_texSize;
}


VkImageView VulkanTextureBuffer::texView()
{
    return m_texView;
}


bool VulkanTextureBuffer::createTexture(VulkanWindow *vulkanWindow,  const QImage &image)
{
    this->vulkanWindow = vulkanWindow;

    VkDevice dev = vulkanWindow->device();

    m_devFuncs = vulkanWindow->getFunctions();


    // Convert to byte ordered RGBA8. Use premultiplied alpha, see pColorBlendState in the pipeline.
    QImage img = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied);

    //const bool srgb = QCoreApplication::arguments().contains(QStringLiteral("--srgb"));
    //if (srgb)
    //qDebug("sRGB swapchain was requested, making texture sRGB too");

	bool srgb = false;

	m_texFormat = srgb ? VK_FORMAT_R8G8B8A8_SRGB : VK_FORMAT_R8G8B8A8_UNORM;


    // Now we can either map and copy the image data directly, or have to go
    // through a staging buffer to copy and convert into the internal optimal
    // tiling format.
    VkFormatProperties props;
    m_devFuncs->vkGetPhysicalDeviceFormatProperties(vulkanWindow->physicalDevice(), m_texFormat, &props);
    const bool canSampleLinear = (props.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
    const bool canSampleOptimal = (props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
    if (!canSampleLinear && !canSampleOptimal) {
        qWarning("Neither linear nor optimal image sampling is supported for RGBA8");
        return false;
    }

    static bool alwaysStage = qEnvironmentVariableIntValue("QT_VK_FORCE_STAGE_TEX");

    if (canSampleLinear && !alwaysStage) {
        if (!createTextureImage(img.size(), &m_texImage, &m_texMem,
                                VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_SAMPLED_BIT,
                                vulkanWindow->hostVisibleMemoryIndex()))
            return false;

        if (!writeLinearImage(img, m_texImage, m_texMem))
            return false;

        m_texLayoutPending = true;
    } else {
        if (!createTextureImage(img.size(), &m_texStaging, &m_texStagingMem,
                                VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                                vulkanWindow->hostVisibleMemoryIndex()))
            return false;

        if (!createTextureImage(img.size(), &m_texImage, &m_texMem,
                                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                vulkanWindow->deviceLocalMemoryIndex()))
            return false;

        if (!writeLinearImage(img, m_texStaging, m_texStagingMem))
            return false;

        m_texStagingPending = true;
    }

    VkImageViewCreateInfo viewInfo;
    memset(&viewInfo, 0, sizeof(viewInfo));
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_texImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = m_texFormat;
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.levelCount = viewInfo.subresourceRange.layerCount = 1;

    VkResult err = m_devFuncs->vkCreateImageView(dev, &viewInfo, nullptr, &m_texView);
    if (err != VK_SUCCESS) {
        qWarning("Failed to create image view for texture: %d", err);
        return false;
    }

    m_texSize = img.size();

    return true;
}


bool VulkanTextureBuffer::createTextureImage(const QSize &size, VkImage *image, VkDeviceMemory *mem, VkImageTiling tiling, VkImageUsageFlags usage, uint32_t memIndex)
{
    VkDevice dev = vulkanWindow->device();

    VkImageCreateInfo imageInfo;
    memset(&imageInfo, 0, sizeof(imageInfo));
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = m_texFormat;
    imageInfo.extent.width = size.width();
    imageInfo.extent.height = size.height();
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = tiling;
    imageInfo.usage = usage;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;


    VkResult err = m_devFuncs->vkCreateImage(dev, &imageInfo, nullptr, image);
    if (err != VK_SUCCESS) {
        qWarning("Failed to create linear image for texture: %d", err);
        return false;
    }

    VkMemoryRequirements memReq;
    m_devFuncs->vkGetImageMemoryRequirements(dev, *image, &memReq);

	if (!(memReq.memoryTypeBits & (1 << memIndex)))
	{
        VkPhysicalDeviceMemoryProperties physDevMemProps;

        m_devFuncs->vkGetPhysicalDeviceMemoryProperties(vulkanWindow->physicalDevice(), &physDevMemProps);

		for (uint32_t i = 0; i < physDevMemProps.memoryTypeCount; ++i)
		{
            if (!(memReq.memoryTypeBits & (1 << i)))
                continue;

            memIndex = i;
        }
    }

    VkMemoryAllocateInfo allocInfo = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        memReq.size,
        memIndex
    };
    //qDebug("allocating %u bytes for texture image", uint32_t(memReq.size));

    err = m_devFuncs->vkAllocateMemory(dev, &allocInfo, nullptr, mem);
    if (err != VK_SUCCESS) {
        qWarning("Failed to allocate memory for linear image: %d", err);
        return false;
    }

    err = m_devFuncs->vkBindImageMemory(dev, *image, *mem, 0);
    if (err != VK_SUCCESS) {
        qWarning("Failed to bind linear image memory: %d", err);
        return false;
    }

    return true;
}


bool VulkanTextureBuffer::writeLinearImage(const QImage &img, VkImage image, VkDeviceMemory memory)
{
    VkDevice dev = vulkanWindow->device();

    VkImageSubresource subres = {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0, // mip level
        0
    };
    VkSubresourceLayout layout;
    m_devFuncs->vkGetImageSubresourceLayout(dev, image, &subres, &layout);

    uchar *p;
    VkResult err = m_devFuncs->vkMapMemory(dev, memory, layout.offset, layout.size, 0, reinterpret_cast<void **>(&p));
    if (err != VK_SUCCESS) {
        qWarning("Failed to map memory for linear image: %d", err);
        return false;
    }

    for (int y = 0; y < img.height(); ++y) {
        const uchar *line = img.constScanLine(y);
        memcpy(p, line, img.width() * 4);
        p += layout.rowPitch;
    }

    m_devFuncs->vkUnmapMemory(dev, memory);

    return true;
}


void VulkanTextureBuffer::ensureTexture(VkCommandBuffer cb)
{
    if (!m_texLayoutPending && !m_texStagingPending)
        return;


    Q_ASSERT(m_texLayoutPending != m_texStagingPending);

#if 1
    VkImageMemoryBarrier barrier;
    memset(&barrier, 0, sizeof(barrier));
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.levelCount = barrier.subresourceRange.layerCount = 1;

    if (m_texLayoutPending) {
        m_texLayoutPending = false;

        barrier.oldLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.image = m_texImage;

        m_devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_HOST_BIT,
                                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                0, 0, nullptr, 0, nullptr,
                                1, &barrier);
    } else {
        m_texStagingPending = false;

        barrier.oldLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.image = m_texStaging;
        m_devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_HOST_BIT,
                                VK_PIPELINE_STAGE_TRANSFER_BIT,
                                0, 0, nullptr, 0, nullptr,
                                1, &barrier);

        barrier.oldLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.image = m_texImage;
        m_devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                VK_PIPELINE_STAGE_TRANSFER_BIT,
                                0, 0, nullptr, 0, nullptr,
                                1, &barrier);

        VkImageCopy copyInfo;
        memset(&copyInfo, 0, sizeof(copyInfo));
        copyInfo.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copyInfo.srcSubresource.layerCount = 1;
        copyInfo.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copyInfo.dstSubresource.layerCount = 1;
        copyInfo.extent.width = m_texSize.width();
        copyInfo.extent.height = m_texSize.height();
        copyInfo.extent.depth = 1;
        m_devFuncs->vkCmdCopyImage(cb, m_texStaging, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                          m_texImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyInfo);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.image = m_texImage;
        m_devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TRANSFER_BIT,
                                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                0, 0, nullptr, 0, nullptr,
                                1, &barrier);
    }

#endif


#if 0
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = m_texImage;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = m_texSize.width();
	int32_t mipHeight = m_texSize.height();


	uint32_t mipLevels = 100;

	for (uint32_t i = 1; i < mipLevels; ++i) {
		// Перевод предыдущего уровня (i-1) в TRANSFER_SRC_OPTIMAL
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL; // или текущий layout для mip0 после copy
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		m_devFuncs->vkCmdPipelineBarrier(
			cb,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		// Описываем blit from level i-1 -> level i
		VkImageBlit blit{};
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.srcOffsets[0] = {0, 0, 0};
		blit.srcOffsets[1] = {mipWidth, mipHeight, 1};

		int32_t dstWidth = std::max(1, mipWidth / 2);
		int32_t dstHeight = std::max(1, mipHeight / 2);

		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;
		blit.dstOffsets[0] = {0, 0, 0};
		blit.dstOffsets[1] = {dstWidth, dstHeight, 1};

		// Перед blit переводим dst уровень в TRANSFER_DST_OPTIMAL
		barrier.subresourceRange.baseMipLevel = i;
		barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; // если уровень не инициализирован
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		m_devFuncs->vkCmdPipelineBarrier(
			cb,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		// Выполняем blit
		m_devFuncs->vkCmdBlitImage(
			cb,
			m_texImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			m_texImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR
		);

		// После blit переводим previous level в SHADER_READ_ONLY_OPTIMAL
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		m_devFuncs->vkCmdPipelineBarrier(
			cb,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		// обновляем размеры для следующей итерации
		mipWidth = dstWidth;
		mipHeight = dstHeight;
	}

	// Переводим последний уровень (mipLevels-1) в SHADER_READ_ONLY_OPTIMAL
	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	m_devFuncs->vkCmdPipelineBarrier(
		cb,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);
#endif

}

