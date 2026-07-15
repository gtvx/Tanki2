#include "TankSkinPartCacheItem.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "hardware/alternativa/engine3d/core/Sorting.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "entrance/platform/client/fp10/core/resource/types/StubBitmapData.h"
#include "game/projects/tanks/clients/flash/resources/resource/Tanks3DSResource.h"
#include <QHash>


static const QString DETAILS = "details.png";
static const QString LIGHTMAP = "lightmap.jpg";

static QHash<QString, std::shared_ptr<BitmapData>> stubBitmaps;



std::shared_ptr<BitmapData> getStubBitmapData(const QString &name, uint _arg_2)
{
	std::shared_ptr<BitmapData> bitmapData = stubBitmaps[name];

	if (bitmapData == nullptr)
	{
		bitmapData = std::make_shared<StubBitmapData>(_arg_2);
		stubBitmaps[name] = bitmapData;
	}

	return bitmapData;
}


TankSkinPartCacheItem::TankSkinPartCacheItem(Tanks3DSResource *resourse)
{
	this->partId = resourse->getId();

	this->details = resourse->textures[DETAILS];
	this->lightmap = resourse->textures[LIGHTMAP];

	if (this->details == nullptr)
		this->details = getStubBitmapData(DETAILS, 0xFF00);

	if (this->lightmap == nullptr)
		this->lightmap = getStubBitmapData(LIGHTMAP, 0x7F7F7F);
}


BitmapData* TankSkinPartCacheItem::createTexture(BitmapData *in)
{
	qDebug("used TankSkinPartCacheItem::createTexture");
	return in;

#if 0
	BitmapData *_local_2 = new BitmapData(this->lightmap->width, this->lightmap->height, false, 0);
	Shape *_local_3 = new Shape();
	_local_3.graphics.beginBitmapFill(_arg_1);
	_local_3.graphics.drawRect(0, 0, this.lightmap.width, this.lightmap.height);
	_local_2.draw(_local_3);
	_local_2.draw(this.lightmap, null, null, BlendMode.HARDLIGHT);
	_local_2.draw(this.details);
	_local_3 = null;
	return (_local_2);
#endif
}

std::shared_ptr<Mesh> TankSkinPartCacheItem::initMesh(std::shared_ptr<Mesh> mesh)
{
	if (mesh->sorting != Sorting::DYNAMIC_BSP)
	{
		mesh->sorting = Sorting::DYNAMIC_BSP;
		mesh->calculateFacesNormals(true);
		mesh->optimizeForDynamicBSP();
		mesh->threshold = 0.01;
	}
	return mesh;
}
