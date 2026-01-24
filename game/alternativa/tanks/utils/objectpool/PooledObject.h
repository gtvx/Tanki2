#ifndef POOLEDOBJECT_H
#define POOLEDOBJECT_H

class Pool;

class PooledObject
{
	Pool *pool;

public:
	PooledObject(Pool *pool)
	{
		this->pool = pool;
	}

	virtual ~PooledObject();

	void recycle();

};

#endif // POOLEDOBJECT_H
