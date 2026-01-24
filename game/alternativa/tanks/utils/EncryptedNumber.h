#ifndef ENCRYPTEDNUMBER_H
#define ENCRYPTEDNUMBER_H

class EncryptedNumber
{
public:
	virtual ~EncryptedNumber() {}

	virtual void setNumber(double) = 0;
	virtual double getNumber() const = 0;
};

#endif // ENCRYPTEDNUMBER_H
