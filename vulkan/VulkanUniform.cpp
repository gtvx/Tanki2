#include "VulkanUniform.h"
#include "VulkanFunctions.h"
#include "VulkanWindow.h"

#include <QDebug>


VulkanUniform::VulkanUniform()
{
	vulkanWindow = nullptr;
	m_uniBuf = VK_NULL_HANDLE;
	m_bufMem = VK_NULL_HANDLE;
	//m_descSet = VK_NULL_HANDLE;
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
	bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;


	VkResult err;

	err = m_devFuncs->vkCreateBuffer(dev, &bufInfo, nullptr, &m_uniBuf);
	if (err != VK_SUCCESS)
		qFatal("Failed to create uniform buffer: %d", err);

	//VkMemoryRequirements uniMemReq;
	//fun->vkGetBufferMemoryRequirements(dev, m_uniBuf, &uniMemReq);





	{
		VkDescriptorSetLayoutBinding layoutBindings[] =
		{
			{
				binding, // binding
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
				1, // descriptorCount
				VK_SHADER_STAGE_VERTEX_BIT,
				nullptr
			}
		};

		VkDescriptorSetLayoutCreateInfo descLayoutInfo = {
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			nullptr,
			0,
			sizeof(layoutBindings) / sizeof(layoutBindings[0]), // bindingCount
			layoutBindings
		};

		err = m_devFuncs->vkCreateDescriptorSetLayout(dev, &descLayoutInfo, nullptr, &m_descSetLayout);
		if (err != VK_SUCCESS)
			qFatal("Failed to create descriptor set layout: %d", err);
	}
#if 0
	VkDescriptorPool m_descPool;

	{

		const int concurrentFrameCount = vulkanWindow->concurrentFrameCount();

		// Set up descriptor set and its layout.
		VkDescriptorPoolSize descPoolSizes[1] = {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, uint32_t(concurrentFrameCount) }
		};
		VkDescriptorPoolCreateInfo descPoolInfo;
		memset(&descPoolInfo, 0, sizeof(descPoolInfo));
		descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descPoolInfo.maxSets = 2;
		descPoolInfo.poolSizeCount = 1;
		descPoolInfo.pPoolSizes = descPoolSizes;
		err = m_devFuncs->vkCreateDescriptorPool(dev, &descPoolInfo, nullptr, &m_descPool);
		if (err != VK_SUCCESS)
			qFatal("Failed to create descriptor pool: %d", err);
	}
#endif
#if 0
	VkDescriptorSetAllocateInfo descSetAllocInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		nullptr,
		m_descPool,
		1,
		&m_descSetLayout
	};

	err = m_devFuncs->vkAllocateDescriptorSets(dev, &descSetAllocInfo, &m_descSet);
	if (err != VK_SUCCESS)
		qFatal("Failed to allocate descriptor 2 set: %d", err);
#endif

	VkMemoryAllocateInfo memAllocInfo = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		nullptr,
		1024,
		vulkanWindow->hostVisibleMemoryIndex()
	};
	err = m_devFuncs->vkAllocateMemory(dev, &memAllocInfo, nullptr, &m_bufMem);
	if (err != VK_SUCCESS)
		qFatal("Failed to allocate memory: %d", err);

	err = m_devFuncs->vkBindBufferMemory(dev, m_uniBuf, m_bufMem, 0);
	if (err != VK_SUCCESS)
		qFatal("Failed to bind vertex buffer memory: %d", err);


#if 0
	VkDescriptorBufferInfo vertUni;
	vertUni.buffer = m_uniBuf;
	vertUni.offset = 0;
	vertUni.range = 1024;

	VkWriteDescriptorSet descWrite;
	memset(&descWrite, 0, sizeof(descWrite));
	descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descWrite.dstSet = m_descSet;
	descWrite.dstBinding = binding;
	descWrite.descriptorCount = 1;
	descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	descWrite.pBufferInfo = &vertUni;

	m_devFuncs->vkUpdateDescriptorSets(dev, 1, &descWrite, 0, nullptr);
#endif

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
