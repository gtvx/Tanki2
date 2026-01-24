#include "BitMask.h"


BitMask::BitMask(uint32_t mask)
{
	this->mask = mask;
}


uint32_t BitMask::getMask()
{
	return this->mask;
}


void BitMask::setBits(uint32_t value)
{
	this->mask = this->mask | value;
}


void BitMask::clearBits(uint32_t value)
{
	this->mask = this->mask & ~value;
}


void BitMask::setBit(uint32_t value)
{
	this->setBits(1 << value);
}


void BitMask::clearBit(uint32_t value)
{
	this->clearBits(1 << value);
}


uint32_t BitMask::getBitValue(uint32_t value)
{
	return 1 & this->mask >> value;
}


void BitMask::clear()
{
	this->mask = 0;
}


bool BitMask::isEmpty()
{
	return this->mask == 0;
}


bool BitMask::isNotEmpty()
{
	return this->mask != 0;
}


bool BitMask::hasAnyBit(uint32_t value)
{
	return (this->mask & value) != 0;
}


void BitMask::change(uint32_t value1, uint32_t value2)
{
	if (value2)
		this->setBits(value1);
	else
		this->clearBits(value1);
}
