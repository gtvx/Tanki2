#ifndef PROPOBJECT_H
#define PROPOBJECT_H

#include <memory>
#include "PropObjectType.h"

class Object3D;

class PropObject
{
	PropObjectType _type;
public:

	std::shared_ptr<Object3D> object;

	PropObject(PropObjectType type)
	{
		this->_type = type;
	}

	PropObjectType type()
	{
		return this->_type;
	}
};

#endif // PROPOBJECT_H
