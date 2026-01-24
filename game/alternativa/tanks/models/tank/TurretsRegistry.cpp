#include "TurretsRegistry.h"
#include "game/projects/tanks/clients/flash/resources/resource/Tanks3DSResource.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/turret/TurretSkinCacheItem.h"
#include "entrance/alternativa/types/LongHash.h"

TurretsRegistry::TurretsRegistry()
{

}


TurretSkinCacheItem* TurretsRegistry::getTurret(Tanks3DSResource *param1)
{
	Long id = param1->getId();

	if (_turrets.contains(id))
		return _turrets[id];

	TurretSkinCacheItem *p = new TurretSkinCacheItem(param1);
	_turrets[id] = p;
	return p;
}
