#include "TankConfigurationModelBase.h"
#include "TankConfigurationCC.h"


TankConfigurationModelBase::TankConfigurationModelBase()
{

}


TankConfigurationCC* TankConfigurationModelBase::getInitParam()
{
	return initParams<TankConfigurationCC>(Model::object()).get();
}
