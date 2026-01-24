#ifndef ITWINSSFXMODELADAPT_H
#define ITWINSSFXMODELADAPT_H

#include "ITwinsSFXModel.h"
#include <memory>

class IGameObject;

class ITwinsSFXModelAdapt : public ITwinsSFXModel
{
	IGameObject *object;
	std::shared_ptr<ITwinsSFXModel> impl;

public:
	ITwinsSFXModelAdapt(IGameObject *object, std::shared_ptr<ITwinsSFXModel> impl);

private:
	TwinsEffects* getPlasmaWeaponEffects() override;
	TwinsSFXData* getSFXData() override;
};

#endif // ITWINSSFXMODELADAPT_H
