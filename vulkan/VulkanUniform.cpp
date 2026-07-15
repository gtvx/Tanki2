#include "VulkanUniform.h"
#include "VulkanFunctions.h"
#include "VulkanWindow.h"
#include <QDebug>


VulkanUniform::VulkanUniform()
{
	vulkanWindow = nullptr;
	m_uniBuf = VK_NULL_HANDLE;
	m_bufMem = VK_NULL_HANDLE;
	m_binding = 0;
}


bool VulkanUniform::init(VulkanWindow *vulkanWindow, uint32_t binding)
{
	this->vulkanWindow = vulkanWindow;
	this->m_binding = binding;

	VulkanFunctions *m_devFuncs = vulkanWindow->getFunctions();

	VkDevice dev = vulkanWindow->device();

	if (dev == VK_NULL_HANDLE)
	{
		qFatal("dev VK_NULL_HANDLE");
	}


	VkBufferCreateInfo bufInfo;
	memset(&bufInfo, 0, sizeof(bufInfo));
	bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	const int blockMeshByteCount = 64 * 4 * sizeof(float);
	bufInfo.size = blockMeshByteCount;
	bufInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;


	VkResult err;

	err = m_devFuncs->vkCreateBuffer(dev, &bufInfo, nullptr, &m_uniBuf);
	if (err != VK_SUCCESS)
		qFatal("Failed to create uniform buffer: %d", err);

	VkMemoryRequirements uniMemReq;
	m_devFuncs->vkGetBufferMemoryRequirements(dev, m_uniBuf, &uniMemReq);


	VkMemoryAllocateInfo memAllocInfo = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		nullptr,
		uniMemReq.size,
		vulkanWindow->hostVisibleMemoryIndex()
	};

	err = m_devFuncs->vkAllocateMemory(dev, &memAllocInfo, nullptr, &m_bufMem);
	if (err != VK_SUCCESS)
		qFatal("Failed to allocate memory: %d", err);

	err = m_devFuncs->vkBindBufferMemory(dev, m_uniBuf, m_bufMem, 0);
	if (err != VK_SUCCESS)
		qFatal("Failed to bind vertex buffer memory: %d", err);

	return true;
}


bool VulkanUniform::upload(void *data, int offset, int size)
{
	VulkanFunctions *m_devFuncs = vulkanWindow->getFunctions();

	VkDevice dev = vulkanWindow->device();

	if (dev == VK_NULL_HANDLE)
	{
		qFatal("dev VK_NULL_HANDLE");
	}


	VkResult err;

	quint8 *p;
	err = m_devFuncs->vkMapMemory(dev, m_bufMem, offset, size, 0, reinterpret_cast<void **>(&p));
	if (err != VK_SUCCESS)
		qFatal("Failed to map memory: %d", err);

	memcpy(p, data, size);

	m_devFuncs->vkUnmapMemory(dev, m_bufMem);

	return true;
}
