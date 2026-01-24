#ifndef IWEAPONWEAKENINGMODELADAPT_H
#define IWEAPONWEAKENINGMODELADAPT_H

#include "IWeaponWeakeningModel.h"
#include <memory>

class IGameObject;

class IWeaponWeakeningModelAdapt : public IWeaponWeakeningModel
{
	IGameObject *object;
	std::shared_ptr<IWeaponWeakeningModel> impl;

public:
	IWeaponWeakeningModelAdapt(IGameObject *object, std::shared_ptr<IWeaponWeakeningModel> impl);

private:
	void getDistanceWeakening(DistanceWeakening*) override;
};

#endif // IWEAPONWEAKENINGMODELADAPT_H
