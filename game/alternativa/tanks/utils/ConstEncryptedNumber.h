#ifndef CONSTENCRYPTEDNUMBER_H
#define CONSTENCRYPTEDNUMBER_H

#include "EncryptedNumber.h"

class ConstEncryptedNumber : public EncryptedNumber
{
	double value;
	unsigned int v1, v2, mask;

public:
	ConstEncryptedNumber(double value);

	void setNumber(double) override;
	double getNumber() const override;
};

#endif // CONSTENCRYPTEDNUMBER_H
