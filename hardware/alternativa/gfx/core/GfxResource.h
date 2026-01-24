#ifndef GFXRESOURCE_H
#define GFXRESOURCE_H

#include <stdint.h>

class VulkanFunctions;

typedef struct VkDevice_T *VkDevice;

class GfxResource
{
public:

	GfxResource();

	virtual ~GfxResource();

	virtual bool vk_create(VulkanFunctions *m_devFuncs,
						   VkDevice dev,
						   uint32_t hostVisibleMemoryIndex);

	virtual bool vk_upload();

	virtual bool vk_isLoaded();
};

#endif // GFXRESOURCE_H
