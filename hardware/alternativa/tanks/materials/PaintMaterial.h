#ifndef PAINTMATERIAL_H
#define PAINTMATERIAL_H

#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"

class PaintMaterialVulkan;

class PaintMaterial : public TextureMaterial
{

	PaintMaterialVulkan *vulkan;

public:
	PaintMaterial(BitmapData *spriteSheetBitmap,
				  BitmapData *lightMapBitmap,
				  BitmapData *texture,
				  int mipMapping = 0);


	void drawOpaqueVulkan(DrawParams *p) override;
	void drawTransparentVulkan(DrawParams *p) override;

	 void setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource) override;

	 bool transparent();

protected:

	BitmapData *spriteSheetBitmap;
	BitmapData *lightMapBitmap;

	std::shared_ptr<BitmapTextureResource> spriteSheetResource;
	std::shared_ptr<BitmapTextureResource> lightMapResource;

	float fragConst[8];
};


std::shared_ptr<PaintMaterial> new_PaintMaterial(BitmapData *spriteSheetBitmap,
												 BitmapData *lightMapBitmap,
												 BitmapData *texture,
												 int mipMapping = 0);

#endif // PAINTMATERIAL_H
