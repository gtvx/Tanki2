#include "HullCommonModelBase.h"


HullCommonModelBase::HullCommonModelBase()
{

}


HullCommonCC* HullCommonModelBase::getInitParam()
{
	return initParams<HullCommonCC>(Model::object()).get();
}
