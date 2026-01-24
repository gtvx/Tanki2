#include "stream_read.h"
#include "s_endian.h"
#include <string.h>

void stream_read_init(struct stream_read *t, uint8_t const * data, int size)
{
	t->data = (const uint8_t*)data;
	t->end = t->data + size;
}

void stream_read_bytes(struct stream_read *t, uint8_t *data, int length)
{
	memcpy(data, t->data, length);
	t->data += length;
}

uint8_t stream_read_uint8(struct stream_read *t)
{
	uint8_t value = *(uint8_t*)t->data;
	t->data += sizeof(uint8_t);
	return value;
}

uint16_t stream_read_uint16_le(struct stream_read *t)
{
	uint16_t value = uint16_little_endian_to_local(*(uint16_t*)t->data);
	t->data += sizeof(uint16_t);
	return value;
}

uint16_t stream_read_uint16_be(struct stream_read *t)
{
	uint16_t value = uint16_big_endian_to_local(*(uint16_t*)t->data);
	t->data += sizeof(uint16_t);
	return value;
}

uint32_t stream_read_uint32_le(struct stream_read *t)
{
	uint32_t value = uint32_little_endian_to_local(*(uint32_t*)t->data);
	t->data += sizeof(uint32_t);
	return value;
}

uint32_t stream_read_uint32_be(struct stream_read *t)
{
	uint32_t value = uint32_big_endian_to_local(*(uint32_t*)t->data);
	t->data += sizeof(uint32_t);
	return value;
}

float stream_read_float_le(stream_read *t)
{
	union { uint32_t i; float f; } u;
	u.i = uint32_little_endian_to_local(*(uint32_t*)t->data);
	t->data += sizeof(float);
	return u.f;
}

float stream_read_float_be(stream_read *t)
{
	union { uint32_t i; float f; } u;
	u.i = uint32_big_endian_to_local(*(uint32_t*)t->data);
	t->data += sizeof(float);
	return u.f;
}

std::string stream_read_string(struct stream_read *t, int length)
{
	std::string s((const char*)t->data, length);
	t->data += length;
	return s;
}
