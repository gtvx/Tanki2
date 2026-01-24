#ifndef ENCRYPTEDINTIMPL_H
#define ENCRYPTEDINTIMPL_H

#include "EncryptedInt.h"

class EncryptedIntImpl final : public EncryptedInt
{
	unsigned int mask;
	int encrypted_value;

public:
	EncryptedIntImpl(int value = 0);
	~EncryptedIntImpl();

	void setInt(int value) override;
	int getInt() const override;
};

#endif // ENCRYPTEDINTIMPL_H
