#ifndef INITTANKPARTADAPT_H
#define INITTANKPARTADAPT_H

#include "InitTankPart.h"

class IGameObject;

class InitTankPartAdapt : public InitTankPart
{
	IGameObject *object;
	std::shared_ptr<InitTankPart> impl;

public:
	InitTankPartAdapt(IGameObject *object, std::shared_ptr<InitTankPart> impl);

private:
	 void initTankPart(std::shared_ptr<Tank>) override;
};

#endif // INITTANKPARTADAPT_H
