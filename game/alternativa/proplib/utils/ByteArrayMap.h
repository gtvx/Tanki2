#ifndef BYTEARRAYMAP_H
#define BYTEARRAYMAP_H

#include <QByteArray>
#include <QMap>

class ByteArrayMap
{
	QMap<QString, QByteArray> map;

public:
	ByteArrayMap(const QMap<QString, QByteArray> &map);

	void putValue(const QString &key, const QByteArray value);

	QByteArray getValue(const QString &key);
};


#endif // BYTEARRAYMAP_H
