#include "DiscreteShotModel.h"
#include "game/alternativa/tanks/utils/EncryptedIntImpl.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/shot/ShotCC.h"


DiscreteShotModel::DiscreteShotModel()
{

}


void DiscreteShotModel::objectLoaded()
{
	ShotCC *cc = getInitParam();
	putData<EncryptedInt>(std::make_shared<EncryptedIntImpl>(cc->reloadMsec));
}


int DiscreteShotModel::getReloadMS()
{
	 return getData<EncryptedInt>()->getInt();
}


void DiscreteShotModel::reconfigureWeapon(int value)
{
	EncryptedInt *encrypted = getData<EncryptedInt>().get();
	int old_value = encrypted->getInt();
	encrypted->setInt(value);
	(void)old_value;
	// WeaponReloadTimeChangedListener(object.event(WeaponReloadTimeChangedListener)).weaponReloadTimeChanged(_local_3, _arg_1);
}
