#ifndef PAINTMATERIAL_H
#define PAINTMATERIAL_H

#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"

class PaintMaterialVulkan;

class PaintMaterial : public TextureMaterial
{

	PaintMaterialVulkan *vulkan;

public:
	PaintMaterial(std::shared_ptr<BitmapData> spriteSheetBitmap,
				  std::shared_ptr<BitmapData> lightMapBitmap,
				  std::shared_ptr<BitmapData> texture,
				  int mipMapping = 0);

	void init(DrawInitParams*) override;

	void drawOpaqueVulkan(DrawParams *p) override;
	void drawTransparentVulkan(DrawParams *p) override;

	void setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource) override;

	bool transparent();


protected:

	std::shared_ptr<BitmapData> spriteSheetBitmap;
	std::shared_ptr<BitmapData> lightMapBitmap;

	std::shared_ptr<BitmapTextureResource> spriteSheetResource;
	std::shared_ptr<BitmapTextureResource> lightMapResource;

	float fragConst[8];
};


std::shared_ptr<PaintMaterial> new_PaintMaterial(std::shared_ptr<BitmapData> spriteSheetBitmap,
												 std::shared_ptr<BitmapData> lightMapBitmap,
												 std::shared_ptr<BitmapData> texture,
												 int mipMapping = 0);

#endif // PAINTMATERIAL_H
