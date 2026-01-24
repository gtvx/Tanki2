#ifndef OBJECT3DSCC_H
#define OBJECT3DSCC_H

#include "entrance/alternativa/types/Long.h"

class Object3DSCC
{
public:
	Long resourceId;

	Object3DSCC(const Long &resourceId)
	{
		this->resourceId = resourceId;
	}
};

#endif // OBJECT3DSCC_H
