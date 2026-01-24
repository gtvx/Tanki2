#include "CommonTargetingSystem.h"
#include "game/alternativa/tanks/models/weapons/targeting/priority/TargetingPriorityCalculator.h"
#include "game/alternativa/tanks/models/weapons/targeting/priority/targeting/CommonTargetPriorityCalculator.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/sector/SectorDirectionCalculator.h"
#include "game/alternativa/tanks/models/weapons/targeting/processor/SingleTargetDirectionProcessor.h"


CommonTargetingSystem::CommonTargetingSystem(IGameObject *user, WeaponObject *weaponObject, double maxDistance)
{
	CommonTargetPriorityCalculator *a = new CommonTargetPriorityCalculator(weaponObject);
	TargetingPriorityCalculator *calculator = new TargetingPriorityCalculator(a);

	init(new SectorDirectionCalculator(user, weaponObject, maxDistance, calculator),
		 new SingleTargetDirectionProcessor(user, maxDistance), calculator);
}
