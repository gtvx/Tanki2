#include "entrance/alternativa/types/Long.h"
#include <QString>
#include <QHash>

static QHash<uint64_t, Long*> longMap;


Long::Long()
{

}

Long::Long(uint32_t high, uint32_t low) {
	_high = high;
	_low = low;
}

QString Long::toString() const
{
	return QString::asprintf("%08X %08X", _high, _low);
}

bool Long::compare(const Long *l) const
{
	return this->high() == l->high() && this->low() == l->low();
}

bool Long::compare(const Long &l) const
{
	return this->high() == l.high() && this->low() == l.low();
}


Long Long::fromString(const QString &s)
{
	uint64_t v = s.toULongLong(nullptr, 16);
	return Long(v >> 32, v & 0xFFFFFFFF);
}

Long* Long::getLong(uint32_t high, uint32_t low)
{
	uint64_t key = (uint64_t(high) << 32) | low;

	Long *l = longMap[key];

	if (l == nullptr)
	{
		l = new Long(high, low);
		longMap[key] = l;
	}

	return l;
}

