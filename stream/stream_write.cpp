#include "stream_write.h"
#include "s_endian.h"

void stream_write_data(struct stream_write *t, const char *d, int size)
{
	t->data.insert(t->data.end(), d, d + size);
}

void stream_write_uint8(struct stream_write *t, uint8_t value)
{
	t->data.push_back(value);
}

void stream_write_uint16_le(struct stream_write *t, uint16_t value)
{
	value = uint16_local_to_little_endian(value);
	stream_write_data(t, (const char*)&value, sizeof(uint16_t));
}

void stream_write_uint16_be(struct stream_write *t, uint16_t value)
{
	value = uint16_local_to_big_endian(value);
	stream_write_data(t, (const char*)&value, sizeof(uint16_t));
}

void stream_write_uint32_le(struct stream_write *t, uint32_t value)
{
	value = uint32_local_to_little_endian(value);
	stream_write_data(t, (const char*)&value, sizeof(uint32_t));
}

void stream_write_uint32_be(struct stream_write *t, uint32_t value)
{
	value = uint32_local_to_big_endian(value);
	stream_write_data(t, (const char*)&value, sizeof(uint32_t));
}

void stream_write_string(struct stream_write *t, const std::string &s)
{
	stream_write_data(t, s.c_str(), s.size());
}
