#ifndef INDEXBUFFERRESOURCE_H
#define INDEXBUFFERRESOURCE_H

#include <QVector>
#include <memory>
#include "hardware/alternativa/gfx/core/GfxResource.h"

class VK_IndexBuffer3D;

class IndexBufferResource : public GfxResource
{
	QVector<uint32_t> _indices;
	int _numIndices;

public:

	std::shared_ptr<VK_IndexBuffer3D> vk_buffer;

	int numIndices() { return _numIndices; }
	QVector<uint32_t> indices() { return _indices; }

	IndexBufferResource(const QVector<uint> &list);

	bool vk_create(VulkanFunctions *m_devFuncs,
				   VkDevice dev,
				   uint32_t hostVisibleMemoryIndex);

	bool vk_upload();

	bool vk_isLoaded();
};

#endif // INDEXBUFFERRESOURCE_H
