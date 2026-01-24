#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <stdint.h>

class BitVector
{
	uint32_t *elements;
	uint32_t count;

public:
	BitVector(uint32_t);
	~BitVector();
	void clear();
	void setBit(uint32_t);
	void clearBit(uint32_t);
	uint32_t getBit(uint32_t);
};

#endif // BITVECTOR_H
