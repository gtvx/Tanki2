#include "TankUsersRegistryServiceImpl.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "Error.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "entrance/alternativa/types/Long.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "game/alternativa/tanks/models/tank/ITankModel.h"
#include "entrance/alternativa/types/LongHash.h"


TankUsersRegistryServiceImpl::TankUsersRegistryServiceImpl()
{
	userCount = 0;
	localUser = nullptr;
}


void TankUsersRegistryServiceImpl::addUser(IGameObject *user)
{
	this->userList.clear();

	if (!users.contains(user->id()))
	{
		this->users[user->id()] = user;
		this->userCount++;
	}

	if (user->adapt<ITankModel>()->isLocal())
	{
		this->localUser = user;
	}
}


void TankUsersRegistryServiceImpl::removeUser(IGameObject *user)
{
	this->userList.clear();

	if (users.remove(user->id()))
	{
		this->userCount--;
	}

	if (user->adapt<ITankModel>()->isLocal())
	{
		this->localUser = nullptr;
	}
}


MyList<IGameObject*>* TankUsersRegistryServiceImpl::getUsers()
{
	if (this->userList.count() == 0)
	{
		if (this->users.isEmpty())
		{
			return &this->userList;
		}

		QHashIterator<Long, IGameObject*> iter = this->users;

		while (iter.hasNext())
		{
			iter.next();
			userList.append(iter.value());
		}
	}

	return &this->userList;
}


IGameObject* TankUsersRegistryServiceImpl::getUser(const Long &id)
{
	return users[id];
}


IGameObject* TankUsersRegistryServiceImpl::getLocalUser()
{
	return localUser;
}
