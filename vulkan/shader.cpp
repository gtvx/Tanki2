#include "shader.h"
#include "VulkanFunctions.h"
#include <QFile>


VkShaderModule shader_create(VulkanFunctions *m_devFuncs, VkDevice dev, const QString &fn)
{
    QFile f(fn);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning("Failed to open %s", qPrintable(fn));
        return VK_NULL_HANDLE;
    }

    QByteArray blob = f.readAll();
    VkShaderModuleCreateInfo shaderInfo;
    memset(&shaderInfo, 0, sizeof(shaderInfo));
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = blob.size();
    shaderInfo.pCode = reinterpret_cast<const uint32_t *>(blob.constData());

    VkShaderModule shaderModule;
    VkResult err = m_devFuncs->vkCreateShaderModule(dev, &shaderInfo, nullptr, &shaderModule);

    if (err != VK_SUCCESS) {
        qWarning("Failed to create shader module: %d", err);
        return VK_NULL_HANDLE;
    }

    return shaderModule;
}


void init_shader_stage_vertex(VkPipelineShaderStageCreateInfo *shaderStage, VkShaderModule shaderModule)
{
    shaderStage->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage->pNext = nullptr;
    shaderStage->flags = 0;
    shaderStage->stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStage->module = shaderModule;
    shaderStage->pName = "main";
    shaderStage->pSpecializationInfo = nullptr;
}


void init_shader_stage_fragment(VkPipelineShaderStageCreateInfo *shaderStage, VkShaderModule shaderModule)
{
    shaderStage->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage->pNext = nullptr;
    shaderStage->flags = 0;
    shaderStage->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStage->module = shaderModule;
    shaderStage->pName = "main";
    shaderStage->pSpecializationInfo = nullptr;
}
