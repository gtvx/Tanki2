#ifndef STREAM_READ_H
#define STREAM_READ_H

#include <vector>
#include <string>
#include <stdint.h>

struct stream_read
{
	uint8_t const * data, *end;
};


void stream_read_init(stream_read *t, uint8_t const * data, int size);

static inline int stream_read_available(const stream_read *t)
{
	return t->end - t->data;
}

void stream_read_bytes(struct stream_read *t, uint8_t *data, int length);

uint8_t stream_read_uint8(stream_read *t);

uint16_t stream_read_uint16_le(stream_read *t);
uint16_t stream_read_uint16_be(stream_read *t);

uint32_t stream_read_uint32_le(stream_read *t);
uint32_t stream_read_uint32_be(stream_read *t);

float stream_read_float_le(stream_read *t);
float stream_read_float_be(stream_read *t);

std::string stream_read_string(stream_read *t, int length);

#endif // STREAM_READ_H
