#include "global_class.h"

static TankUsersRegistry *tankUsersRegistry;
static BattlefieldModel *battlefieldModel;


void setTankUsersRegistry(TankUsersRegistry *tankUsersRegistry)
{
	::tankUsersRegistry = tankUsersRegistry;
}

TankUsersRegistry* getTankUsersRegistry()
{
	return ::tankUsersRegistry;
}



void setBattlefieldModel(BattlefieldModel *battlefieldModel)
{
	::battlefieldModel = battlefieldModel;
}

BattlefieldModel* getBattlefieldModel()
{
	return ::battlefieldModel;
}



