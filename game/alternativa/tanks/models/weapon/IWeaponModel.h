#ifndef IWEAPONMODEL_H
#define IWEAPONMODEL_H

class Weapon;
class IGameObject;

class IWeaponModel
{
public:
	virtual Weapon* createLocalWeapon(IGameObject*) = 0;
	virtual Weapon* createRemoteWeapon(IGameObject*) = 0;
};

#endif // IWEAPONMODEL_H
