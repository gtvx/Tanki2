#ifndef HULLCOMMONADAPT_H
#define HULLCOMMONADAPT_H

#include "HullCommon.h"
#include <memory>

class IGameObject;

class HullCommonAdapt : public HullCommon
{
	IGameObject *object;
	std::shared_ptr<HullCommon> impl;

public:
	HullCommonAdapt(IGameObject *object, std::shared_ptr<HullCommon> impl);

	double getMass() override;
};

#endif // HULLCOMMONADAPT_H
