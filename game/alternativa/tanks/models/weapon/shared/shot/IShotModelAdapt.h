#ifndef ISHOTMODELADAPT_H
#define ISHOTMODELADAPT_H

#include "IShotModel.h"
#include <memory>

class IGameObject;

class IShotModelAdapt : public IShotModel
{
	IGameObject *object;
	std::shared_ptr<IShotModel> impl;

public:
	IShotModelAdapt(IGameObject *object, std::shared_ptr<IShotModel> impl);

private:
	int getReloadMS() override;
};

#endif // ISHOTMODELADAPT_H
