#include "PooledObject.h"
#include "Pool.h"


PooledObject::~PooledObject()
{

}


void PooledObject::recycle()
{
	this->pool->putObject(this);
}
