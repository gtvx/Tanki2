#include "WeaponWeakeningModelBase.h"


WeaponWeakeningModelBase::WeaponWeakeningModelBase()
{

}


WeaponWeakeningCC* WeaponWeakeningModelBase::getInitParam()
{
	return initParams<WeaponWeakeningCC>(Model::object()).get();
}
