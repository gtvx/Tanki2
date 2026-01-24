#ifndef WEAPONMOUNTPROVIDERADAPT_H
#define WEAPONMOUNTPROVIDERADAPT_H

#include "WeaponMountProvider.h"
#include <memory>

class IGameObject;

class WeaponMountProviderAdapt : public WeaponMountProvider
{
	IGameObject *object;
	std::shared_ptr<WeaponMountProvider> impl;

public:
	WeaponMountProviderAdapt(IGameObject *object, std::shared_ptr<WeaponMountProvider> impl);

private:
	std::shared_ptr<WeaponMount> createWeaponMount(IGameObject *tankObject) override;
};

#endif // WEAPONMOUNTPROVIDERADAPT_H
