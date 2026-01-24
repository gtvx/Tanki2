#include "DiscreteShotModelBase.h"


DiscreteShotModelBase::DiscreteShotModelBase()
{

}


ShotCC* DiscreteShotModelBase::getInitParam()
{
	return initParams<ShotCC>(Model::object()).get();
}
