#ifndef HULLSREGISTRY_H
#define HULLSREGISTRY_H

#include <QHash>

class TankHullSkinCacheItem;
class Tanks3DSResource;
class Long;

class HullsRegistry
{
	QHash<Long, TankHullSkinCacheItem*> _hulls;

public:
	HullsRegistry();
	TankHullSkinCacheItem *getHull(Tanks3DSResource*);
};

#endif // HULLSREGISTRY_H
