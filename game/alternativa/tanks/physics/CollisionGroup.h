#ifndef COLLISIONGROUP_H
#define COLLISIONGROUP_H

struct CollisionGroup
{
	enum : int
	{
		EMPTY2 = -1,
		EMPTY = 0,
		TANK = 1,
		ACTIVE_TRACK = 2,
		INACTIVE_TRACK = 4,
		WEAPON = 8,
		STATIC = 16,
		BONUS_WITH_TANK = 64,
		STATIC_GROUP = 255
	};
};

#endif // COLLISIONGROUP_H
