#ifndef ITANKMODELADAPT_H
#define ITANKMODELADAPT_H

#include "ITankModel.h"
#include <memory>

class IGameObject;

class ITankModelAdapt : public ITankModel
{
	IGameObject *object;
	std::shared_ptr<ITankModel> impl;

public:
	ITankModelAdapt(IGameObject *object, std::shared_ptr<ITankModel> impl);

private:
	Tank* getTank() override;
	bool isLocal() override;
};

#endif // ITANKMODELADAPT_H
