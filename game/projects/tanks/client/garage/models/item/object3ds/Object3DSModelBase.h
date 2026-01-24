#ifndef OBJECT3DSMODELBASE_H
#define OBJECT3DSMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class Object3DSCC;

class Object3DSModelBase : public Model
{
public:
	Object3DSModelBase();

protected:
	Object3DSCC* getInitParam();
};

#endif // OBJECT3DSMODELBASE_H
