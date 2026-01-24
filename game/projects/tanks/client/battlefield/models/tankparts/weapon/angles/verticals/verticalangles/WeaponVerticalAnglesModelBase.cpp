#include "WeaponVerticalAnglesModelBase.h"
#include "VerticalAnglesCC.h"


WeaponVerticalAnglesModelBase::WeaponVerticalAnglesModelBase()
{

}


VerticalAnglesCC* WeaponVerticalAnglesModelBase::getInitParam()
{
	return initParams<VerticalAnglesCC>(Model::object()).get();
}
