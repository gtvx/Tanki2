#ifndef TANKCONFIGURATIONMODELBASE_H
#define TANKCONFIGURATIONMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class TankConfigurationCC;

class TankConfigurationModelBase : public Model
{
public:
	TankConfigurationModelBase();

protected:
	TankConfigurationCC* getInitParam();
};

#endif // TANKCONFIGURATIONMODELBASE_H
