#ifndef WEAPONCOMMONMODEL_H
#define WEAPONCOMMONMODEL_H

#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/common/WeaponCommonModelBase.h"
#include "IWeaponCommonModel.h"

//extends WeaponCommonModelBase implements IWeaponCommonModelBase, IWeaponCommonModel, WeaponSound, ObjectLoadListener, ObjectUnloadListener

class WeaponCommonModel : public WeaponCommonModelBase, public IWeaponCommonModel
{
public:
	WeaponCommonModel();

	void objectLoaded();

	void storeTank(std::shared_ptr<Tank> tank) override;
	Tank* getTank() override;
	void getGunParams(AllGlobalGunParams *allGunParams, int index = 0) override;
	void setBuffed(bool _arg_1, double _arg_2);
	std::shared_ptr<WeaponCommonData> getCommonData() override;

};

#endif // WEAPONCOMMONMODEL_H
