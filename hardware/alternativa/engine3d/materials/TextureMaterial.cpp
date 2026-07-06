#include "TextureMaterial.h"
#include "hardware/alternativa/engine3d/core/Wrapper.h"
#include "MyMath.h"
#include "hardware/alternativa/engine3d/core/Camera3D.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "_global.h"
#include "hardware/alternativa/gfx/core/BitmapTextureResource.h"
#include "TextureResourcesRegistry.h"
#include "flash/display/BitmapData.h"
#include "vulkan/TextureMaterialVulkan.h"
#include "DrawParams.h"
#include "vulkan/VulkanFunctions.h"
#include "vulkan/VulkanWindow.h"
#include "hardware/alternativa/gfx/core/VertexBufferResource.h"
#include "hardware/alternativa/gfx/core/IndexBufferResource.h"
#include "utils/float_array.h"
#include "hardware/alternativa/engine3d/core/MipMapping.h"
#include <QVector>
#include <QDebug>



std::shared_ptr<TextureMaterial> new_TextureMaterial(int id, BitmapData *bitmap, bool repeat, bool smooth, int _mipMapping, double resolution)
{
	return std::make_shared<TextureMaterial>(id, bitmap, repeat, smooth, _mipMapping, resolution);
}



TextureMaterial::TextureMaterial(int id, BitmapData *bitmap, bool repeat, bool smooth, int _mipMapping, double resolution)
{
	this->repeat = repeat;
	this->smooth = smooth;
	this->_mipMapping = _mipMapping;
	this->resolution = resolution;
	this->numMaps = 0;
	this->correctUV = false;
	this->threshold = 0.01;
	this->textureResource = nullptr;
	this->_hardwareMipMaps = false;

	this->id = id;


	if (bitmap != nullptr)
	{
		this->bitmap = bitmap;
		this->textureResource = TextureResourcesRegistry::getTextureResourceS(bitmap, this->_mipMapping > 0, repeat, this->_hardwareMipMaps);
	}


	this->numMaps = 0;
	this->correctUV = false;
	this->threshold = 0.01;
	this->bitmap = nullptr;
	this->_hardwareMipMaps = false;
	//this->_textureATFAlpha = nullptr;
	//this->declaration = nullptr;

	this->repeat = repeat;
	this->smooth = smooth;
	//this->_mipMapping = mipMapping;
	this->resolution = resolution;
	//this->textureATFResource = nullptr;
	//this->textureATFAlphaResource = nullptr;


	//float_array_set_8(correctionConst,  0, 0, 0, 1,  0, 0, 0, 1);
	float_array_set_8(uvTransformConst,  1, 0, 0, 0,  0, 1, 0, 0);
	//float_array_set_4(uvCorrection,  1, 1, 0, 1);
	//float_array_set_4(fragmentConst,  0, 0, 0, 1);


	vulkan = new  TextureMaterialVulkan;


	if (this->textureResource != nullptr)
	{
		vulkan->setTextureResource(this->textureResource);
	}
}


void TextureMaterial::draw(Camera3D *param1, Canvas *param2, Shared<Face> param3, double param4)
{
	(void)param1;
	(void)param2;
	(void)param3;
	(void)param4;

	qDebug("TextureMaterial::draw");
}


BitmapData* TextureMaterial::texture()
{
	if (this->textureResource != nullptr)
	{
		return this->textureResource->bitmapData();
	}

	return nullptr;
}


void TextureMaterial::setTexture(BitmapData *bitmapData)
{
	BitmapData *current = this->texture();

	if (bitmapData != current)
	{
		if (current != nullptr)
		{
			//this->textureResource->dispose();
			this->textureResource = nullptr;
			//this->_textureResource = nullptr;
		}

		if (bitmapData != nullptr)
		{
			std::shared_ptr<BitmapTextureResource> textureResource =
					TextureResourcesRegistry::getTextureResourceS(bitmapData,
																 this->_mipMapping != (int)MipMapping::NONE,
																 this->repeat,
																 this->_hardwareMipMaps);
			setTextureResource(textureResource);
		}
	}
}


bool TextureMaterial::transparent()
{
	//return true;
	if (Material::transparent())
	{
		return true;
	}

	if (this->texture() != nullptr)
	{
		return this->texture()->transparent();
	}

#if 0
	if (this->_textureATF != nullptr)
	{
		return this->_textureATFAlpha != nullptr;
	}
#endif
	return false;
}


bool isLoadResourceVulkan(VulkanWindow *window, GfxResource *resource)
{
	if (resource->vk_isLoaded())
	{
		return true;
	}

	if (window == nullptr)
		return false;


	VkDevice dev = window->device();

	if (dev == nullptr)
		return false;

	VulkanFunctions *m_devFuncs = window->getFunctions();


	if (!resource->vk_create(m_devFuncs, dev, window->hostVisibleMemoryIndex()))
	{
		return false;
	}

	if (resource->vk_upload())
	{
		return true;
	}
	else
	{
		qDebug("error isLoadResourceVulkan");
		return false;
	}
}


void TextureMaterial::drawOpaqueVulkan(DrawParams *p)
{
	if (!isLoadResourceVulkan(p->vulkanWindow, p->vertexBuffer))
	{
		qDebug("error isLoadResource 5");
		return;
	}

	if (!isLoadResourceVulkan(p->vulkanWindow, p->indexBuffer))
	{
		qDebug("error isLoadResource 6");
		return;
	}


	if (this->textureResource == nullptr)
	{
		qDebug() << this->diffuseMapURL << this->opacityMapURL;
		qDebug("error TextureMaterial::drawOpaqueVulkan id = %d", id);
		return;
	}

	vulkan->drawOpaque(p->vulkanWindow,
					   p->camera,
					   p->vertexBuffer,
					   p->indexBuffer,
					   p->firstIndex,
					   p->numTriangles,
					   p->object,
					   p->decal,
					   p->vulkanUniform);
}


void TextureMaterial::drawTransparentVulkan(DrawParams *p)
{
	if (!isLoadResourceVulkan(p->vulkanWindow, p->vertexBuffer))
	{
		qDebug("error isLoadResource 5");
		return;
	}

	if (p->indexBuffer != nullptr && !isLoadResourceVulkan(p->vulkanWindow, p->indexBuffer))
	{
		qDebug("error isLoadResource 6");
		return;
	}

	if (this->textureResource == nullptr)
	{
		qDebug() << this->diffuseMapURL << this->opacityMapURL;
		qDebug("error TextureMaterial::drawOpaqueVulkan id = %d", id);
		return;
	}

	float_array_set_12(p->object->transformConst, 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0);

	vulkan->drawOpaque(p->vulkanWindow,
					   p->camera,
					   p->vertexBuffer,
					   p->indexBuffer,
					   p->firstIndex,
					   p->numTriangles,
					   p->object,
					   p->decal,
					   p->vulkanUniform);
}


void TextureMaterial::setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource)
{
	//this->_textureResource = textureResource;
	this->textureResource = textureResource;
	if (this->vulkan != nullptr)
	{
		this->vulkan->setTextureResource(textureResource);
	}
}
