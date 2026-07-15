#ifndef BILLBOARDS_H
#define BILLBOARDS_H

#include <memory>
#include <QVector>
#include "shared.h"

class Mesh;
class BitmapData;
class TextureMaterial;
class Face;

class Billboards
{
	QVector<std::shared_ptr<Mesh>> billboards;
	QVector<Shared<Face>> faces;
	std::shared_ptr<TextureMaterial> material;
	std::shared_ptr<BitmapData> billboardImage;

public:
	Billboards();
	void add(std::shared_ptr<Mesh> mesh);
	void setImage(std::shared_ptr<BitmapData> bitmapData);

private:
	void updateMaterial();
	void setMaterialResolution();
};

#endif // BILLBOARDS_H
