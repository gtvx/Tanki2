#ifndef SHELLSTATES_H
#define SHELLSTATES_H

#include "game/alternativa/tanks/utils/objectpool/PooledObject.h"
#include <QVector>

class Vector3;
class ShellState;
class BattleService;


class ShellStates : public PooledObject
{
	Vector3 *lastControlPosition;
	Vector3 *lastControlDirection;

	Vector3 *prevPosition;
	Vector3 *prevDirection;

	int prevStateTime;
	int lastControlTime;
	int startTime;
	bool prevPositionAlreadyAdded;

	QVector<ShellState*> states;

public:

	static BattleService *battleService;

	ShellStates(Pool *pool);
	Vector3* getLastControlPosition();
	Vector3* getLastControlDirection();
	int getLastControlTime();
	int getTimeSinceLastControlState();
	virtual void start(const Vector3 *_arg_1, const Vector3 *_arg_2);
	virtual void updateState(const Vector3 *_arg_1, const Vector3 *_arg_2, int _arg_3 = 666666);
	QVector<ShellState*> getControlStates();
	virtual void savePrevPosition();
	virtual void destroy();
};

#endif // SHELLSTATES_H
