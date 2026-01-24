#ifndef TANKPARTSREGISTRY_H
#define TANKPARTSREGISTRY_H

class TankHullSkinCacheItem;
class TurretSkinCacheItem;
class Tanks3DSResource;
class HullsRegistry;
class TurretsRegistry;

class TankPartsRegistry
{
	HullsRegistry *hullsRegistry;
	TurretsRegistry *turretsRegistry;

public:
	TankPartsRegistry();
	TankHullSkinCacheItem* getHullDescriptor(Tanks3DSResource *param1);
	TurretSkinCacheItem* getTurretDescriptor(Tanks3DSResource *param1);
};

#endif // TANKPARTSREGISTRY_H
