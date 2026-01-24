#include "HullsRegistry.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankHullSkinCacheItem.h"
#include "game/projects/tanks/clients/flash/resources/resource/Tanks3DSResource.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/turret/TurretSkinCacheItem.h"
#include "entrance/alternativa/types/LongHash.h"


HullsRegistry::HullsRegistry()
{

}


TankHullSkinCacheItem* HullsRegistry::getHull(Tanks3DSResource *p)
{
	Long id = p->getId();

	if (this->_hulls.contains(id))
		return this->_hulls[id];

	TankHullSkinCacheItem *t = new TankHullSkinCacheItem(p);

	this->_hulls[id] = t;

	return t;
}
