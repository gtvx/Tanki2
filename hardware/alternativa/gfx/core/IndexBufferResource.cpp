#include "IndexBufferResource.h"
#include "flash/display3D/VK_IndexBuffer3D.h"
#include "vulkan/VulkanFunctions.h"
#include <QDebug>


IndexBufferResource::IndexBufferResource(const QVector<uint> &list)
{
	this->_indices = list;
	this->_numIndices = this->_indices.length();
}


bool IndexBufferResource::vk_create(VulkanFunctions *m_devFuncs, VkDevice dev, uint32_t hostVisibleMemoryIndex)
{
	if (vk_buffer != nullptr)
		return true;


	vk_buffer = std::make_shared<VK_IndexBuffer3D>();


	vk_buffer->init(m_devFuncs, dev, hostVisibleMemoryIndex, _numIndices);

	return true;
}


bool IndexBufferResource::vk_upload()
{
	if (this->vk_buffer != nullptr)
	{

		if (this->vk_buffer->uploadFromVector(this->_indices, 0, this->_numIndices))
		{
			return true;
		}
		else
		{
			qDebug() << "error IndexBufferResource::upload() vk";
			return false;
		}
	}

	return false;
}


bool IndexBufferResource::vk_isLoaded()
{
	return vk_buffer != nullptr;
}
