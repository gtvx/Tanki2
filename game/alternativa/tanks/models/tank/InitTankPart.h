#ifndef INITTANKPART_H
#define INITTANKPART_H

#include <memory>

class Tank;

class InitTankPart
{
public:
	virtual void initTankPart(std::shared_ptr<Tank>) = 0;
};

#endif // INITTANKPART_H
