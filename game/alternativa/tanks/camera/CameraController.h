#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

class GameCamera;

class CameraController
{
public:
	virtual void deactivate() = 0;
	virtual void activate(GameCamera*) = 0;
	virtual void update(GameCamera*, int, int) = 0;
};

#endif // CAMERACONTROLLER_H
