#ifndef TRACKMATERIAL_H
#define TRACKMATERIAL_H

#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"

class UVMatrixProvider;
class TrackMaterialVulkan;

class TrackMaterial : public TextureMaterial
{
public:

	UVMatrixProvider *uvMatrixProvider;
	TrackMaterialVulkan *vulkan;

	TrackMaterial(BitmapData *texture);

	void update();

	bool transparent() override;
	void drawOpaqueVulkan(DrawParams *p) override;
	void setMipMapping(int value); //override
	void setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource) override;
};

std::shared_ptr<TrackMaterial> new_TrackMaterial(BitmapData *bitmap);

#endif // TRACKMATERIAL_H
