#ifndef PROPLIBRARY_H
#define PROPLIBRARY_H

class ByteArrayMap;
class PropGroup;
class TextureByteDataMap;
class QString;
class QDomElement;
class PropObject;
class PropState;
class PropData;
class PropMesh;
class PropSprite;

class PropLibrary
{
	PropGroup *_rootGroup;
	ByteArrayMap *files;
	TextureByteDataMap *imageMap;

public:
	PropLibrary(ByteArrayMap *files);
	QString getName();
	PropGroup *getRootGroup();


private:
	TextureByteDataMap* parseImageMap(const QDomElement &imagesXml);
	PropGroup* parseGroup(const QDomElement &groupXML);
	PropObject* parsePropObject(const QDomElement &parentXmlElement);
	PropState* parseState(const QDomElement &stateXml);
	PropData* parseProp(const QDomElement &propXml);
	PropMesh* parsePropMesh(const QDomElement &propXml);
	PropSprite* parsePropSprite(const QDomElement &propXml);
};

#endif // PROPLIBRARY_H
