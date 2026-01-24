#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "Material.h"
#include <QVector>

class BitmapTextureResource;
class BitmapData;
class TextureMaterialVulkan;

class TextureMaterial : public Material
{
	TextureMaterialVulkan *vulkan;

public:
	TextureMaterial(int id, BitmapData *bitmap = nullptr, bool repeat = false, bool smooth = true, int _mipMapping = 0, double resolution = 1);

	QString diffuseMapURL;
	QString opacityMapURL;
	virtual void draw(Camera3D *param1, Canvas *param2, Shared<Face> param3, double param4) override;

	std::shared_ptr<BitmapTextureResource> textureResource;

	double resolution;
	double threshold;

	float uvTransformConst[8];

	int numMaps;
	int _mipMapping;
	int id;

	bool smooth;
	bool repeat;
	bool correctUV;
	bool _hardwareMipMaps;

	BitmapData *bitmap;
	QVector<BitmapData*> mipMap;

	void setTexture(BitmapData *texture);
	BitmapData* texture();

	bool transparent() override;

	void drawOpaqueVulkan(DrawParams *p) override;
	void drawTransparentVulkan(DrawParams *p) override;

	virtual void setTextureResource(std::shared_ptr<BitmapTextureResource> &textureResource);

};


std::shared_ptr<TextureMaterial> new_TextureMaterial(int id, BitmapData *bitmap = nullptr, bool repeat = false, bool smooth = true, int _mipMapping = 0, double resolution = 1);

#endif // TEXTUREMATERIAL_H
