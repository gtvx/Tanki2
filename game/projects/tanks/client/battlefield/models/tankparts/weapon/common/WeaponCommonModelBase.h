#ifndef WEAPONCOMMONMODELBASE_H
#define WEAPONCOMMONMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class WeaponCommonCC;

class WeaponCommonModelBase : public Model
{
public:
	WeaponCommonModelBase();

protected:
	WeaponCommonCC* getInitParam();
};

#endif // WEAPONCOMMONMODELBASE_H
