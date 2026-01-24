#include "RotatingTurretModelBase.h"


RotatingTurretModelBase::RotatingTurretModelBase()
{

}


RotatingTurretCC* RotatingTurretModelBase::getInitParam()
{
	return initParams<RotatingTurretCC>(Model::object()).get();
}
