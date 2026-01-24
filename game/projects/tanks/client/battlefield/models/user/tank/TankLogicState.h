#ifndef TANKLOGICSTATE_H
#define TANKLOGICSTATE_H


class TankLogicState
{
	const char *_name;
	int _value;

public:
	TankLogicState(int, const char*);
	static const TankLogicState * const NEW;
	static const TankLogicState * const OUT_OF_GAME;
	static const TankLogicState * const ACTIVATING;
	static const TankLogicState * const ACTIVE;
	static const TankLogicState * const DEAD;
};

#endif // TANKLOGICSTATE_H
