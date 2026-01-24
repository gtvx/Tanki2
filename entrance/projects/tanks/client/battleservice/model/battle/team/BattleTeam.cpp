#include "BattleTeam.h"

const BattleTeam *BattleTeam::RED = new BattleTeam(0, "RED");
const BattleTeam *BattleTeam::BLUE = new BattleTeam(1 ,"BLUE");
const BattleTeam *BattleTeam::NONE = new BattleTeam(2, "NONE");

BattleTeam::BattleTeam(int value, const char *name)
{
	this->_value = value;
	this->_name = name;
}
