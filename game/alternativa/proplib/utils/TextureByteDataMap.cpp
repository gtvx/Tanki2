#include "TextureByteDataMap.h"
#include <QDebug>

TextureByteDataMap::TextureByteDataMap()
{

}

void TextureByteDataMap::putValue(const QString &key, TextureByteData *value)
{
	map[key] = value;
}

TextureByteData *TextureByteDataMap::getValue(const QString &key)
{
	return map[key];
}
