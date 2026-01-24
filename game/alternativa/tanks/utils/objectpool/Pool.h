#ifndef POOL_H
#define POOL_H

#include <QVector>

class PooledObject;

class Pool
{
	int numObjects;

	QVector<PooledObject*> objects;

public:
	Pool();

	template <typename T>
	T* getObject()
	{
		if (this->numObjects == 0)
		{
			return new T(this);
		}

		PooledObject *object = this->objects[--this->numObjects];
		this->objects[this->numObjects] = nullptr;
		return (T*)object;
	}

	void putObject(PooledObject *object);

	void clear();
};

#endif // POOL_H
