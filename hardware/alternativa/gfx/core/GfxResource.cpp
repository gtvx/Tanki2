#include "GfxResource.h"
#include <QDebug>


GfxResource::GfxResource()
{

}


GfxResource::~GfxResource()
{

}


bool GfxResource::vk_create(VulkanFunctions *m_devFuncs, VkDevice dev, uint32_t hostVisibleMemoryIndex)
{
	qDebug() << "error Resource::vk_create";
	(void)m_devFuncs;
	(void)dev;
	(void)hostVisibleMemoryIndex;
	return false;
}


bool GfxResource::vk_upload()
{
	qDebug() << "error Resource::vk_upload";
	return false;
}


bool GfxResource::vk_isLoaded()
{
	qDebug() << "error Resource::vk_isLoaded";
	return false;
}
