#include "TankBodyIdProvider.h"
#include "game/alternativa/tanks/utils/BitVector.h"
#include "Error.h"

static TankBodyIdProvider instance;
static const int MAX_IDS = 64;


TankBodyIdProvider::TankBodyIdProvider()
{
	ids.resize(MAX_IDS);
	claimedIds = new BitVector(MAX_IDS);
	firstFreeIdIndex = 0;
}


void TankBodyIdProvider::resetIds()
{
	instance.reset();
}


void TankBodyIdProvider::reset()
{
	this->firstFreeIdIndex = 0;
	int index = 0;
	while (index < MAX_IDS)
	{
		this->ids.replace(index, index);
		index++;
	}
	this->claimedIds->clear();
}


int TankBodyIdProvider::claimId()
{
	return instance.claim();
}


int TankBodyIdProvider::claim()
{
	if (this->firstFreeIdIndex < MAX_IDS)
	{
		int i = this->ids.at(this->firstFreeIdIndex++);
		this->claimedIds->setBit(i);
		return i;
	}
	throw Error(2);
}


void TankBodyIdProvider::release(int id)
{
	if (this->claimedIds->getBit(id) == 1)
	{
		this->claimedIds->clearBit(id);
		int l = --this->firstFreeIdIndex;
		this->ids.replace(l, id);
	}
}


void TankBodyIdProvider::releaseId(int id)
{
	instance.release(id);
}
