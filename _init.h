#ifndef _INIT_H
#define _INIT_H

#include <stdint.h>

class Tank;
class ResourceRegistry;
class TankManager;
class TankState;

class _init
{

	TankManager *tankManager;

public:

	_init(ResourceRegistry *resourceRegistry);

	Tank* createTank(uint32_t id, bool local, float health, TankState *tankstate);
};

#endif // _INIT_H
