#ifndef IWEAPONMODELADAPT_H
#define IWEAPONMODELADAPT_H

#include "IWeaponModel.h"
#include <memory>

class IWeaponModelAdapt : public IWeaponModel
{
	IGameObject *object;
	std::shared_ptr<IWeaponModel> s_impl;
	IWeaponModel *impl;

public:
	IWeaponModelAdapt(IGameObject *object, std::shared_ptr<IWeaponModel> impl);

private:
	Weapon* createLocalWeapon(IGameObject *user) override;
	Weapon* createRemoteWeapon(IGameObject *user) override;
};

#endif // IWEAPONMODELADAPT_H
