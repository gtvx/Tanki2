#include "Space.h"
#include "GameObject.h"
#include "entrance/alternativa/types/LongHash.h"
#include "Error.h"


Space::Space(const Long &id, ISpaceCommandHandler *handler, IProtocol *protocol, bool protection)
{
	(void)handler;
	(void)protocol;
	(void)protection;

	this->_id = id;

	this->createObject(id, nullptr, "Space object");
}

Long Space::getId()
{
	return _id;
}

IGameObject* Space::getObject(const Long &id)
{
	return this->objectById[id];
}

IGameObject* Space::getRootObject()
{
	return this->getObject(this->_id);
}

IGameObject* Space::createObject(const Long &id, IGameClass *gameClass, const QString &name)
{
	if (this->objectById[id] == nullptr)
	{
		GameObject *gameObject = new GameObject(id, gameClass, name, this);
		this->objectById[gameObject->id()] = gameObject;
		this->_objects.append(gameObject);
	}

	return this->objectById[id];
}

void Space::destroyObject(const Long &objectId)
{
	GameObject *clientObject = this->objectById[objectId];
	if (clientObject != nullptr)
	{
		try
		{
			//this.invokeUnloadListeners(clientObject);
			//this.invokeDestructionListeners(clientObject);
		}
		catch(Error&)
		{
			//processUnloadingError(e, objectId);
		}

		this->removeObject(clientObject);
	}
}

void Space::removeObject(GameObject *object)
{
	this->_objects.removeAll(object);
	//this->_objects->splice(this->_objects.indexOf(object), 1);
	this->objectById.remove(object->id());
	object->clear();
}


void Space::close()
{

}
