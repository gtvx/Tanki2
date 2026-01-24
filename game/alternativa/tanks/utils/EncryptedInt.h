#ifndef ENCRYPTEDINT_H
#define ENCRYPTEDINT_H

class EncryptedInt
{
public:
	virtual ~EncryptedInt() {}

	virtual void setInt(int) = 0;
	virtual int getInt() const = 0;
};

#endif // ENCRYPTEDINT_H
