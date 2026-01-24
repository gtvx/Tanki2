#ifndef LOCALTANKINFOSERVICE_H
#define LOCALTANKINFOSERVICE_H

class IGameObject;
class Tank;

class LocalTankInfoService
{
public:
	virtual bool isLocalTankLoaded() = 0;
	virtual IGameObject* getLocalTankObject() = 0;
	virtual IGameObject* getLocalTankObjectOrNull() = 0;
	virtual Tank* getLocalTank() = 0;
};

#endif // LOCALTANKINFOSERVICE_H
