#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <QHash>
#include "Pool.h"

class ObjectPool
{
	QHash<const char*, Pool*> pools;

public:
	ObjectPool();


	template <typename T>
	T* getObject()
	{
		const char* name = typeid(T).name();

		Pool *pool = this->pools[name];
		if (pool == nullptr)
		{
			pool = new Pool();
			this->pools[name] = pool;
		}
		return pool->getObject<T>();
	}

	void clear();

	void clearPoolForClass(const char *name);

	QString toString() const;
};

#endif // OBJECTPOOL_H
