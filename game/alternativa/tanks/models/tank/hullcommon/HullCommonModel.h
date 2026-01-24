#ifndef HULLCOMMONMODEL_H
#define HULLCOMMONMODEL_H

#include "game/projects/tanks/client/battlefield/models/tankparts/armor/common/HullCommonModelBase.h"
#include "HullCommon.h"

// extends HullCommonModelBase implements IHullCommonModelBase, HullCommon

class HullCommonModel : public HullCommonModelBase, public HullCommon
{
public:
	HullCommonModel();

private:
	double getMass() override;
};

#endif // HULLCOMMONMODEL_H
