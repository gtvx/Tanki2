#include "PaintMaterial.h"
#include "utils/float_array.h"
#include "flash/display/BitmapData.h"
#include "hardware/alternativa/engine3d/core/MipMapping.h"
#include "hardware/alternativa/engine3d/materials/TextureResourcesRegistry.h"
#include "hardware/alternativa/gfx/core/BitmapTextureResource.h"
#include "vulkan/PaintMaterialVulkan.h"
#include "flash/display/BitmapData.h"
#include "hardware/alternativa/engine3d/materials/DrawParams.h"
#include "hardware/alternativa/gfx/core/VertexBufferResource.h"
#include "hardware/alternativa/gfx/core/IndexBufferResource.h"
#include <QDebug>


bool isLoadResourceVulkan(VulkanWindow *window, GfxResource *resource);


std::shared_ptr<PaintMaterial> new_PaintMaterial(std::shared_ptr<BitmapData> spriteSheetBitmap,
												 std::shared_ptr<BitmapData> lightMapBitmap,
												 std::shared_ptr<BitmapData> texture,
												 int mipMapping)
{
	return std::make_shared<PaintMaterial>(spriteSheetBitmap, lightMapBitmap, texture, mipMapping);
}



PaintMaterial::PaintMaterial(std::shared_ptr<BitmapData> spriteSheetBitmap,
							 std::shared_ptr<BitmapData> lightMapBitmap,
							 std::shared_ptr<BitmapData> texture,
							 int mipMapping) :
	TextureMaterial(10, texture, true, true, mipMapping)
{
	this->spriteSheetBitmap = spriteSheetBitmap;
	this->lightMapBitmap = lightMapBitmap;

	this->spriteSheetResource = TextureResourcesRegistry::getTextureResourceS(spriteSheetBitmap, (_mipMapping > 0), true, false);
	this->lightMapResource = TextureResourcesRegistry::getTextureResourceS(lightMapBitmap, (_mipMapping > 0), true, false);


	float_array_set_8(fragConst, 0, 0.5, 1, 2, 0.999, 0.999, 0, 0);


	uvTransformConst[0] = (double(texture->width()) / spriteSheetBitmap->width());
	uvTransformConst[5] = (texture->height() / spriteSheetBitmap->height());

	_mipMapping = this->spriteSheetResource->getMipMapping() ? (int)MipMapping::PER_PIXEL : 0;

	//std::shared_ptr<BitmapData> s_spriteSheetBitmap = std::make_shared<BitmapData>();
	//s_spriteSheetBitmap->copy(spriteSheetBitmap);

	//std::shared_ptr<BitmapData> s_lightMapBitmap = std::make_shared<BitmapData>();
	//s_lightMapBitmap->copy(lightMapBitmap);

	//std::shared_ptr<BitmapData> s_texture = std::make_shared<BitmapData>();
	//s_texture->copy(texture);


	vulkan = new PaintMaterialVulkan(spriteSheetBitmap, lightMapBitmap, texture);
}


void PaintMaterial::init(DrawInitParams *p)
{
	vulkan->init(p);
}


bool PaintMaterial::transparent()
{
	//return true;
	return false;
}


void PaintMaterial::drawOpaqueVulkan(DrawParams *p)
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

	vulkan->drawOpaque(p->vulkanWindow,
					   p->camera,
					   p->vertexBuffer,
					   p->indexBuffer,
					   p->firstIndex,
					   p->numTriangles,
					   p->object,
					   this->uvTransformConst,
					   this->fragConst);
}


void PaintMaterial::drawTransparentVulkan(DrawParams *p)
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

	vulkan->drawOpaque(p->vulkanWindow,
					   p->camera,
					   p->vertexBuffer,
					   p->indexBuffer,
					   p->firstIndex,
					   p->numTriangles,
					   p->object,
					   this->uvTransformConst,
					   this->fragConst);

}


void PaintMaterial::setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource)
{
	TextureMaterial::setTextureResource(textureResource);
	vulkan->setTextureResource(textureResource);
}

