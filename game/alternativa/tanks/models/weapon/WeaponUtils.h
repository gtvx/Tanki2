#ifndef WEAPONUTILS_H
#define WEAPONUTILS_H

class Object3D;
class Vector3;
class AllGlobalGunParams;

class WeaponUtils
{
public:
	static void calculateMainGunParams(const Object3D*, const Vector3*, AllGlobalGunParams*);
};

#endif // WEAPONUTILS_H
