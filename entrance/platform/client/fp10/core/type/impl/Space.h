#ifndef SPACE_H
#define SPACE_H

#include "../ISpace.h"
#include "entrance/alternativa/types/Long.h"
#include <QHash>

class IGameObject;
class GameObject;
class ISpaceCommandHandler;
class IProtocol;

class Space : public ISpace
{

	QHash<Long, GameObject*> objectById;
	QVector<IGameObject*> _objects;

	Long _id;

public:
	Space(const Long &id, ISpaceCommandHandler *handler, IProtocol *protocol, bool protection);

	Long getId() override;

	IGameObject* getObject(const Long &id) override;

	IGameObject* getRootObject() override;

	IGameObject* createObject(const Long &id, IGameClass*, const QString &name) override;

	void destroyObject(const Long &id) override;

	void close() override;

private:
	void removeObject(GameObject *object);
};

#endif // SPACE_H
