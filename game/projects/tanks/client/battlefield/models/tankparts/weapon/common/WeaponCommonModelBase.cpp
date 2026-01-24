#include "WeaponCommonModelBase.h"


WeaponCommonModelBase::WeaponCommonModelBase()
{

}


WeaponCommonCC* WeaponCommonModelBase::getInitParam()
{
	return initParams<WeaponCommonCC>(Model::object()).get();
}
