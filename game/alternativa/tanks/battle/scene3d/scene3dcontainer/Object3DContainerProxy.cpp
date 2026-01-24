#include "Object3DContainerProxy.h"
#include "hardware/alternativa/engine3d/core/Camera3D.h"
#include "hardware/alternativa/engine3d/core/Object3DContainer.h"
#include <Error.h>
#include <QVector>


Object3DContainerProxy::Object3DContainerProxy(std::shared_ptr<Object3DContainer> param1)
{
	this->container = new_Object3DContainer();
	this->setContainer(param1);
}


void Object3DContainerProxy::addChild(std::shared_ptr<Object3D> object3D)
{
	if (object3D == nullptr)
		throw ArgumentError("Parameter is null");

	if (!this->objects.contains(object3D))
	{
		this->objects[object3D] = true;
		this->container->addChild(object3D);
	}
}


void Object3DContainerProxy::setContainer(std::shared_ptr<Object3DContainer> container)
{
	QVector<std::shared_ptr<Object3D>> list = this->removeAllChildren();
	this->container = container != nullptr ? container : new_Object3DContainer();
	this->addChildren(list);
}


void Object3DContainerProxy::addChildren(QVector<std::shared_ptr<Object3D>> &list)
{
	for (std::shared_ptr<Object3D> item : list)
	{
		this->addChild(item);
	}
}


QVector<std::shared_ptr<Object3D>> Object3DContainerProxy::removeAllChildren()
{
	QVector<std::shared_ptr<Object3D>> result;

	QMapIterator<std::shared_ptr<Object3D>, bool> iter(this->objects);

	while (iter.hasNext()) {
		iter.next();
		std::shared_ptr<Object3D> object = iter.key();
		this->container->removeChild(object);
		result.append(object);
	}

	this->objects.clear();

	return result;
}


void Object3DContainerProxy::removeChild(std::shared_ptr<Object3D> object3D)
{
	if (object3D == nullptr)
		throw ArgumentError("Parameter is null");

	if (this->objects.contains(object3D))
	{
		this->objects.remove(object3D);
		this->container->removeChild(object3D);
	}
}


void Object3DContainerProxy::addChildAt(std::shared_ptr<Object3D> object3D, int v)
{
	if (object3D == nullptr)
	{
		throw ArgumentError("Parameter is null");
	}

	if (!this->objects.contains(object3D))
	{
		this->objects[object3D] = true;
		this->container->addChildAt(object3D, v);
	}
}
