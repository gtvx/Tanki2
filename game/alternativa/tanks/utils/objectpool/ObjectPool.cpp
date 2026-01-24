#include "ObjectPool.h"


ObjectPool::ObjectPool()
{

}


void ObjectPool::clear()
{
	QHashIterator<const char*, Pool*> i(pools);

	while (i.hasNext()) {
		i.next();

		Pool *pool = i.value();
		pool->clear();
		delete pool;
	}

	pools.clear();
}


void ObjectPool::clearPoolForClass(const char *name)
{
	Pool *pool = this->pools[name];
	if (pool != nullptr)
	{
		pool->clear();
	}
}


QString ObjectPool::toString() const
{
	/*
	var _local_2:*;
	var _local_3:Pool;
	var _local_1:String = "";
	for (_local_2 in this.pools)
	{
		_local_3 = this.pools[_local_2];
		_local_1 = (_local_1 + (((_local_2 + ": ") + _local_3.getNumObjects()) + "\n"));
	}
	return (_local_1);
	*/

	return QString();
}
