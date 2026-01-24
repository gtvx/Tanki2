#include "Pool.h"
#include "PooledObject.h"


Pool::Pool()
{
	numObjects = 0;
}


void Pool::putObject(PooledObject *object)
{
	int index = this->numObjects++;
	if (this->objects.length() < this->numObjects)
		this->objects.resize(this->numObjects);
	this->objects[index] = object;
}


void Pool::clear()
{
	for (PooledObject *object : objects)
	{
		delete object;
	}

	this->objects.clear();
	this->numObjects = 0;
}
