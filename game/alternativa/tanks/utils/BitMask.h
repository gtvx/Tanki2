#ifndef BITMASK_H
#define BITMASK_H

#include <stdint.h>

class BitMask
{
	uint32_t mask;
public:
	BitMask(uint32_t = 0);
	uint32_t getMask();
	void setBits(uint32_t value);
	void clearBits(uint32_t value);
	void setBit(uint32_t value);
	void clearBit(uint32_t value);
	uint32_t getBitValue(uint32_t value);
	void clear();
	bool isEmpty();
	bool isNotEmpty();
	bool hasAnyBit(uint32_t value);
	void change(uint32_t value1, uint32_t value2);
};

#endif // BITMASK_H
