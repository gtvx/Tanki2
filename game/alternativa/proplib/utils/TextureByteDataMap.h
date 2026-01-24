#ifndef TEXTUREBYTEDATAMAP_H
#define TEXTUREBYTEDATAMAP_H

#include <QMap>

class TextureByteData;

class TextureByteDataMap
{
	QMap<QString, TextureByteData*> map;

public:
	TextureByteDataMap();

	void putValue(const QString &key, TextureByteData *value);

	TextureByteData* getValue(const QString &key);
};

#endif // TEXTUREBYTEDATAMAP_H
