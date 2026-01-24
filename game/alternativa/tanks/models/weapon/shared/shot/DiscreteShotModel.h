#ifndef DISCRETESHOTMODEL_H
#define DISCRETESHOTMODEL_H

#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/shot/DiscreteShotModelBase.h"
#include "IShotModel.h"

//extends DiscreteShotModelBase implements IDiscreteShotModelBase, IShotModel, ObjectLoadListener

class DiscreteShotModel : public DiscreteShotModelBase, public IShotModel
{
public:
	DiscreteShotModel();

	void objectLoaded();

	int getReloadMS() override;
	void reconfigureWeapon(int value);
};

#endif // DISCRETESHOTMODEL_H
