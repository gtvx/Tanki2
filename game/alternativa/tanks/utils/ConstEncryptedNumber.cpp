#include "ConstEncryptedNumber.h"
#include <stdint.h>

namespace
{
	union U
	{
		struct { uint32_t v1, v2; };
		double value;
	};
}


ConstEncryptedNumber::ConstEncryptedNumber(double value)
{
	this->mask = 56765183;
	U u;
	u.value = value;
	this->v1 = u.v1 ^ mask;
	this->v2 = u.v2 ^ mask;
}


void ConstEncryptedNumber::setNumber(double)
{

}


double ConstEncryptedNumber::getNumber() const
{
	U u;
	u.v1 = this->v1 ^ mask;
	u.v2 = this->v2 ^ mask;
	return u.value;
}
