#ifndef TANKBODYIDPROVIDER_H
#define TANKBODYIDPROVIDER_H

#include <QVector>

class BitVector;

class TankBodyIdProvider
{
	QVector<int> ids;
	BitVector *claimedIds;
	int firstFreeIdIndex;

public:

	static int claimId();
	static void releaseId(int id);
	static void resetIds();

	TankBodyIdProvider();

	void reset();
	int claim();
	void release(int id);
};

#endif // TANKBODYIDPROVIDER_H
