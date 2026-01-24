#ifndef WEAPONWEAKENINGMODELBASE_H
#define WEAPONWEAKENINGMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class WeaponWeakeningCC;

class WeaponWeakeningModelBase : public Model
{
public:
	WeaponWeakeningModelBase();

protected:
	WeaponWeakeningCC* getInitParam();
};

#endif // WEAPONWEAKENINGMODELBASE_H
