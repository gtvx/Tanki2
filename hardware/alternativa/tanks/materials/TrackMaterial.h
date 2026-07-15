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

	TrackMaterial(std::shared_ptr<BitmapData> texture);

	void update();

	bool transparent() override;
	void drawOpaqueVulkan(DrawParams *p) override;
	void setMipMapping(int value); //override
	void setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource) override;
	void init(DrawInitParams*) override;
};

std::shared_ptr<TrackMaterial> new_TrackMaterial(std::shared_ptr<BitmapData> bitmap);

#endif // TRACKMATERIAL_H
