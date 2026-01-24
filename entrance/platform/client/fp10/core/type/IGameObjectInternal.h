#ifndef IGAMEOBJECTINTERNAL_H
#define IGAMEOBJECTINTERNAL_H

#include "IGameObject.h"

class Model;

class IGameObjectInternal : public IGameObject
{
	virtual void _putData(Model*, const char*, GameObjectValue*) = 0;
	virtual GameObjectValue* _getData(Model*, const char*) = 0;
	virtual void _clearData(Model*, const char*) = 0;

	friend class Model;

public:

	template <typename T>
	void putData(Model *model, std::shared_ptr<T> p)
	{
		auto o = new GameObjectValueSharedPtr<T>;
		o->t = p;

		_putData(model, typeid(GameObjectValueSharedPtr<T>).name(), o);
	}


	template <typename T>
	std::shared_ptr<T> getData(Model *model)
	{
		GameObjectValue *o = _getData(model, typeid(GameObjectValueSharedPtr<T>).name());
		if (o == nullptr)
			return nullptr;

		return ((GameObjectValueSharedPtr<T>*)o)->t;
	}

	template <typename T>
	void clearData(Model *model)
	{
		GameObjectValue *o = _clearData(model, typeid(GameObjectValueSharedPtr<T>).name());

		if (o != nullptr)
		{
			delete o;
		}
	}

};

#endif // IGAMEOBJECTINTERNAL_H
