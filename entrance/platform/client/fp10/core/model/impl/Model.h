#ifndef MODEL_H
#define MODEL_H

#include "entrance/platform/client/fp10/core/model/IModel.h"
#include "entrance/platform/client/fp10/core/type/GameObjectValue.h"
#include <type_traits>

class IGameObject;
class Function;
class ModelPrivate;

class Model : public IModel
{
	ModelPrivate *p;

	void _putData(const char*, GameObjectValue*);
	GameObjectValue* _getData(const char*);
	void _clearData(const char*);

	void _putInitParams(GameObjectValue*);
	GameObjectValue* _initParams(IGameObject *o);


public:
	Model();
	virtual ~Model();

	static IGameObject* object();
	static void set_object(IGameObject*);
	static void popObject();
	void invoke(Long*, ProtocolBuffer*);
	Long* getId();


	template <typename T>
	void putInitParams(std::shared_ptr<T> p)
	{
		auto o = new GameObjectValueSharedPtr<T>;
		o->t = p;
		_putInitParams(o);
	}

	template <typename T>
	std::shared_ptr<T> initParams(IGameObject *gameObject)
	{
		GameObjectValue *o = _initParams(gameObject);
		if (o == nullptr)
			return nullptr;

		return ((GameObjectValueSharedPtr<T>*)o)->t;
	}


	void clearInitParams();



	template<typename T, typename = std::enable_if_t<std::is_same_v<T, bool>>>
	void putData(bool status)
	{
		auto o = new GameObjectValueSharedBool;
		o->status = status;

		_putData(typeid(GameObjectValueSharedBool).name(), o);
	}

	template<typename T, typename = std::enable_if_t<std::is_same_v<T, bool>>>
	bool getData()
	{
		GameObjectValue *o = _getData(typeid(GameObjectValueSharedBool).name());
		if (o == nullptr)
			return false;

		return ((GameObjectValueSharedBool*)o)->status;
	}




	template<typename T, typename = std::enable_if_t<std::is_same_v<T, int>>>
	void putData(int value)
	{
		auto o = new GameObjectValueSharedInt;
		o->value = value;

		_putData(typeid(GameObjectValueSharedInt).name(), o);
	}

	template<typename T, typename = std::enable_if_t<std::is_same_v<T, int>>>
	int getData()
	{
		GameObjectValue *o = _getData(typeid(GameObjectValueSharedInt).name());
		if (o == nullptr)
			return 0;

		return ((GameObjectValueSharedInt*)o)->value;
	}






	template <typename T>
	void putData(std::shared_ptr<T> p)
	{
		auto o = new GameObjectValueSharedPtr<T>;
		o->t = p;

		_putData(typeid(GameObjectValueSharedPtr<T>).name(), o);
	}


	template <typename T>
	std::shared_ptr<T> getData()
	{
		GameObjectValue *o = _getData(typeid(GameObjectValueSharedPtr<T>).name());
		if (o == nullptr)
			return nullptr;

		return ((GameObjectValueSharedPtr<T>*)o)->t;
	}



	template <typename T>
	void clearData()
	{
		GameObjectValue *o = _clearData(typeid(GameObjectValueSharedPtr<T>).name());

		if (o != nullptr)
		{
			delete o;
		}
	}

};

#endif // MODEL_H
