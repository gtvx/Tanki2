#ifndef TWINSSHOOTSFXMODELBASE_H
#define TWINSSHOOTSFXMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class TwinsShootSFXCC;

class TwinsShootSFXModelBase : public Model
{
public:
	TwinsShootSFXModelBase();

protected:
	TwinsShootSFXCC* getInitParam();
};

#endif // TWINSSHOOTSFXMODELBASE_H
