#ifndef VERTEXBUFFERRESOURCE_H
#define VERTEXBUFFERRESOURCE_H

#include "GfxResource.h"
#include <memory>
#include <QVector>

class VK_VertexBuffer3D;

class VertexBufferResource : public GfxResource
{
	QVector<float> _vertices;
	int _numVertices;
	int _data32PerVertex;
	bool _upload;	

public:

	std::shared_ptr<VK_VertexBuffer3D> vk_buffer;

	int numVertices() { return _numVertices; }
	int data32PerVertex() { return _data32PerVertex; }
	QVector<float> get() { return _vertices; }

	void set(const QVector<float> &vertices) { _vertices = vertices; _upload = false; }

	VertexBufferResource(const QVector<float> &list, int data32PerVertex);
	VertexBufferResource(int count, int data32PerVertex);
	~VertexBufferResource();

	bool vk_create(VulkanFunctions *m_devFuncs,
				   VkDevice dev,
				   uint32_t hostVisibleMemoryIndex);

	bool vk_upload();
	bool vk_upload(const float *data, int numVertices);

	bool vk_isLoaded();
};

#endif // VERTEXBUFFERRESOURCE_H
