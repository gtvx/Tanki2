#include "TankPartsRegistry.h"
#include "HullsRegistry.h"
#include "TurretsRegistry.h"


TankPartsRegistry::TankPartsRegistry()
{
	this->hullsRegistry = new HullsRegistry();
	this->turretsRegistry = new TurretsRegistry();
}


TankHullSkinCacheItem* TankPartsRegistry::getHullDescriptor(Tanks3DSResource *param1)
{
   return this->hullsRegistry->getHull(param1);
}


TurretSkinCacheItem* TankPartsRegistry::getTurretDescriptor(Tanks3DSResource *param1)
{
   return this->turretsRegistry->getTurret(param1);
}
