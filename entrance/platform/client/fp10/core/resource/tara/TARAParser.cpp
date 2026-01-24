#include "TARAParser.h"
#include "stream/stream_read.h"
#include "Error.h"


namespace
{
	class FileInfo
	{
	public:
		QString name;
		int size;

		FileInfo(const QString &name, int size)
		{
			this->name = name;
			this->size = size;
		}
	};
}



TARAParser::TARAParser(const QByteArray &data)
{
	if (!data.isEmpty())
	{
		parse(data);
	}
}


void TARAParser::parse(const QByteArray &data)
{
	this->_data.clear();

	stream_read stream;
	stream_read_init(&stream, (const uint8_t*)data.constData(), data.size());

	const int count = stream_read_uint32_be(&stream);

	std::vector<FileInfo*> list;
	list.reserve(count);

	for (int i = 0; i < count; i++)
	{
		if (stream_read_available(&stream) < 2)
			throw Error("TARAParser::parse limit");

		uint16_t name_size = stream_read_uint16_be(&stream);;

		if (stream_read_available(&stream) < name_size)
			throw Error("TARAParser::parse limit");

		std::string n = stream_read_string(&stream, name_size);
		QString name = QString::fromStdString(n);
		int size = stream_read_uint32_be(&stream);;
		list.push_back(new FileInfo(name, size));
	}

	for (int i = 0; i < count; i++)
	{
		FileInfo *info = list[i];

		if (stream_read_available(&stream) < info->size)
			throw Error("TARAParser::parse limit");

		std::string data = stream_read_string(&stream, info->size);
		this->_data[info->name] = QByteArray::fromStdString(data);

		delete info;
	}
}

QMap<QString, QByteArray> TARAParser::data() const
{
	return _data;
}

QByteArray TARAParser::getFileData(const QString &name) const
{
	return _data[name];
}

bool TARAParser::contains(const QString &name) const
{
	return _data.contains(name);
}

