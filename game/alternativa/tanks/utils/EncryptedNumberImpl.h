#ifndef ENCRYPTEDNUMBERIMPL_H
#define ENCRYPTEDNUMBERIMPL_H

#include "EncryptedNumber.h"

class EncryptedNumberImpl : public EncryptedNumber
{
	unsigned int v1, v2, mask;

public:
	EncryptedNumberImpl(double);

	void setNumber(double) override;
	double getNumber() const override;
};

#endif // ENCRYPTEDNUMBERIMPL_H
