#ifndef WEAPONVERTICALANGLESMODELBASE_H
#define WEAPONVERTICALANGLESMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class VerticalAnglesCC;

class WeaponVerticalAnglesModelBase : public Model
{
public:
	WeaponVerticalAnglesModelBase();

protected:
	VerticalAnglesCC* getInitParam();
};

#endif // WEAPONVERTICALANGLESMODELBASE_H
