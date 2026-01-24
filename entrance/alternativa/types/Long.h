#ifndef LONG_H
#define LONG_H

#include <stdint.h>

class QString;

class Long
{
	union {
		struct { uint32_t _high, _low; };
		uint64_t v;
	};

public:
	Long();
	Long(uint32_t high, uint32_t low);
	QString toString() const;
	bool compare(const Long*) const;
	bool compare(const Long&) const;
	uint32_t high() const { return _high; }
	uint32_t low() const { return _low; }
	bool operator<(const Long &l) const { return this->v < l.v; }
	bool operator>(const Long &l) const { return this->v > l.v; }
	bool operator==(const Long &l) const { return this->v == l.v; }


	static Long* getLong(uint32_t high, uint32_t low);
	static Long fromString(const QString &s);
};

#endif // LONG_H
