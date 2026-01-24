#ifndef TANKUSERSREGISTRY_H
#define TANKUSERSREGISTRY_H

#include "MyList.h"

class Long;
class IGameObject;

class TankUsersRegistry
{
public:
	virtual void addUser(IGameObject*) = 0;
	virtual void removeUser(IGameObject*) = 0;
	virtual MyList<IGameObject*>* getUsers() = 0;
	virtual IGameObject* getUser(const Long&) = 0;
	virtual IGameObject* getLocalUser() = 0;

	/*
	virtual void addUser(IGameObject) = 0;
	virtual void removeUser(IGameObject) = 0;
	virtual int getUserCount() = 0;
	virtual QVector<IGameObject> getUsers() = 0;
	virtual IGameObject getUser(const Long&) = 0;
	virtual IGameObject getLocalUser() = 0;
	virtual bool existLocalUser() = 0;
	*/
};

#endif // TANKUSERSREGISTRY_H
