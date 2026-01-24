#include "TwinsShootSFXModelBase.h"


TwinsShootSFXModelBase::TwinsShootSFXModelBase()
{

}


TwinsShootSFXCC* TwinsShootSFXModelBase::getInitParam()
{
	return initParams<TwinsShootSFXCC>(Model::object()).get();
}
