#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../IGameObject.h"
#include "../IGameObjectInternal.h"
#include "entrance/alternativa/types/Long.h"
#include <QHash>

class IGameClass;

class GameObject :
		//public IGameObject,
		public IGameObjectInternal
{
	QHash<const char*, GameObjectValue*> adapts;

	QHash<Model*, QHash<const char*, GameObjectValue*>*> data;

	QString m_name;

	Long m_id;

	ISpace *m_space;

	GameObjectValue* _adapt(const char *key) override;

	bool _hasModel(const char*) override;

	void _putData(Model*, const char*, GameObjectValue*) override;
	GameObjectValue* _getData(Model*, const char*) override;
	void _clearData(Model*, const char*) override;

public:

	GameObject(const Long &id, IGameClass*, const QString &name, ISpace *space)
	{
		this->m_id = id;
		this->m_name = name;
		this->m_space = space;
	}

	~GameObject();

	Long id() override
	{
		return m_id;
	}

	QString name() override
	{
		return m_name;
	}

	ISpace* space()
	{
		return m_space;
	}


	template <typename T>
	void putAdapt(std::shared_ptr<T> p)
	{
		auto o = new GameObjectValueSharedPtr<T>;
		o->t = p;

		const char *key = typeid(GameObjectValueSharedPtr<T>).name();

		auto e = adapts[key];
		if (e != nullptr)
			delete e;

		adapts[key] = o;
	}


	int count() const override;

	std::string to_string() const override;


	void clear();

};

#endif // GAMEOBJECT_H
