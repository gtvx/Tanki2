#include "BitVector.h"
#include "BitVector.h"


BitVector::BitVector(uint32_t p)
{
	count = p >> 5;
	this->elements = new uint32_t[count];
}


BitVector::~BitVector()
{
	delete this->elements;
}


void BitVector::setBit(uint32_t p)
{
	uint32_t v = p >> 5;
	this->elements[v] = this->elements[v] | (1 << (p & 0x1F));
}


void BitVector::clearBit(uint32_t p)
{
	uint32_t v = p >> 5;
	this->elements[v] = this->elements[v] & (~(1 << (p & 0x1F)));
}


uint32_t BitVector::getBit(uint32_t p)
{
	uint32_t v = p >> 5;
	return (this->elements[v] >> (p & 0x1F)) & 1;
}


void BitVector::clear()
{
	for (uint32_t i = 0; i < this->count; i++)
		this->elements[i] = 0;
}
