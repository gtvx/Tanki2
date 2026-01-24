#include "Object3DSModelBase.h"


Object3DSModelBase::Object3DSModelBase()
{

}


Object3DSCC* Object3DSModelBase::getInitParam()
{
	return initParams<Object3DSCC>(Model::object()).get();
}
