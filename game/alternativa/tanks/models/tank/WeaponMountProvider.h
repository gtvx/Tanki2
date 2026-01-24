#ifndef WEAPONMOUNTPROVIDER_H
#define WEAPONMOUNTPROVIDER_H

#include <memory>

class WeaponMount;
class IGameObject;

class WeaponMountProvider
{
public:
	virtual std::shared_ptr<WeaponMount> createWeaponMount(IGameObject*) = 0;
};

#endif // WEAPONMOUNTPROVIDER_H
