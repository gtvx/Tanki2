#ifndef SPEEDCHARACTERISTICSMODELBASE_H
#define SPEEDCHARACTERISTICSMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class SpeedCharacteristicsCC;

class SpeedCharacteristicsModelBase : public Model
{
public:
	SpeedCharacteristicsModelBase();

protected:
	SpeedCharacteristicsCC* getInitParam();
};

#endif // SPEEDCHARACTERISTICSMODELBASE_H
