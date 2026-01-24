#include "EncryptedNumberImpl.h"
#include <stdint.h>

namespace
{
	union U
	{
		struct { uint32_t v1, v2; };
		double value;
	};
}


EncryptedNumberImpl::EncryptedNumberImpl(double value)
{
	this->mask = 64251551;
	this->setNumber(value);
}


void EncryptedNumberImpl::setNumber(double value)
{
	U u;
	u.value = value;
	this->v1 = u.v1 ^ mask;
	this->v2 = u.v2 ^ mask;
}


double EncryptedNumberImpl::getNumber() const
{
	U u;
	u.v1 = this->v1 ^ mask;
	u.v2 = this->v2 ^ mask;
	return u.value;
}
