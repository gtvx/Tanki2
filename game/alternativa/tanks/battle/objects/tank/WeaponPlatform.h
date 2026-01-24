#ifndef WEAPONPLATFORM_H
#define WEAPONPLATFORM_H

#include <memory>

class AllGlobalGunParams;
class BasicGlobalGunParams;
class Body;
class Vector3;
class Object3D;
class TankSkin;
class BattleTeam;
class WeaponMount;

class WeaponPlatform
{
public:
	virtual void getAllGunParams(AllGlobalGunParams*, int=0) = 0;
	virtual void getBasicGunParams(BasicGlobalGunParams*, int=0) = 0;
	virtual Body* getBody() = 0;
	virtual Vector3* getLocalMuzzlePosition(int=0) = 0;
	virtual Vector3* getLaserLocalPosition() = 0;
	virtual double getBarrelLength(int=0) = 0;
	virtual std::shared_ptr<Object3D> getTurret3D() = 0;
	virtual void stopMovement() = 0;
	virtual void lockMovement(bool) = 0;
	virtual int getNumberOfBarrels() = 0;
	virtual void enableTurretSound(bool) = 0;
	virtual TankSkin* getSkin() = 0;
	virtual void showTitle() = 0;
	virtual void addDust(int=7) = 0;
	virtual BattleTeam* get_teamType() = 0;
	virtual WeaponMount* getWeaponMount() = 0;
};

#endif // WEAPONPLATFORM_H
