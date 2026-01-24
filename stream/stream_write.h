#ifndef STREAM_WRITE_H
#define STREAM_WRITE_H

#include <vector>
#include <string>
#include <stdint.h>

struct stream_write
{
	std::vector<char> data;
};

void stream_write_data(struct stream_write *t, const char *d, int size);

void stream_write_uint8(struct stream_write *t, uint8_t value);

void stream_write_uint16_be(struct stream_write *t, uint16_t value);

void stream_write_uint32_be(struct stream_write *t, uint32_t value);

void stream_write_string(struct stream_write *t, const std::string &s);


#endif // STREAM_WRITE_H
