#ifndef SHADER_H
#define SHADER_H

#include <vulkan/vulkan.h>

class VulkanFunctions;
class QString;

VkShaderModule shader_create(VulkanFunctions *m_devFuncs, VkDevice dev, const QString &fn);

void init_shader_stage_vertex(VkPipelineShaderStageCreateInfo *shaderStage, VkShaderModule shaderModule);
void init_shader_stage_fragment(VkPipelineShaderStageCreateInfo *shaderStage, VkShaderModule shaderModule);

#endif
