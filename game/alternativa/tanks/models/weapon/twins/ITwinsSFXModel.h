#ifndef ITWINSSFXMODEL_H
#define ITWINSSFXMODEL_H

class TwinsEffects;
class TwinsSFXData;

class ITwinsSFXModel
{
public:
	virtual TwinsEffects* getPlasmaWeaponEffects() = 0;
	virtual TwinsSFXData* getSFXData() = 0;
};

#endif // ITWINSSFXMODEL_H
