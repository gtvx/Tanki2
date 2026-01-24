#include "PaintMaterialVulkan.h"
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
#include "VulkanFunctions.h"
#include "path.h"
#include <QVulkanDeviceFunctions>
#include <QImage>
#include "MyMath.h"
#include <QDebug>

static const int details_binding = 1;
static const int spriteSheet_binding = 4;
static const int lightMap_binding = 6;


#define CONST_TRANSFORM_OFFSET 0
#define CONST_TRANSFORM_SIZE 48

#define CONST_PROJECTION_OFFSET 48
#define CONST_PROJECTION_SIZE 32

#define CONST_UV_CORRECTION_OFFSET 64
#define CONST_UV_CORRECTION_SIZE 16

#define CONST_UV_TRANSFORM_OFFSET 80
#define CONST_UV_TRANSFORM_SIZE 64

#define CONST_FRAG_OFFSET 112
#define CONST_FRAG_SIZE 64



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
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		.offset = CONST_UV_TRANSFORM_OFFSET,
		.size = CONST_UV_TRANSFORM_SIZE
	},
	{
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
		.offset = CONST_FRAG_OFFSET,
		.size = CONST_FRAG_SIZE
	}
};



#if 0
#include <QFileSystemWatcher>

class DynamicChangeShader : public QObject
{
	QFileSystemWatcher fileWatcher;

	PaintMaterialVulkan *w;

	void fileChanged(const QString &path)
	{
		(void)path;
		//qDebug() << "fileChanged" << path;
		w->reset();
	}



public:
	DynamicChangeShader(PaintMaterialVulkan *w)
	{
		this->w = w;

		fileWatcher.addPath(path::getShaders() + "PaintVertex.spv");
		fileWatcher.addPath(path::getShaders() + "AnimatedPaintFragment.spv");

		QObject::connect(&fileWatcher, &QFileSystemWatcher::fileChanged, this, &DynamicChangeShader::fileChanged);
	}
};
#endif



PaintMaterialVulkan::PaintMaterialVulkan(std::shared_ptr<BitmapData> spriteSheetBitmap,
                                         std::shared_ptr<BitmapData> lightMapBitmap,
                                         std::shared_ptr<BitmapData> detailsBitmap)
{
    this->pipeline = VK_NULL_HANDLE;
    this->pipelineLayout = VK_NULL_HANDLE;

    this->shaderModule_vs = VK_NULL_HANDLE;
    this->shaderModule_fs = VK_NULL_HANDLE;

    this->m_sampler_details = VK_NULL_HANDLE;
    this->m_sampler_spriteSheet = VK_NULL_HANDLE;
    this->m_sampler_lightMap = VK_NULL_HANDLE;

    this->spriteSheetBitmap = spriteSheetBitmap;
    this->lightMapBitmap = lightMapBitmap;
    this->detailsBitmap = detailsBitmap;

    this->textureBuffer_details = nullptr;
    this->textureBuffer_spriteSheet = nullptr;
    this->textureBuffer_lightMap = nullptr;

	//dynamicChangeShader = new DynamicChangeShader(this);
}


PaintMaterialVulkan::~PaintMaterialVulkan()
{
	//delete dynamicChangeShader;
}


void PaintMaterialVulkan::reset()
{
	pipeline = VK_NULL_HANDLE;
}


void PaintMaterialVulkan::drawOpaque(VulkanWindow *window,
                                     Camera3D *camera,
                                     VertexBufferResource *vertexBuffer,
                                     IndexBufferResource *indexBuffer,
                                     int firstIndex,
                                     int numTriangles,
                                     Object3D *object,
                                     float uvTransformConst[8],
									 float fragConst[8],
									 void *buffer,
									 int buffer_size)
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
            if (textureBuffer_details == nullptr)
            {
                if (detailsBitmap != nullptr)
                {
                    QImage image = *detailsBitmap->qimage();
                    textureBuffer_details = new VulkanTextureBuffer();
                    textureBuffer_details->createTexture(window, image);
                }
            }

            if (textureBuffer_spriteSheet == nullptr)
            {
                if (spriteSheetBitmap != nullptr)
                {
                    QImage image = *spriteSheetBitmap->qimage();
                    textureBuffer_spriteSheet = new VulkanTextureBuffer();
                    textureBuffer_spriteSheet->createTexture(window, image);
                }
            }

            if (textureBuffer_lightMap == nullptr)
            {
                if (lightMapBitmap != nullptr)
                {
                    QImage image = *lightMapBitmap->qimage();
                    textureBuffer_lightMap = new VulkanTextureBuffer();
                    textureBuffer_lightMap->createTexture(window, image);
                }
            }

			shaderModule_vs = shader_create(m_devFuncs, dev, path::getShaders() + "PaintVertex.spv");

			shaderModule_fs = shader_create(m_devFuncs, dev, path::getShaders() + "AnimatedPaintFragment.spv");

            VkResult err;


			VkMemoryRequirements uniMemReq;
            {
				uniform_buffer = VK_NULL_HANDLE;
				{
					VkBufferCreateInfo bufInfo;
					memset(&bufInfo, 0, sizeof(bufInfo));
					bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
					bufInfo.size = 8 * 10;
					bufInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

					VkResult err = m_devFuncs->vkCreateBuffer(dev, &bufInfo, nullptr, &uniform_buffer);
					if (err != VK_SUCCESS)
						qFatal("Failed to create vertex buffer: %d", err);

					m_devFuncs->vkGetBufferMemoryRequirements(dev, uniform_buffer, &uniMemReq);

				}

				{
					VkMemoryAllocateInfo memAllocInfo = {
						VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
						nullptr,
						uniMemReq.size,
						window->hostVisibleMemoryIndex()
					};



					err = m_devFuncs->vkAllocateMemory(dev, &memAllocInfo, nullptr, &m_bufMem);

					m_devFuncs->vkBindBufferMemory(dev, uniform_buffer, m_bufMem, 0);


					quint8 *p;
					VkResult err = m_devFuncs->vkMapMemory(dev, m_bufMem, 0, uniMemReq.size, 0,
														   reinterpret_cast<void **>(&p));
					if (err != VK_SUCCESS)
						qFatal("Failed to map memory: %d", err);
					memset(p, 0, uniMemReq.size);
					m_devFuncs->vkUnmapMemory(dev, m_bufMem);
				}




                {
                    // Sampler.
                    VkSamplerCreateInfo samplerInfo;
                    memset(&samplerInfo, 0, sizeof(samplerInfo));
                    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
					samplerInfo.magFilter = VK_FILTER_NEAREST;
					samplerInfo.minFilter = VK_FILTER_NEAREST;
					samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					samplerInfo.maxAnisotropy = 16.f;
					samplerInfo.anisotropyEnable = VK_TRUE;
					samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
					samplerInfo.unnormalizedCoordinates = VK_FALSE;

					samplerInfo.compareEnable = VK_FALSE;
					samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

					samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;


					err = m_devFuncs->vkCreateSampler(dev, &samplerInfo, nullptr, &m_sampler_details);
                    if (err != VK_SUCCESS)
                        qFatal("Failed to create sampler: %d", err);



                    err = m_devFuncs->vkCreateSampler(dev, &samplerInfo, nullptr, &m_sampler_lightMap);
                    if (err != VK_SUCCESS)
                        qFatal("Failed to create sampler: %d", err);


					samplerInfo.minLod = 90.0f;
					samplerInfo.maxLod = 100.0f;

					samplerInfo.maxAnisotropy = 10.f;
					samplerInfo.anisotropyEnable = VK_FALSE;
					err = m_devFuncs->vkCreateSampler(dev, &samplerInfo, nullptr, &m_sampler_spriteSheet);
					if (err != VK_SUCCESS)
						qFatal("Failed to create sampler: %d", err);
                }



                const int concurrentFrameCount = window->concurrentFrameCount();

                {
                    // Set up descriptor set and its layout.
					VkDescriptorPoolSize descPoolSizes[4] = {
                        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, uint32_t(concurrentFrameCount) },
                        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, uint32_t(concurrentFrameCount) },
						{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, uint32_t(concurrentFrameCount) },
						{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, uint32_t(concurrentFrameCount) },
                    };
                    VkDescriptorPoolCreateInfo descPoolInfo;
                    memset(&descPoolInfo, 0, sizeof(descPoolInfo));
                    descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
					descPoolInfo.maxSets = concurrentFrameCount;
                    descPoolInfo.poolSizeCount = sizeof(descPoolSizes) / sizeof(descPoolSizes[0]);;
                    descPoolInfo.pPoolSizes = descPoolSizes;
                    err = m_devFuncs->vkCreateDescriptorPool(dev, &descPoolInfo, nullptr, &m_descPool);
                    if (err != VK_SUCCESS)
                        qFatal("Failed to create descriptor pool: %d", err);
                }

                {
					VkDescriptorSetLayoutBinding layoutBinding[4] =
                    {
                        {
                            details_binding, // binding
                            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                            1, // descriptorCount
                            VK_SHADER_STAGE_FRAGMENT_BIT,
                            &m_sampler_details
                        },



                        {
                            spriteSheet_binding, // binding
                            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                            1, // descriptorCount
                            VK_SHADER_STAGE_FRAGMENT_BIT,
                            &m_sampler_spriteSheet
                        },
                        {
                            lightMap_binding, // binding
                            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                            1, // descriptorCount
                            VK_SHADER_STAGE_FRAGMENT_BIT,
                            &m_sampler_lightMap
						},


						{
							5, // binding
							VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
							1, // descriptorCount
							VK_SHADER_STAGE_VERTEX_BIT,
							nullptr
						},

                    };

                    VkDescriptorSetLayoutCreateInfo descLayoutInfo = {
                        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                        nullptr,
                        0,
						4, // bindingCount
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

					VkWriteDescriptorSet descWrite[4];
                    memset(descWrite, 0, sizeof(descWrite));

                    VkDescriptorImageInfo descImageInfo_details = {
                        m_sampler_details,
                        textureBuffer_details->texView(),
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    };

                    VkDescriptorImageInfo descImageInfo__spriteSheet = {
                        m_sampler_spriteSheet,
                        textureBuffer_spriteSheet->texView(),
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    };

                    VkDescriptorImageInfo descImageInfo_lightMap = {
                        m_sampler_lightMap,
                        textureBuffer_lightMap->texView(),
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    };

                    descWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descWrite[0].dstSet = m_descSet[i];
                    descWrite[0].dstBinding = details_binding;
                    descWrite[0].descriptorCount = 1;
                    descWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    descWrite[0].pImageInfo = &descImageInfo_details;

                    descWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descWrite[1].dstSet = m_descSet[i];
                    descWrite[1].dstBinding = spriteSheet_binding;
                    descWrite[1].descriptorCount = 1;
                    descWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    descWrite[1].pImageInfo = &descImageInfo__spriteSheet;

                    descWrite[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descWrite[2].dstSet = m_descSet[i];
                    descWrite[2].dstBinding = lightMap_binding;
                    descWrite[2].descriptorCount = 1;
                    descWrite[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    descWrite[2].pImageInfo = &descImageInfo_lightMap;




					VkDescriptorBufferInfo buffer =
					{
						uniform_buffer,
						0,
						uniMemReq.size
					};

					descWrite[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descWrite[3].dstSet = m_descSet[i];
					descWrite[3].dstBinding = 5;
					descWrite[3].descriptorCount = 1;
					descWrite[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
					descWrite[3].pBufferInfo = &buffer;



					m_devFuncs->vkUpdateDescriptorSets(dev, 4, descWrite, 0, nullptr);
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
					VkVertexInputBindingDescription vertexBindingDesc[] = {
						{
							0, // binding
							8 * sizeof(float),
							VK_VERTEX_INPUT_RATE_VERTEX
						}
					};


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
					pipelineInfo.stageCount = sizeof(shaderStages) / sizeof(shaderStages[0]);
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

        if (textureBuffer_details == nullptr)
        {
            qDebug() << "error vulkanTextureBuffer";
        }
    }


	{
		if (buffer != nullptr && buffer_size != 0)
		{
			update_uniform(window, buffer, buffer_size);
		}

        VkCommandBuffer cb = window->currentCommandBuffer();

        m_devFuncs->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, CONST_TRANSFORM_OFFSET, CONST_TRANSFORM_SIZE, object->transformConst);
		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, CONST_PROJECTION_OFFSET, CONST_PROJECTION_SIZE, camera->projection);
		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, CONST_UV_TRANSFORM_OFFSET, CONST_UV_TRANSFORM_SIZE, uvTransformConst);
		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, CONST_FRAG_OFFSET, CONST_FRAG_SIZE, fragConst);


		float uvCorrection[4] = {1, 1, 0, 1};

		m_devFuncs->vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, CONST_UV_CORRECTION_OFFSET, CONST_UV_CORRECTION_SIZE, uvCorrection);


		uint32_t frameUniOffset = 0;
		uint32_t frameUniOffsets[] = {frameUniOffset};

        m_devFuncs->vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
											&m_descSet[window->currentFrame()], 1, frameUniOffsets);

        VkBuffer buffer = vertexBuffer->vk_buffer->get();

        VkDeviceSize vbOffset = 0;
        m_devFuncs->vkCmdBindVertexBuffers(cb, 0, 1, &buffer, &vbOffset);

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


void PaintMaterialVulkan::setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource)
{
    (void)textureResource;
    //_textureResource = textureResource;
}


void PaintMaterialVulkan::update_uniform(VulkanWindow *window, const void *data, int size)
{
	VulkanFunctions *m_devFuncs = window->getFunctions();

	VkDevice dev = window->device();

	quint8 *p;
	VkResult err = m_devFuncs->vkMapMemory(dev, m_bufMem, 0, size, 0, reinterpret_cast<void **>(&p));

	if (err != VK_SUCCESS)
		qFatal("Failed to map memory: %d", err);

	memcpy(p, data, size);

	m_devFuncs->vkUnmapMemory(dev, m_bufMem);
}
