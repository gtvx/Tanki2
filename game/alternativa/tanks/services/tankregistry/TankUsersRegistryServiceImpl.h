#ifndef TANKUSERSREGISTRYSERVICEIMPL_H
#define TANKUSERSREGISTRYSERVICEIMPL_H

#include "TankUsersRegistry.h"
#include <QHash>

class TankUsersRegistryServiceImpl : public TankUsersRegistry
{
	MyList<IGameObject*> userList;
	QHash<Long, IGameObject*> users;

	int userCount;
	IGameObject *localUser;

public:
	TankUsersRegistryServiceImpl();

	void addUser(IGameObject*) override;
	void removeUser(IGameObject*) override;
	MyList<IGameObject*>* getUsers() override;
	IGameObject* getUser(const Long&) override;
	IGameObject* getLocalUser() override;
};

#endif // TANKUSERSREGISTRYSERVICEIMPL_H
