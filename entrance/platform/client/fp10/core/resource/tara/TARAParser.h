#ifndef TARAPARSER_H
#define TARAPARSER_H

#include <QByteArray>
#include <QMap>

class TARAParser
{
	QMap<QString, QByteArray> _data;

public:

	TARAParser(const QByteArray &data);
	void parse(const QByteArray &data);

	QMap<QString, QByteArray> data() const;

	QByteArray getFileData(const QString &name) const;

	bool contains(const QString &name) const;
};

#endif // TARAPARSER_H
