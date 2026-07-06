#include "TrackMaterialVulkan.h"
#include "hardware/alternativa/gfx/core/VertexBufferResource.h"
#include "hardware/alternativa/gfx/core/IndexBufferResource.h"
#include "hardware/alternativa/engine3d/core/Camera3D.h"
#include "VulkanWindow.h"
#include "shader.h"
#include "flash/display/BitmapData.h"
#include "VulkanTextureBuffer.h"
#include "hardware/alternativa/gfx/core/BitmapTextureResource.h"
#include "flash/display3D/VK_VertexBuffer3D.h"
#include "flash/display3D/VK_IndexBuffer3D.h"
#include "path.h"
#include "VulkanFunctions.h"
#include "VulkanUniform.h"
#include <QVulkanDeviceFunctions>
#include <QImage>
#include <QDebug>


#define CONST_TRANSFORM_OFFSET 0 //c0 c1 c2
#define CONST_TRANSFORM_SIZE 48

#define CONST_UV_CORRECTION_OFFSET 48 //c4
#define CONST_UV_CORRECTION_SIZE 16

#define CONST_UV_TRANSFORM_OFFSET 64 //c14 c15
#define CONST_UV_TRANSFORM_SIZE 32

#define CONST_COLOR_OFFSET 96 //fc0
#define CONST_COLOR_SIZE 32


static VkPushConstantRange pushConstantRanges[] = {
	{
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		.offset = CONST_TRANSFORM_OFFSET,
		.size = CONST_TRANSFORM_SIZE
	},
	{
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		.offset = CONST_UV_TRANSFORM_OFFSET,
		.size = CONST_UV_TRANSFORM_SIZE
	},
	{
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
		.offset = CONST_COLOR_OFFSET,
		.size = CONST_COLOR_SIZE
	}
};



TrackMaterialVulkan::TrackMaterialVulkan()
{
    pipeline = VK_NULL_HANDLE;
    pipelineLayout = VK_NULL_HANDLE;

    shaderModule_vs = VK_NULL_HANDLE;
    shaderModule_fs = VK_NULL_HANDLE;

    vulkanTextureBuffer = nullptr;
    m_sampler = VK_NULL_HANDLE;
}


void TrackMaterialVulkan::drawOpaque(VulkanWindow *window,
                                     Camera3D *camera,
                                     VertexBufferResource *vertexBuffer,
                                     IndexBufferResource *indexBuffer,
                                     int firstIndex,
                                     int numTriangles,
                                     Object3D *object,
									 float uvTransformConst[8],
									 VulkanUniform *vulkanUniform)
{
	(void)camera;

    VulkanFunctions *m_devFuncs = window->getFunctions();


    if (window != nullptr && window->isInit())
    {
        VkPipelineCache pipelineCache = window->getPipelineCache();

        if (pipelineCache == VK_NULL_HANDLE)
        {
            return;
        }

        VkDevice dev = window->device();

        if (pipeline == VK_NULL_HANDLE)
        {
            if (vulkanTextureBuffer == nullptr)
            {
				//std::shared_ptr<BitmapData> bitmapData = this->textureResource->bitmapData();
				BitmapData *bitmapData = this->textureResource->bitmapData();
                if (bitmapData != nullptr)
                {
                    QImage image = *bitmapData->qimage();

                    vulkanTextureBuffer = new VulkanTextureBuffer();
                    vulkanTextureBuffer->createTexture(window, image);
                }

            }

			shaderModule_vs = shader_create(m_devFuncs, dev, path::getShaders() + "TrackMaterialVertex.spv");
			shaderModule_fs = shader_create(m_devFuncs, dev, path::getShaders() + "TextureMaterialFragment.spv");

            VkResult err;

            {
                {
                    // Sampler.
                    VkSamplerCreateInfo samplerInfo;
                    memset(&samplerInfo, 0, sizeof(samplerInfo));
                    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
                    samplerInfo.magFilter = VK_FILTER_LINEAR;
                    samplerInfo.minFilter = VK_FILTER_LINEAR;
                    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
                    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
                    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
                    samplerInfo.maxAnisotropy = 10.f;
                    samplerInfo.anisotropyEnable = VK_TRUE;
                    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
                    samplerInfo.unnormalizedCoordinates = VK_FALSE;
                    samplerInfo.compareEnable = VK_FALSE;
                    samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
                    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

                    err = m_devFuncs->vkCreateSampler(dev, &samplerInfo, nullptr, &m_sampler);
                    if (err != VK_SUCCESS)
                        qFatal("Failed to create sampler: %d", err);
                }



                const int concurrentFrameCount = window->concurrentFrameCount();

                //qDebug() << "concurrentFrameCount" << concurrentFrameCount;

                {
                    // Set up descriptor set and its layout.
                    VkDescriptorPoolSize descPoolSizes[1] = {
                        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, uint32_t(concurrentFrameCount) }
                    };
                    VkDescriptorPoolCreateInfo descPoolInfo;
                    memset(&descPoolInfo, 0, sizeof(descPoolInfo));
                    descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
					descPoolInfo.maxSets = 2; //concurrentFrameCount;
                    descPoolInfo.poolSizeCount = 1;
                    descPoolInfo.pPoolSizes = descPoolSizes;
                    err = m_devFuncs->vkCreateDescriptorPool(dev, &descPoolInfo, nullptr, &m_descPool);
                    if (err != VK_SUCCESS)
                        qFatal("Failed to create descriptor pool: %d", err);
                }

                {
					VkDescriptorSetLayoutBinding layoutBinding[] =
                    {
                        {
                            1, // binding
                            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                            1, // descriptorCount
                            VK_SHADER_STAGE_FRAGMENT_BIT,
                            nullptr
						},
						{
							vulkanUniform->binding(), // binding
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
						sizeof(layoutBinding) / sizeof(layoutBinding[0]), // bindingCount
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

                    VkWriteDescriptorSet descWrite[1];
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


					{
						VkDescriptorBufferInfo vertUni;
						vertUni.buffer = vulkanUniform->buffer();
						vertUni.offset = 0;
						vertUni.range = 1024;

						VkWriteDescriptorSet descWrite;
						memset(&descWrite, 0, sizeof(descWrite));
						descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						descWrite.dstSet = m_descSet[i];
						descWrite.dstBinding = 2;
						descWrite.descriptorCount = 1;
						descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
						descWrite.pBufferInfo = &vertUni;

						m_devFuncs->vkUpdateDescriptorSets(dev, 1, &descWrite, 0, nullptr);
					}
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


                    //3
                    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
                    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                    vertexInputInfo.pNext = nullptr;
                    vertexInputInfo.flags = 0;
                    vertexInputInfo.vertexBindingDescriptionCount = sizeof(vertexBindingDesc) / sizeof(vertexBindingDesc[0]);
                    vertexInputInfo.pVertexBindingDescriptions = vertexBindingDesc;
                    vertexInputInfo.vertexAttributeDescriptionCount = sizeof(vertexAttrDesc) / sizeof(vertexAttrDesc[0]);
                    vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;
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
                    rs.polygonMode = VK_POLYGON_MODE_FILL;
                    rs.cullMode = VK_CULL_MODE_BACK_BIT;
                    rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
                    rs.lineWidth = 10.0f;
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
                    ds.depthTestEnable = VK_TRUE;
                    ds.depthWriteEnable = VK_TRUE;
                    ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
                    pipelineInfo.pDepthStencilState = &ds;

                    //11
                    VkPipelineColorBlendStateCreateInfo cb;
                    memset(&cb, 0, sizeof(cb));
                    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                    VkPipelineColorBlendAttachmentState att;
                    memset(&att, 0, sizeof(att));
                    att.colorWriteMask = 0xF;
                    cb.attachmentCount = 1;
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
                                                                         pipelineCache,
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

        m_devFuncs->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, CONST_TRANSFORM_OFFSET, CONST_TRANSFORM_SIZE, object->transformConst);

		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, CONST_UV_TRANSFORM_OFFSET, CONST_UV_TRANSFORM_SIZE, uvTransformConst);

		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, CONST_COLOR_OFFSET, CONST_COLOR_SIZE, object->colorConst);


		{
			uint32_t pDynamicOffsets[1];
			pDynamicOffsets[0] = 0;
			m_devFuncs->vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_descSet[window->currentFrame()], 1, pDynamicOffsets);
		}

        VkBuffer buffer = vertexBuffer->vk_buffer->get();
		VkBuffer buffer2 = vulkanUniform->buffer();

        VkDeviceSize vbOffset = 0;
        m_devFuncs->vkCmdBindVertexBuffers(cb, 0, 1, &buffer, &vbOffset);
		m_devFuncs->vkCmdBindVertexBuffers(cb, 1, 1, &buffer2, &vbOffset);


        m_devFuncs->vkCmdBindIndexBuffer(cb, indexBuffer->vk_buffer->get(), 0, VK_INDEX_TYPE_UINT16);

        m_devFuncs->vkCmdDrawIndexed(cb,
                                     numTriangles * 3, //indexCount
                                     1, //instanceCount
                                     firstIndex, //firstIndex
                                     0, //vertexOffset
                                     0); //firstInstance

    }
}


void TrackMaterialVulkan::setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource)
{
    this->textureResource = textureResource;
}
