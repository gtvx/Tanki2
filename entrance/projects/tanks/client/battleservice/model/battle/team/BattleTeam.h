#ifndef BATTLETEAM_H
#define BATTLETEAM_H

class BattleTeam
{
	int _value;
	const char* _name;

public:
	BattleTeam(int value, const char *name);
	static const BattleTeam *RED;
	static const BattleTeam *BLUE;
	static const BattleTeam *NONE;
};

#endif // BATTLETEAM_H
