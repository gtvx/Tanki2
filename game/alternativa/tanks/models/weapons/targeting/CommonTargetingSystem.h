#ifndef COMMONTARGETINGSYSTEM_H
#define COMMONTARGETINGSYSTEM_H

#include "TargetingSystem.h"

class IGameObject;
class WeaponObject;

class CommonTargetingSystem : public TargetingSystem
{
public:
	CommonTargetingSystem(IGameObject *user, WeaponObject *weaponObject, double maxDistance);
};

#endif // COMMONTARGETINGSYSTEM_H
