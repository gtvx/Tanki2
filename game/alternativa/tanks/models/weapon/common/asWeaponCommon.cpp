#include "asWeaponCommon.h"
#include "IWeaponCommonModel.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"


IWeaponCommonModel* asWeaponCommon(IGameObject *object)
{
	return object->adapt<IWeaponCommonModel>().get();
}
