#include "TankLogicState.h"

const TankLogicState * const TankLogicState::NEW = new TankLogicState(0, "NEW");
const TankLogicState * const TankLogicState::OUT_OF_GAME = new TankLogicState(1, "OUT_OF_GAME");
const TankLogicState * const TankLogicState::ACTIVATING = new TankLogicState(2, "ACTIVATING");
const TankLogicState * const TankLogicState::ACTIVE = new TankLogicState(3, "ACTIVE");
const TankLogicState * const TankLogicState::DEAD = new TankLogicState(4, "DEAD");


TankLogicState::TankLogicState(int value, const char *name)
{
	this->_value = value;
	this->_name = name;
}
