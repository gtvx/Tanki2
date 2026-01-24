#include "TankModelBase.h"
#include "Error.h"


TankModelBase::TankModelBase()
{

}


TankCC* TankModelBase::getInitParam()
{
	TankCC *result = initParams<TankCC>(Model::object()).get();

	if (result == nullptr)
	{
		throw Error("TankModelBase::getInitParam");
	}

	return result;
}
