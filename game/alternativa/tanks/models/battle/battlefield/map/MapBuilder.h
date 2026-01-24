#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#include <memory>
#include <QVector>

class QDomElement;
class QByteArray;
class KDContainer;
class Object3DContainer;
class MapGeometryParser;
class PropLibRegistry;
class TextureMaterialRegistry;
class MapTexturesBuilder;
class BitmapData;
class AABB;

class MapBuilder
{
public:
	MapBuilder(TextureMaterialRegistry *materialRegistry,
			   PropLibRegistry *propLibRegistry);

	void build(const QByteArray &data);

	std::shared_ptr<KDContainer> getMapContainer();
	TextureMaterialRegistry *materialRegistry;
	PropLibRegistry *propLibRegistry;
	MapGeometryParser *mapGeometryParser;
	std::shared_ptr<KDContainer> mapContainer;
	MapTexturesBuilder *texturesBuilder;
	QVector<BitmapData*> textures;
	AABB *mapBounds;

private:
	void initPhysicsGeometry(const QDomElement &xml);
	void parseMapGeometry(const QDomElement &xml);
	void createMap();
	void complete();
	void buildTextures();
	void onTexturesReady();
	void setupChristmasTree();
	void setupChristmasTreeToys();
	void setupBillboards();
};

#endif // MAPBUILDER_H
