#ifndef TRACKMATERIALVULKAN_H
#define TRACKMATERIALVULKAN_H

#include <vulkan/vulkan.h>
#include <memory>
#include <QSize>
#include "MaxConCurrentFrameCount.h"

class Camera3D;
class VertexBufferResource;
class IndexBufferResource;
class Object3D;
class VulkanVertexBuffer;
class VulkanIndexBuffer;
class VulkanTextureBuffer;
class BitmapTextureResource;
class VulkanWindow;


class TrackMaterialVulkan
{
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    VkSampler m_sampler;

    VkShaderModule shaderModule_vs;
    VkShaderModule shaderModule_fs;

    VulkanTextureBuffer *vulkanTextureBuffer;

    std::shared_ptr<BitmapTextureResource> textureResource;

    VkDescriptorPool m_descPool;
    VkDescriptorSetLayout m_descSetLayout;

    VkDescriptorSet m_descSet[MAX_CONCURRENT_FRAME_COUNT];

public:
    TrackMaterialVulkan();

    void drawOpaque(VulkanWindow *window,
                    Camera3D *camera,
                    VertexBufferResource *vertexBuffer,
                    IndexBufferResource *indexBuffer,
                    int firstIndex,
                    int numTriangles,
                    Object3D *object,
                    float uvTransformConst[8]);

    void setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource);

};

#endif // TRACKMATERIALVULKAN_H
