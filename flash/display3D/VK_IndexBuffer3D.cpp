#include "VK_IndexBuffer3D.h"
#include "vulkan/VulkanFunctions.h"


VK_IndexBuffer3D::VK_IndexBuffer3D()
{
	m_devFuncs = nullptr;
	dev = VK_NULL_HANDLE;
	memory = VK_NULL_HANDLE;
	buffer = VK_NULL_HANDLE;
}


bool VK_IndexBuffer3D::uploadFromVector(const QVector<uint32_t> &data, int startOffset, int count)
{
    (void)startOffset;

    VkDeviceSize SizeToLock = count * sizeof(uint16_t);

    VkResult err;

    uint16_t *_data;
    err = m_devFuncs->vkMapMemory(dev,
                                  memory,
                                  0,
                                  SizeToLock,
                                  0,
                                  reinterpret_cast<void **>(&_data));

    if (err != VK_SUCCESS)
    {
        qFatal("Failed to map memory 2: %d", err);
        return false;
    }


    for (int i = 0; i < data.size(); i++)
        _data[i] = data.at(i);

    m_devFuncs->vkUnmapMemory(dev, memory);

    return true;
}


bool VK_IndexBuffer3D::init(VulkanFunctions *m_devFuncs, VkDevice dev, uint32_t hostVisibleMemoryIndex, int numIndices)
{
	this->m_devFuncs = m_devFuncs;
	this->dev = dev;

	VkDeviceSize SizeToLock = numIndices * sizeof(uint16_t);

	VkBufferCreateInfo bufInfo;
	memset(&bufInfo, 0, sizeof(bufInfo));
	bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufInfo.size = SizeToLock;
	bufInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

	VkResult err;

	err = m_devFuncs->vkCreateBuffer(dev, &bufInfo, nullptr, &buffer);
	if (err != VK_SUCCESS)
	{
		qFatal("Failed to create instance buffer: %d", err);
		return false;
	}

	VkMemoryRequirements uniMemReq;
	m_devFuncs->vkGetBufferMemoryRequirements(dev, buffer, &uniMemReq);

	VkMemoryAllocateInfo memAllocInfo = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		nullptr,
		uniMemReq.size,
		hostVisibleMemoryIndex
	};

	err = m_devFuncs->vkAllocateMemory(dev, &memAllocInfo, nullptr, &memory);
	if (err != VK_SUCCESS)
	{
		qFatal("Failed to allocate memory: %d", err);
		return false;
	}

	err = m_devFuncs->vkBindBufferMemory(dev, buffer, memory, 0);
	if (err != VK_SUCCESS)
	{
		qFatal("Failed to bind instance buffer memory: %d", err);
		return false;
	}

	return true;
}


bool VK_IndexBuffer3D::uploadFromVector(const uint16_t *data, int startOffset, int count)
{
    (void)startOffset;

    VkDeviceSize SizeToLock = count * sizeof(uint16_t);

    VkResult err;

    uint16_t *_data;
    err = m_devFuncs->vkMapMemory(dev,
                                  memory,
                                  0,
                                  SizeToLock,
                                  0,
                                  reinterpret_cast<void **>(&_data));

    if (err != VK_SUCCESS)
    {
        qFatal("Failed to map memory 2: %d", err);
        return false;
    }

    memcpy(_data, data, SizeToLock);

    m_devFuncs->vkUnmapMemory(dev, memory);

    return true;
}
