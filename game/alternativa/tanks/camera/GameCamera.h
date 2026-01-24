#ifndef GAMECAMERA_H
#define GAMECAMERA_H

#include "hardware/alternativa/engine3d/core/Camera3D.h"
#include "game/alternativa/math/Vector3.h"

class GameCamera : public Camera3D
{
public:

	Vector3 position;
	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;

	GameCamera();

	void setPosition(const Vector3*);
	void setRotation(const Vector3*);
	void readPosition(Vector3*) const;
	void readRotation(Vector3*) const;
	void calculateAdditionalData();
};

std::shared_ptr<GameCamera> new_GameCamera();

#endif // GAMECAMERA_H
