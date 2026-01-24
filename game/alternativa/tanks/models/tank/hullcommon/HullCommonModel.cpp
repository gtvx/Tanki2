#include "HullCommonModel.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/armor/common/HullCommonCC.h"



HullCommonModel::HullCommonModel()
{

}

#if 0
public function getDeadColoring():TextureResource
{
	return (getInitParam().deadColoring);
}

public function getCC():HullCommonCC
{
	return (getInitParam());
}
#endif

double HullCommonModel::getMass()
{
	return getInitParam()->mass;
}

#if 0
public function setTankObject(_arg_1:IGameObject):void
{
	putData(IGameObject, _arg_1);
}

public function getTankObject():IGameObject
{
	return (IGameObject(getData(IGameObject)));
}

public function getStunEffectTexture():BitmapData
{
	return (getInitParam().stunEffectTexture.data);
}

public function getStunSound():Sound
{
	return (getInitParam().stunSound.sound);
}

public function getUltimateIconIndex():int
{
	return (getInitParam().ultimateIconIndex);
}
#endif
