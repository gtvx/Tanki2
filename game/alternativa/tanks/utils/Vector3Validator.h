#ifndef VECTOR3VALIDATOR_H
#define VECTOR3VALIDATOR_H

#include "game/alternativa/math/Vector3.h"

class EncryptedNumber;

class Vector3Validator
{
	const Vector3 *vector;
	EncryptedNumber *x;
	EncryptedNumber *y;
	EncryptedNumber *z;

public:
	Vector3Validator(const Vector3 *vector);
	~Vector3Validator();

	bool isInvalid();
	bool isValid();
};

#endif // VECTOR3VALIDATOR_H
