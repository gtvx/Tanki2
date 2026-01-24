#ifndef BATTLEMAPMODEL_H
#define BATTLEMAPMODEL_H

#include "IBattleMap.h"

//BattleMapModelBase implements IBattleMapModelBase

class BattleMapModel : public IBattleMap
{
public:

	MapResource *mapResource;

	BattleMapModel();

	MapResource* getMapResource() override;

#if 0
	public function getEnvironmentSound():SoundResource
	{
		return (getInitParam().environmentSound);
	}

	public function getSkybox():SkyboxSides
	{
		return (getInitParam().skyboxSides);
	}

	public function getGravity():Number
	{
		return (getInitParam().gravity);
	}

	public function getSkyboxRevolutionAxis():Vector3d
	{
		return (getInitParam().skyBoxRevolutionAxis);
	}

	public function getSkyBoxRevolutionSpeed():Number
	{
		return (getInitParam().skyBoxRevolutionSpeed);
	}

	public function getFogParams():FogParams
	{
		return (getInitParam().fogParams);
	}

	public function getDynamicShadowParams():DynamicShadowParams
	{
		return (getInitParam().dynamicShadowParams);
	}

	public function getDustParams():DustParams
	{
		return (getInitParam().dustParams);
	}

	public function getSSAOColor():uint
	{
		return (getInitParam().ssaoColor);
	}
#endif
};

#endif // BATTLEMAPMODEL_H
