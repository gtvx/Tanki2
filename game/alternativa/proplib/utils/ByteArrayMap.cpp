#include "ByteArrayMap.h"


ByteArrayMap::ByteArrayMap(const QMap<QString, QByteArray> &map)
{
	this->map = map;
}

void ByteArrayMap::putValue(const QString &key, const QByteArray value)
{
	map[key] = value;
}

QByteArray ByteArrayMap::getValue(const QString &key)
{
	return map[key];
}
