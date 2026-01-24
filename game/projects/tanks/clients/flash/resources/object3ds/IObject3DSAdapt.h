#ifndef IOBJECT3DSADAPT_H
#define IOBJECT3DSADAPT_H

#include "IObject3DS.h"
#include <memory>

class IGameObject;

class IObject3DSAdapt : public IObject3DS
{
	IGameObject *object;
	std::shared_ptr<IObject3DS> impl;

public:
	IObject3DSAdapt(IGameObject *object, std::shared_ptr<IObject3DS> impl);

	Tanks3DSResource* getResource3DS() override;
};

#endif // IOBJECT3DSADAPT_H
