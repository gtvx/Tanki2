#ifndef TANKCC_H
#define TANKCC_H

class TankLogicState;
class TankState;
class BattleTeam;

class TankCC
{
public:
	int health;
	bool local;
	TankLogicState const* logicState;
	int movementDistanceBorderUntilTankCorrection;
	int movementTimeoutUntilTankCorrection;
	TankState *tankState;
	BattleTeam const* team;
};

#endif // TANKCC_H
