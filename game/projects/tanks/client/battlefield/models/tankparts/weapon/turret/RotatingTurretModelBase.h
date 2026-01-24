#ifndef ROTATINGTURRETMODELBASE_H
#define ROTATINGTURRETMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class RotatingTurretCC;

class RotatingTurretModelBase : public Model
{
public:
	RotatingTurretModelBase();

protected:
	RotatingTurretCC* getInitParam();
};

#endif // ROTATINGTURRETMODELBASE_H
