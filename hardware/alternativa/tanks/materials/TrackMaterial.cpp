#include "TrackMaterial.h"
#include "hardware/alternativa/engine3d/materials/UVMatrixProvider.h"
#include "vulkan/TrackMaterialVulkan.h"
#include "hardware/alternativa/engine3d/materials/DrawParams.h"
#include "hardware/alternativa/gfx/core/VertexBufferResource.h"
#include "hardware/alternativa/gfx/core/IndexBufferResource.h"
#include "hardware/alternativa/engine3d/materials/TextureResourcesRegistry.h"

bool isLoadResourceVulkan(VulkanWindow *window, GfxResource *resource);


std::shared_ptr<TrackMaterial> new_TrackMaterial(std::shared_ptr<BitmapData> bitmap)
{
	return std::make_shared<TrackMaterial>(bitmap);
}



TrackMaterial::TrackMaterial(std::shared_ptr<BitmapData> bitmap) :
	TextureMaterial(12, bitmap, true)
{
	uvMatrixProvider = new UVMatrixProvider;
	vulkan = new TrackMaterialVulkan;

	if (bitmap != nullptr)
	{
		this->bitmap = bitmap;
		this->textureResource = TextureResourcesRegistry::getTextureResourceS(bitmap, this->_mipMapping > 0, repeat, this->_hardwareMipMaps);

		setTextureResource(this->textureResource);
	}
}


void TrackMaterial::init(DrawInitParams *p)
{
	vulkan->init(p);
}


void TrackMaterial::update()
{
	this->uvMatrixProvider->getValues(uvTransformConst);
}


bool TrackMaterial::transparent()
{
	return false;
}


void TrackMaterial::setMipMapping(int value)
{
	_mipMapping = value;
	//textureResource = TextureResourcesRegistry::getTextureResource(bitmap, _mipMapping, repeat, _hardwareMipMaps);
}


void TrackMaterial::drawOpaqueVulkan(DrawParams *p)
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

	this->update();

	vulkan->drawOpaque(p->vulkanWindow,
					   p->camera,
					   p->vertexBuffer,
					   p->indexBuffer,
					   p->firstIndex,
					   p->numTriangles,
					   p->object,
					   this->uvTransformConst);
}


void TrackMaterial::setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource)
{
	TextureMaterial::setTextureResource(textureResource);
	vulkan->setTextureResource(textureResource);
}
