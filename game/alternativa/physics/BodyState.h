#ifndef BODYSTATE_H
#define BODYSTATE_H

#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/math/Quaternion.h"

class BodyState
{
public:
	BodyState();

	Vector3 velocity; //Скорость
	Vector3 angularVelocity; //Угловая скорость
	Quaternion orientation; //Ориентация
	Vector3 position; //Позиция

	void copy(const BodyState*);
	bool isValid() const;
};

#endif // BODYSTATE_H
