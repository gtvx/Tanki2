#include "EncryptedIntImpl.h"

EncryptedIntImpl::EncryptedIntImpl(int value)
{
	this->mask = 905943434;
	this->setInt(value);
}


EncryptedIntImpl::~EncryptedIntImpl()
{

}


void EncryptedIntImpl::setInt(int value)
{
	this->encrypted_value = value ^ mask;
}


int EncryptedIntImpl::getInt() const
{
	return this->encrypted_value ^ mask;
}
