#include "VertexBufferResource.h"
#include "vulkan/VulkanFunctions.h"
#include "flash/display3D/VK_VertexBuffer3D.h"
#include <QDebug>


VertexBufferResource::VertexBufferResource(const QVector<float> &vertices, int data32PerVertex)
{
	this->_vertices = vertices;
	this->_data32PerVertex = data32PerVertex;
	this->_numVertices = this->_vertices.length() / this->_data32PerVertex;
	this->_upload = false;
}


VertexBufferResource::VertexBufferResource(int count, int data32PerVertex)
{
	this->_data32PerVertex = data32PerVertex;
	this->_numVertices = count / this->_data32PerVertex;
	this->_upload = false;
}


VertexBufferResource::~VertexBufferResource()
{
	vk_buffer = nullptr;
}


bool VertexBufferResource::vk_create(VulkanFunctions *m_devFuncs,
									 VkDevice dev,
									 uint32_t hostVisibleMemoryIndex)
{
	if (vk_buffer != nullptr)
		return true;


	vk_buffer = std::make_shared<VK_VertexBuffer3D>();

	vk_buffer->init(m_devFuncs,
					dev,
					hostVisibleMemoryIndex,
					_numVertices,
					_data32PerVertex);

	return true;
}


bool VertexBufferResource::vk_upload()
{
	if (this->vk_buffer != nullptr)
	{
		//qDebug("VertexBufferResource::vk_upload");
		if (this->vk_buffer->uploadFromVector(this->_vertices, 0, this->_numVertices))
		{
			_upload = true;
			return true;
		}
		else
		{
			qDebug() << "error VertexBufferResource::upload() vk";
			return false;
		}
	}

	return false;
}


bool VertexBufferResource::vk_upload(const float *data, int numVertices)
{
	if (this->vk_buffer != nullptr)
	{
		//qDebug("VertexBufferResource::vk_upload");
		if (this->vk_buffer->uploadFromVector(data, 0, numVertices))
		{
			_upload = true;
			return true;
		}
		else
		{
			qDebug() << "error VertexBufferResource::upload() vk";
			return false;
		}
	}

	return false;
}


bool VertexBufferResource::vk_isLoaded()
{
	return this->vk_buffer != nullptr && _upload == true;
}
