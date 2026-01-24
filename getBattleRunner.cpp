#include "getBattleRunner.h"

static BattleRunner *battleRunner;

BattleRunner *getBattleRunner()
{
	return battleRunner;
}

void setBattleRunner(BattleRunner *battleRunner)
{
	::battleRunner = battleRunner;
}
