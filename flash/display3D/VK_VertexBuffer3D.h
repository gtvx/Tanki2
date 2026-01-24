#ifndef VK_VERTEXBUFFER3D_H
#define VK_VERTEXBUFFER3D_H

#include <QVector>
#include <vulkan/vulkan.h>

class VulkanFunctions;

class VK_VertexBuffer3D
{
	VulkanFunctions *m_devFuncs;
	VkDevice dev;
	uint32_t hostVisibleMemoryIndex;

	VkDeviceMemory memory;
	VkBuffer buffer;
	int numVertices;
	int data32PerVertex;

public:
	VK_VertexBuffer3D();

    ~VK_VertexBuffer3D();

	bool init(VulkanFunctions *m_devFuncs,
			  VkDevice dev,
			  uint32_t hostVisibleMemoryIndex,
			  int numVertices,
			  int data32PerVertex);

    VkBuffer get() { return buffer; }

    int getNumVertices() { return numVertices; }
    int getData32PerVertex() { return data32PerVertex; }

	bool uploadFromVector(QVector<float> &data, int startVertex, int numVertices);
    bool uploadFromVector(const float *data, int startVertex, int numVertices);
};

#endif // VK_VERTEXBUFFER3D_H
