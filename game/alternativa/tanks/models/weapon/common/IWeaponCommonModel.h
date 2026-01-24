#ifndef IWEAPONCOMMONMODEL_H
#define IWEAPONCOMMONMODEL_H

#include <memory>

class WeaponCommonData;
class Tank;
class AllGlobalGunParams;

class IWeaponCommonModel
{
public:
	virtual std::shared_ptr<WeaponCommonData> getCommonData() = 0;
	virtual void storeTank(std::shared_ptr<Tank>) = 0;
	virtual Tank* getTank() = 0;
	virtual void getGunParams(AllGlobalGunParams*, int = 0) = 0;
};

#endif // IWEAPONCOMMONMODEL_H
