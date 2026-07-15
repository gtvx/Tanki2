#ifndef TANKSKINPARTCACHEITEM_H
#define TANKSKINPARTCACHEITEM_H

#include <memory>
#include "entrance/alternativa/types/Long.h"

class Mesh;
class Long;
class BitmapData;
class Tanks3DSResource;

class TankSkinPartCacheItem
{
public:
	TankSkinPartCacheItem(Tanks3DSResource *resourse);

	BitmapData* createTexture(BitmapData*);

	Long partId;
	std::shared_ptr<BitmapData> details;
	std::shared_ptr<BitmapData> lightmap;

protected:
	std::shared_ptr<Mesh> initMesh(std::shared_ptr<Mesh> param1);
};

#endif // TANKSKINPARTCACHEITEM_H
