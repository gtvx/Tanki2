#include "Model.h"
#include "entrance/platform/client/fp10/core/type/IGameObjectInternal.h"
#include "entrance/alternativa/types/Long.h"
#include <QVector>
#include <QHash>
#include "Error.h"
#include <QDebug>

static IGameObject *currentObject;
static QVector<IGameObject*> objects;


class ModelPrivate
{
public:
	 QHash<IGameObject*, GameObjectValue*> initParams;
};


Model::Model()
{
	p = new ModelPrivate;
}

Model::~Model()
{
	qDebug("~Model %s", typeid(*this).name());
}

IGameObject* Model::object()
{
	//qDebug() << "Model::object" << currentObject;
	return currentObject;
}

void Model::set_object(IGameObject *object)
{
	//qDebug() << "Model::set_object" << object;
	objects.append(currentObject);
	currentObject = object;
}

void Model::popObject()
{
	currentObject = objects.takeLast();
}

void Model::invoke(Long*, ProtocolBuffer*)
{

}

Long* Model::getId()
{
	return nullptr;
}

GameObjectValue* Model::_initParams(IGameObject *object)
{
	auto value = p->initParams[object];
	//qDebug() << "Model::_initParams" << object->id().toString() << typeid(*this).name() << value;
	return value;
}

void Model::_putInitParams(GameObjectValue *value)
{
	//qDebug() << "Model::_putInitParams" << object()->id().toString() << value;
	p->initParams[object()] = value;
}

void Model::clearInitParams()
{
	p->initParams.remove(object());
}

void Model::_putData(const char *name, GameObjectValue *value)
{
	IGameObjectInternal *internal = (IGameObjectInternal*)currentObject;
	internal->_putData(this, name, value);
}

void Model::_clearData(const char *name)
{
	IGameObjectInternal *internal = (IGameObjectInternal*)currentObject;
	return internal->_clearData(this, name);
}


GameObjectValue* Model::_getData(const char *name)
{
	IGameObjectInternal *internal = (IGameObjectInternal*)currentObject;
	return internal->_getData(this, name);
}


#if 0
Function* Model::getFunctionWrapper(Function *)
{
	throw Error("used Model::getFunctionWrapper");
	return nullptr;

	Function *f = param1;
	Dictionary *wrappers = this.getData(Model) as Dictionary;
	if (wrappers == nullptr)
	{
		wrappers = new Dictionary();
		this->putData(Model,wrappers);
	}
	Function *wrapper = wrappers[f];
	if (wrapper == nullptr)
	{
		IGameObject *object = Model.object;

		wrappers[f] = wrapper;
	}
	return wrapper;
}
#endif
