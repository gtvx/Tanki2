#ifndef GAMEOBJECTVALUE_H
#define GAMEOBJECTVALUE_H

#include <memory>

struct GameObjectValue
{
	virtual ~GameObjectValue()
	{

	}
};

struct GameObjectValueSharedBool: GameObjectValue
{
	bool status;
};

struct GameObjectValueSharedInt: GameObjectValue
{
	int value;
};

template <typename T>
struct GameObjectValueSharedPtr: GameObjectValue
{
	std::shared_ptr<T> t;
};

#endif // GAMEOBJECTVALUE_H
