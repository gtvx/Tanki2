#ifndef HULLCOMMONMODELBASE_H
#define HULLCOMMONMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class HullCommonCC;

class HullCommonModelBase : public Model
{
public:
	HullCommonModelBase();

protected:
	HullCommonCC* getInitParam();
};

#endif // HULLCOMMONMODELBASE_H
