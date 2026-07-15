#ifndef PAINTMATERIALVULKAN_H
#define PAINTMATERIALVULKAN_H

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
class BitmapData;
class VulkanUniform;
class DynamicChangeShader;
class DrawInitParams;


class PaintMaterialVulkan
{
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;

    VkShaderModule shaderModule_vs;
    VkShaderModule shaderModule_fs;

    VkSampler m_sampler_details;
    VkSampler m_sampler_spriteSheet;
    VkSampler m_sampler_lightMap;


    std::shared_ptr<BitmapData> detailsBitmap; //0
    std::shared_ptr<BitmapData> spriteSheetBitmap; //4
    std::shared_ptr<BitmapData> lightMapBitmap; //6


    VulkanTextureBuffer *textureBuffer_details; //0
    VulkanTextureBuffer *textureBuffer_spriteSheet; //4
    VulkanTextureBuffer *textureBuffer_lightMap; //6


    VkDescriptorPool m_descPool;
    VkDescriptorSetLayout m_descSetLayout;

    VkDescriptorSet m_descSet[MAX_CONCURRENT_FRAME_COUNT];

	DynamicChangeShader *dynamicChangeShader;


public:
    PaintMaterialVulkan(std::shared_ptr<BitmapData> spriteSheetBitmap,
                        std::shared_ptr<BitmapData> lightMapBitmap,
                        std::shared_ptr<BitmapData> detailsBitmap);


	~PaintMaterialVulkan();

	void init(DrawInitParams*);

	void reset();

    void drawOpaque(VulkanWindow *window,
                    Camera3D *camera,
                    VertexBufferResource *vertexBuffer,
                    IndexBufferResource *indexBuffer,
                    int firstIndex,
                    int numTriangles,
                    Object3D *object,
                    float uvTransformConst[8],
					float fragConst[8]);

    void setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource);
};

#endif // PAINTMATERIALVULKAN_H
