#include "VK_VertexBuffer3D.h"
#include "vulkan/VulkanFunctions.h"


VK_VertexBuffer3D::VK_VertexBuffer3D()
{
	m_devFuncs = nullptr;
	dev = VK_NULL_HANDLE;
	hostVisibleMemoryIndex = 0;
	memory = VK_NULL_HANDLE;
	buffer = VK_NULL_HANDLE;
	numVertices = 0;
	data32PerVertex = 0;
}


VK_VertexBuffer3D::~VK_VertexBuffer3D()
{
	m_devFuncs->vkDestroyBuffer(dev, buffer, nullptr);
	m_devFuncs->vkFreeMemory(dev, memory, nullptr);
}


bool VK_VertexBuffer3D::init(VulkanFunctions *m_devFuncs,
							 VkDevice dev,
							 uint32_t hostVisibleMemoryIndex,
							 int numVertices,
							 int data32PerVertex)
{
	//GfxResource::create(context);

	this->m_devFuncs = m_devFuncs;
	this->dev = dev;
	this->data32PerVertex = data32PerVertex;
	this->numVertices = numVertices;

	VkDeviceSize SizeToLock = numVertices * data32PerVertex * sizeof(float);

	VkBufferCreateInfo bufInfo;
	memset(&bufInfo, 0, sizeof(bufInfo));
	bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufInfo.size = SizeToLock;
	bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;


	VkResult err = m_devFuncs->vkCreateBuffer(dev, &bufInfo, nullptr, &buffer);
	if (err != VK_SUCCESS)
	{
		qFatal("Failed to create instance buffer: %d", err);
		return false;
	}

	VkMemoryAllocateInfo memAllocInfo = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		nullptr,
		SizeToLock,
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


bool VK_VertexBuffer3D::uploadFromVector(QVector<float> &data, int startVertex, int numVertices)
{
    (void)startVertex;

    VkResult err;

    VkDeviceSize SizeToLock = numVertices *
			data32PerVertex *
            sizeof(float);

    float *_data;
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


	for (auto i = 0; i < data.size(); i++)
        _data[i] = data.at(i);

	m_devFuncs->vkUnmapMemory(dev, memory);

    return true;
}


bool VK_VertexBuffer3D::uploadFromVector(const float *data, int startVertex, int numVertices)
{
    (void)startVertex;

    VkResult err;

	VkDeviceSize SizeToLock = numVertices * data32PerVertex * sizeof(float);

    float *_data;
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

	int count = numVertices * data32PerVertex;

	for (int i = 0; i < count; i++)
	{
		_data[i] = data[i];
	}

	m_devFuncs->vkUnmapMemory(dev, memory);

    return true;
}
