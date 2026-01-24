#include "file.h"
#include <QFile>
#include <QDebug>


bool File::read(const QString &filename, QByteArray &data)
{
	QFile file(filename);

	if (file.open(QIODevice::ReadOnly))
	{
		data = file.readAll();
		//qDebug() << "File::read" << data.size() << filename;
		file.close();
		return true;
	}
	else
	{
		file.close();
		data.clear();

		qDebug() << "error File::read" << filename;

		return false;
	}
}


QByteArray File::read(const QString &filename)
{
	QFile file(filename);

	if (file.open(QIODevice::ReadOnly))
	{
		return file.readAll();
	}
	else
	{
		file.close();
		qDebug() << "error File::read" << filename;
		return QByteArray();
	}
}


bool File::save(const QString &filename, const QByteArray &data)
{
	QFile file(filename);
	if (file.open(QIODevice::WriteOnly))
	{
		file.write(data);
		return true;
	}
	else
	{
		return false;
	}
}


bool File::exists(const QString &filename)
{
	return QFile::exists(filename);
}
