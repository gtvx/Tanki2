#ifndef DUMMYCAMERACONTROLLER_H
#define DUMMYCAMERACONTROLLER_H

#include "CameraController.h"

class DummyCameraController : public CameraController
{
public:
	static DummyCameraController *INSTANCE;

	virtual void deactivate();
	virtual void activate(GameCamera*);
	virtual void update(GameCamera*, int, int);
};

#endif // DUMMYCAMERACONTROLLER_H
