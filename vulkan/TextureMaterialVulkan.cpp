#include "TextureMaterialVulkan.h"
#include "hardware/alternativa/gfx/core/VertexBufferResource.h"
#include "hardware/alternativa/gfx/core/IndexBufferResource.h"
#include "hardware/alternativa/engine3d/core/Camera3D.h"
#include "VulkanWindow.h"
#include "shader.h"
#include "flash/display/BitmapData.h"
#include "VulkanTextureBuffer.h"
#include "hardware/alternativa/gfx/core/BitmapTextureResource.h"
#include "VulkanFunctions.h"
#include <QImage>
#include "flash/display3D/VK_VertexBuffer3D.h"
#include "flash/display3D/VK_IndexBuffer3D.h"
#include "path.h"
#include <QDebug>



#define CONST_TRANSFORM_OFFSET 0
#define CONST_TRANSFORM_SIZE 48

#define CONST_PROJECTION_OFFSET 48
#define CONST_PROJECTION_SIZE 32

#define CONST_COLOR_OFFSET 112
#define CONST_COLOR_SIZE 32



static VkPushConstantRange pushConstantRanges[] = {
	{
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		.offset = CONST_TRANSFORM_OFFSET,
		.size = CONST_TRANSFORM_SIZE
	},
	{
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		.offset = CONST_PROJECTION_OFFSET,
		.size = CONST_PROJECTION_SIZE
	},
	{
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
		.offset = CONST_COLOR_OFFSET,
		.size = CONST_COLOR_SIZE
	}
};


TextureMaterialVulkan::TextureMaterialVulkan()
{
    pipeline = VK_NULL_HANDLE;
    pipelineLayout = VK_NULL_HANDLE;

    shaderModule_vs = VK_NULL_HANDLE;
    shaderModule_fs = VK_NULL_HANDLE;

    vulkanTextureBuffer = nullptr;
    m_sampler = VK_NULL_HANDLE;
    pipelineCache = VK_NULL_HANDLE;
}


void TextureMaterialVulkan::drawOpaque(VulkanWindow *window,
                                       Camera3D *camera,
                                       VertexBufferResource *vertexBuffer,
                                       IndexBufferResource *indexBuffer,
                                       int firstIndex,
                                       int numTriangles,
									   Object3D *object,
									   bool decal)
{
    VulkanFunctions *m_devFuncs = window->getFunctions();


    if (window != nullptr && window->isInit())
    {
        VkDevice dev = window->device();

        if (pipelineCache == VK_NULL_HANDLE)
        {
            VkPipelineCacheCreateInfo pipelineCacheInfo;
            memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));
            pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
            VkResult err = m_devFuncs->vkCreatePipelineCache(dev, &pipelineCacheInfo, nullptr, &pipelineCache);
            if (err != VK_SUCCESS)
                qFatal("Failed to create pipeline cache: %d", err);

        }


        if (pipeline == VK_NULL_HANDLE)
        {
            if (vulkanTextureBuffer == nullptr)
            {
				if (this->textureResource == nullptr)
				{
					qDebug() << this << "error TextureMaterialVulkan::drawOpaque this->textureResource";
					return;
				}
				else
				{

				}

				//std::shared_ptr<BitmapData> bitmapData = this->textureResource->bitmapData();
				BitmapData *bitmapData = this->textureResource->bitmapData();
                if (bitmapData != nullptr)
                {
                    QImage image = *bitmapData->qimage();

                    vulkanTextureBuffer = new VulkanTextureBuffer();
                    vulkanTextureBuffer->createTexture(window, image);
                }

            }

			shaderModule_vs = shader_create(m_devFuncs, dev, path::getShaders() + "TextureMaterialVertex.spv");
			shaderModule_fs = shader_create(m_devFuncs, dev, path::getShaders() + "TextureMaterialFragment.spv");

            VkResult err;


            {
                {
                    // Sampler.
                    VkSamplerCreateInfo samplerInfo;
                    memset(&samplerInfo, 0, sizeof(samplerInfo));
                    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;


					if (decal == false)
					{
						samplerInfo.magFilter = VK_FILTER_LINEAR;                    // плавная фильтрация при увеличении
						samplerInfo.minFilter = VK_FILTER_LINEAR;                    // плавная при уменьшении
						samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;      // линейная интерполяция мип-уровней

						samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;  // повтор текстуры по U
						samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;  // по V
						samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

						samplerInfo.mipLodBias = 0.0f;
						samplerInfo.anisotropyEnable = VK_TRUE;                     // включить анизотропную фильтрацию
						samplerInfo.maxAnisotropy = 8.0f;                           // 4-16 в зависимости от качества/производительности
						samplerInfo.compareEnable = VK_FALSE;
						samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
						samplerInfo.minLod = 0.0f;
						samplerInfo.maxLod = VK_LOD_CLAMP_NONE;                     // или (float)maxMipLevels-1
						samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK; // не используется для repeat
						samplerInfo.unnormalizedCoordinates = VK_FALSE;             // обычно нормализованные UV (0..1)

					}
					else
					{
						samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
						samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
						samplerInfo.anisotropyEnable = VK_FALSE;   // редко нужен для 2D overlay
						samplerInfo.maxAnisotropy = 1.0f;
						samplerInfo.minFilter = VK_FILTER_LINEAR;
						samplerInfo.magFilter = VK_FILTER_LINEAR;
						samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST; // если нет mipmaps, или VK_SAMPLER_MIPMAP_MODE_LINEAR если есть
						samplerInfo.minLod = 0.0f;
						samplerInfo.maxLod = 1.0f;                  // запретить выбор мипов если их нет
						samplerInfo.unnormalizedCoordinates = VK_FALSE; // обычно нормализованные coords

						samplerInfo.compareEnable = VK_TRUE;
						samplerInfo.compareOp = VK_COMPARE_OP_EQUAL;
					}

                    err = m_devFuncs->vkCreateSampler(dev, &samplerInfo, nullptr, &m_sampler);
                    if (err != VK_SUCCESS)
                        qFatal("Failed to create sampler: %d", err);
                }


                const int concurrentFrameCount = window->concurrentFrameCount();

                {
                    // Set up descriptor set and its layout.
                    VkDescriptorPoolSize descPoolSizes[2] = {
                        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, uint32_t(concurrentFrameCount) }
                    };
                    VkDescriptorPoolCreateInfo descPoolInfo;
                    memset(&descPoolInfo, 0, sizeof(descPoolInfo));
                    descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                    descPoolInfo.maxSets = concurrentFrameCount;
                    descPoolInfo.poolSizeCount = 1;
                    descPoolInfo.pPoolSizes = descPoolSizes;
                    err = m_devFuncs->vkCreateDescriptorPool(dev, &descPoolInfo, nullptr, &m_descPool);
                    if (err != VK_SUCCESS)
                        qFatal("Failed to create descriptor pool: %d", err);
                }

                {
                    VkDescriptorSetLayoutBinding layoutBinding[1] =
                    {
                        {
                            1, // binding
                            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                            1, // descriptorCount
                            VK_SHADER_STAGE_FRAGMENT_BIT,
                            nullptr
                        }
                    };

                    VkDescriptorSetLayoutCreateInfo descLayoutInfo = {
                        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                        nullptr,
                        0,
                        1, // bindingCount
                        layoutBinding
                    };
                    err = m_devFuncs->vkCreateDescriptorSetLayout(dev, &descLayoutInfo, nullptr, &m_descSetLayout);
                    if (err != VK_SUCCESS)
                        qFatal("Failed to create descriptor set layout: %d", err);
                }


                for (int i = 0; i < concurrentFrameCount; ++i)
                {
                    VkDescriptorSetAllocateInfo descSetAllocInfo = {
                        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                        nullptr,
                        m_descPool,
                        1,
                        &m_descSetLayout
                    };
                    err = m_devFuncs->vkAllocateDescriptorSets(dev, &descSetAllocInfo, &m_descSet[i]);
                    if (err != VK_SUCCESS)
                        qFatal("Failed to allocate descriptor 2 set: %d", err);

                    VkWriteDescriptorSet descWrite[2];
                    memset(descWrite, 0, sizeof(descWrite));

                    VkDescriptorImageInfo descImageInfo = {
                        m_sampler,
                        vulkanTextureBuffer->texView(),
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    };

                    descWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descWrite[0].dstSet = m_descSet[i];
                    descWrite[0].dstBinding = 1;
                    descWrite[0].descriptorCount = 1;
                    descWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    descWrite[0].pImageInfo = &descImageInfo;

                    m_devFuncs->vkUpdateDescriptorSets(dev, 1, descWrite, 0, nullptr);
                }


                {
                    // Graphics pipeline.
                    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
                    memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
                    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                    pipelineLayoutInfo.setLayoutCount = 1;
                    pipelineLayoutInfo.pSetLayouts = &m_descSetLayout;

					pipelineLayoutInfo.pushConstantRangeCount = sizeof(pushConstantRanges) / sizeof(pushConstantRanges[0]);
                    pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges;

                    err = m_devFuncs->vkCreatePipelineLayout(dev, &pipelineLayoutInfo, nullptr, &pipelineLayout);
                    if (err != VK_SUCCESS)
                        qFatal("Failed to create pipeline layout: %d", err);
                }

                {
                    //1
                    // Vertex layout.
                    VkVertexInputBindingDescription vertexBindingDesc[] = {
                        {
                            0, // binding
                            8 * sizeof(float),
                            VK_VERTEX_INPUT_RATE_VERTEX
                        }
                    };

                    //2
                    VkVertexInputAttributeDescription vertexAttrDesc[] = {
                        { // position
                          0, // location
                          0, // binding
                          VK_FORMAT_R32G32B32_SFLOAT,
                          0 // offset
						},
                        { // texcoord
                          1,
                          0,
                          VK_FORMAT_R32G32_SFLOAT,
                          3 * sizeof(float)
                        }
                    };

                    //3
                    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
                    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                    vertexInputInfo.pNext = nullptr;
                    vertexInputInfo.flags = 0;
                    vertexInputInfo.vertexBindingDescriptionCount = sizeof(vertexBindingDesc) / sizeof(vertexBindingDesc[0]);
                    vertexInputInfo.pVertexBindingDescriptions = vertexBindingDesc;
                    vertexInputInfo.vertexAttributeDescriptionCount = sizeof(vertexAttrDesc) / sizeof(vertexAttrDesc[0]);
                    vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;



                    //4
                    VkGraphicsPipelineCreateInfo pipelineInfo;
                    memset(&pipelineInfo, 0, sizeof(pipelineInfo));
                    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

                    //5
                    VkPipelineShaderStageCreateInfo shaderStages[2];
                    init_shader_stage_vertex(&shaderStages[0], shaderModule_vs);
                    init_shader_stage_fragment(&shaderStages[1], shaderModule_fs);
                    pipelineInfo.stageCount = 2;
                    pipelineInfo.pStages = shaderStages;
                    pipelineInfo.pVertexInputState = &vertexInputInfo;


                    //6
                    VkPipelineInputAssemblyStateCreateInfo ia;
                    memset(&ia, 0, sizeof(ia));
                    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                    pipelineInfo.pInputAssemblyState = &ia;

                    //7
                    VkPipelineViewportStateCreateInfo vp;
                    memset(&vp, 0, sizeof(vp));
                    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                    vp.viewportCount = 1;
                    vp.scissorCount = 1;
                    pipelineInfo.pViewportState = &vp;

                    //8
                    VkPipelineRasterizationStateCreateInfo rs;
                    memset(&rs, 0, sizeof(rs));
                    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;


					if (decal == false)
					{
						rs.pNext = NULL;
						rs.flags = 0;
						rs.depthClampEnable = VK_FALSE;
						rs.rasterizerDiscardEnable = VK_FALSE;
						rs.polygonMode = VK_POLYGON_MODE_FILL;
						rs.cullMode = VK_CULL_MODE_BACK_BIT;
						rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
						rs.depthBiasEnable = VK_FALSE;
						rs.depthBiasConstantFactor = 0.0f;
						rs.depthBiasClamp = 0.0f;
						rs.depthBiasSlopeFactor = 0.0f;
						rs.lineWidth = 1.0f;
					}

					if (decal == true)
					{
#if 1
						//Вариант A — HUD / 2D overlay (обычно)
						rs.cullMode = VK_CULL_MODE_NONE;          // двусторонняя плоскость
						rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
						rs.depthBiasEnable = VK_TRUE;            // глубина обрабатывается в пайплайне/рендер-пассах
						rs.polygonMode = VK_POLYGON_MODE_FILL;
						rs.lineWidth = 0.0f;
#endif

#if 0
						//Вариант B — Decal / проецируемая картинка на стену (чтобы избежать мерцания)
						rs.cullMode = VK_CULL_MODE_BACK_BIT;        // или NONE, если нужно рисовать обе стороны
						rs.depthBiasEnable = VK_TRUE;               // небольшое смещение, чтобы избежать Z-fighting
						rs.depthBiasConstantFactor = 1.0f;          // начать с ~1.0, подбирать эмпирически
						rs.depthBiasSlopeFactor = 1.0f;             // комбинированные значения подбирать по сцене
						rs.depthBiasClamp = 0.0f;
#endif

						//rs.depthBiasConstantFactor = 1.0f;          // начать с ~1.0, подбирать эмпирически
						//rs.depthBiasSlopeFactor = .5f;             // комбинированные значения подбирать по сцене
						//rs.depthBiasClamp = 1.0f;

						//rs.depthBiasEnable = VK_TRUE;
						//rs.depthBiasConstantFactor = 0.005f; // постоянная смещающая составляющая (units)
						//rs.depthBiasClamp = 0.0f;          // максимум для глубинного смещения (0 = no clamp)
						//rs.depthBiasSlopeFactor = 0.005f;    // множитель по углу (slope)



						rs.polygonMode = VK_POLYGON_MODE_FILL;
						rs.cullMode = VK_CULL_MODE_NONE;         // часто отключают
						rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

						rs.depthBiasEnable = VK_TRUE;
						rs.depthBiasConstantFactor = -1.0f;
						rs.depthBiasSlopeFactor = -1.0f;
						rs.depthBiasClamp = 0.0f;

						rs.lineWidth = 1.0f;
					}

					//rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
					//rs.lineWidth = 1.0f;
                    pipelineInfo.pRasterizationState = &rs;

                    //9
                    VkPipelineMultisampleStateCreateInfo ms;
                    memset(&ms, 0, sizeof(ms));
                    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                    ms.rasterizationSamples = window->sampleCountFlagBits();
                    ms.sampleShadingEnable = VK_FALSE;
                    ms.alphaToCoverageEnable = VK_TRUE;
                    //ms.rasterizationSamples = VK_SAMPLE_COUNT_4_BIT;
                    pipelineInfo.pMultisampleState = &ms;



                    //10
                    VkPipelineDepthStencilStateCreateInfo ds;
                    memset(&ds, 0, sizeof(ds));
                    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

					if (decal == false)
					{
						ds.depthTestEnable = VK_TRUE;
						ds.depthWriteEnable = VK_TRUE;
						ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
					}
					else
					{
						//this.device.setDepthTest(false, Context3DCompareMode.LESS);
						//D3DCMP_LESS
						//VK_COMPARE_OP_LESS

						//ds.depthTestEnable = VK_TRUE;
						//ds.depthWriteEnable = VK_FALSE;
						//ds.depthCompareOp = VK_COMPARE_OP_LESS;


						ds.depthBoundsTestEnable = VK_TRUE;

						ds.minDepthBounds = 0.0f;                        // если depthBoundsTestEnable = VK_TRUE
						ds.maxDepthBounds = 1.0f;


						ds.depthTestEnable = VK_TRUE;
						ds.depthWriteEnable = VK_FALSE;
						ds.depthCompareOp = VK_COMPARE_OP_LESS; // <-- здесь указываете "ZFUNC"
						ds.depthBoundsTestEnable = VK_FALSE;
						ds.stencilTestEnable = VK_FALSE;
					}


                    pipelineInfo.pDepthStencilState = &ds;

                    //11
                    VkPipelineColorBlendStateCreateInfo cb;
                    memset(&cb, 0, sizeof(cb));
                    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                    VkPipelineColorBlendAttachmentState att;
                    memset(&att, 0, sizeof(att));
                    att.colorWriteMask = 0xF;
                    cb.attachmentCount = 1;

					if (decal == true)
					{
						att.blendEnable = VK_TRUE;
						att.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
						att.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
						att.colorBlendOp = VK_BLEND_OP_ADD;
						att.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
						att.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
						att.alphaBlendOp = VK_BLEND_OP_ADD;
						att.colorWriteMask =
							VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
							VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
					}

                    cb.pAttachments = &att;
                    pipelineInfo.pColorBlendState = &cb;

                    //12
                    VkDynamicState dynEnable[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
                    VkPipelineDynamicStateCreateInfo dyn;
                    memset(&dyn, 0, sizeof(dyn));
                    dyn.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                    dyn.dynamicStateCount = sizeof(dynEnable) / sizeof(VkDynamicState);
                    dyn.pDynamicStates = dynEnable;
                    pipelineInfo.pDynamicState = &dyn;

                    pipelineInfo.layout = pipelineLayout;
                    pipelineInfo.renderPass = window->defaultRenderPass();

                    //13
                    VkResult err = m_devFuncs->vkCreateGraphicsPipelines(dev,
                                                                         VK_NULL_HANDLE,
                                                                         1,
                                                                         &pipelineInfo,
                                                                         nullptr,
                                                                         &pipeline);

                    if (err != VK_SUCCESS)
                        qFatal("Failed to create graphics pipeline: %d", err);


                }

            }


        }

        if (vulkanTextureBuffer == nullptr)
        {
            qDebug() << "error vulkanTextureBuffer";
        }
    }


    {
        VkCommandBuffer cb = window->currentCommandBuffer();

        if (vulkanTextureBuffer != nullptr)
        {
            //vulkanTextureBuffer->ensureTexture(cb);
        }


        m_devFuncs->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, CONST_TRANSFORM_OFFSET, CONST_TRANSFORM_SIZE, object->transformConst);

		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, CONST_PROJECTION_OFFSET, CONST_PROJECTION_SIZE, camera->projection);

		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, CONST_COLOR_OFFSET, CONST_COLOR_SIZE, object->colorConst);

        m_devFuncs->vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                                            &m_descSet[window->currentFrame()], 0, nullptr);


        VkBuffer buffer = vertexBuffer->vk_buffer->get();

        VkDeviceSize vbOffset = 0;
        m_devFuncs->vkCmdBindVertexBuffers(cb, 0, 1, &buffer, &vbOffset);

		if (indexBuffer != nullptr)
		{
			m_devFuncs->vkCmdBindIndexBuffer(cb, indexBuffer->vk_buffer->get(), 0, VK_INDEX_TYPE_UINT16);
		}


		if (indexBuffer != nullptr)
		{
			m_devFuncs->vkCmdBindIndexBuffer(cb, indexBuffer->vk_buffer->get(), 0, VK_INDEX_TYPE_UINT16);

			m_devFuncs->vkCmdDrawIndexed(cb,
										 numTriangles * 3, //indexCount
										 1, //instanceCount
										 firstIndex, //firstIndex
										 0, //vertexOffset
										 0); //firstInstance
		}
		else
		{
			m_devFuncs->vkCmdDraw(cb,
								  numTriangles * 3, //vertexCount
								  1, //instanceCount
								  firstIndex, //firstIndex
								  0); //firstInstance
		}

    }
}


void TextureMaterialVulkan::setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource)
{
    this->textureResource = textureResource;
}
