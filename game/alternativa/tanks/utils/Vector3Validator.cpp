#include "Vector3Validator.h"
#include "EncryptedNumberImpl.h"


Vector3Validator::Vector3Validator(const Vector3 *vector) :
	vector(vector)
{
	this->x = new EncryptedNumberImpl(vector->getX());
	this->y = new EncryptedNumberImpl(vector->getY());
	this->z = new EncryptedNumberImpl(vector->getZ());
}


Vector3Validator::~Vector3Validator()
{
	delete this->x;
	delete this->y;
	delete this->z;
}


bool Vector3Validator::isInvalid()
{
	return this->vector->getX() != this->x->getNumber() ||
			this->vector->getY() != this->y->getNumber() ||
			this->vector->getZ() != this->z->getNumber();
}


bool Vector3Validator::isValid()
{
   return !this->isInvalid();
}
