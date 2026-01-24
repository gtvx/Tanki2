#ifndef IWEAPONCOMMONMODELADAPT_H
#define IWEAPONCOMMONMODELADAPT_H

#include "IWeaponCommonModel.h"

class IGameObject;

class IWeaponCommonModelAdapt : public IWeaponCommonModel
{
	IGameObject *object;
	std::shared_ptr<IWeaponCommonModel> impl;

public:
	IWeaponCommonModelAdapt(IGameObject *object, std::shared_ptr<IWeaponCommonModel> impl);

private:
	std::shared_ptr<WeaponCommonData> getCommonData() override;
	void storeTank(std::shared_ptr<Tank> tank) override;
	Tank* getTank() override;
	void getGunParams(AllGlobalGunParams *allGunParams, int barrelIndex = 0) override;
};

#endif // IWEAPONCOMMONMODELADAPT_H
