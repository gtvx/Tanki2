#ifndef DISCRETESHOTMODELBASE_H
#define DISCRETESHOTMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class ShotCC;

class DiscreteShotModelBase : public Model
{
public:
	DiscreteShotModelBase();

protected:
	ShotCC* getInitParam();
};

#endif // DISCRETESHOTMODELBASE_H
