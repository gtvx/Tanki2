#ifndef TURRETSREGISTRY_H
#define TURRETSREGISTRY_H

#include "entrance/alternativa/types/Long.h"
#include <QHash>

class TurretSkinCacheItem;
class Tanks3DSResource;

class TurretsRegistry
{
	QHash<Long, TurretSkinCacheItem*> _turrets;

public:
	TurretsRegistry();
	TurretSkinCacheItem *getTurret(Tanks3DSResource *param1);
};

#endif // TURRETSREGISTRY_H
