#ifndef FILE_H
#define FILE_H

class QByteArray;
class QString;

class File
{
public:
	static QByteArray read(const QString &filename);
	static bool read(const QString&, QByteArray &data);
	static bool save(const QString&, const QByteArray &data);
	static bool exists(const QString &filename);
};

#endif // FILE_H
