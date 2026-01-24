#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include "GameObjectValue.h"

class Long;
class QString;
class IGameClass;
class ISpace;
class Class;

class IGameObject
{	
protected:

public:
	virtual Long id() = 0;
	virtual QString name() = 0;
	virtual ISpace* space() = 0;


	template <typename T>
	std::shared_ptr<T> adapt()
	{
		GameObjectValue *o = _adapt(typeid(GameObjectValueSharedPtr<T>).name());
		if (o == nullptr)
			return nullptr;

		return ((GameObjectValueSharedPtr<T>*)o)->t;
	}


	template <typename T>
	bool hasModel()
	{
		return _hasModel(typeid(GameObjectValueSharedPtr<T>).name());
	}


	virtual int count() const = 0;

	virtual std::string to_string() const = 0;

private:
	virtual GameObjectValue* _adapt(const char*) = 0;
	virtual bool _hasModel(const char*) = 0;
};

#endif // IGAMEOBJECT_H
