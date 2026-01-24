#ifndef TANKCONFIGURATION_H
#define TANKCONFIGURATION_H

class IGameObject;

class TankConfiguration
{
public:
	virtual IGameObject* getHullObject() = 0;
	virtual IGameObject* getWeaponObject() = 0;
	virtual IGameObject* getColoringObject()= 0;
	//virtual bool hasDrone() = 0;
	//virtual IGameObject* getDrone() = 0;
};

#endif // TANKCONFIGURATION_H
