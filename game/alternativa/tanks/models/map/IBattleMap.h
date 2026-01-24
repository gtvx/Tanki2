#ifndef IBATTLEMAP_H
#define IBATTLEMAP_H

class MapResource;

class IBattleMap
{
public:
	virtual MapResource* getMapResource() = 0;
	//SoundResource* getEnvironmentSound();
	//SkyboxSides* getSkybox();
	//double getGravity();
	//Vector3d* getSkyboxRevolutionAxis();
	//double getSkyBoxRevolutionSpeed();
	//FogParams getFogParams();
	//DynamicShadowParams getDynamicShadowParams();
	//DustParams getDustParams();
	//uint getSSAOColor();
};

#endif // IBATTLEMAP_H
