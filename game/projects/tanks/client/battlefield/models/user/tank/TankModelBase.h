#ifndef TANKMODELBASE_H
#define TANKMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class TankCC;

class TankModelBase : public Model
{
public:
	TankModelBase();

protected:
	TankCC* getInitParam();
};

#endif // TANKMODELBASE_H
