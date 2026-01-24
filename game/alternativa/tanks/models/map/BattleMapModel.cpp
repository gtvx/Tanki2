#include "BattleMapModel.h"
#include "game/projects/tanks/clients/flash/resources/resource/MapResource.h"


BattleMapModel::BattleMapModel()
{
	mapResource = nullptr;
}


MapResource* BattleMapModel::getMapResource()
{
	//return (getInitParam().mapResource);
	return mapResource;
}
