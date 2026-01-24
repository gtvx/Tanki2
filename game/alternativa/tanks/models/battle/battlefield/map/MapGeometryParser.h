#ifndef MAPGEOMETRYPARSER_H
#define MAPGEOMETRYPARSER_H

#include "game/alternativa/physics/collision/types/AABB.h"
#include <QVector>
#include <QMap>
#include <memory>

class PropLibRegistry;
class TexturedPropsRegistry;
class Object3D;
class Sprite3D;
class Occluder;
class Mesh;
class TexturedPropsCollection;
class QDomElement;
class QDomNode;
class PropObject;
class PropMesh;
class PropSprite;
class Vector3;
class Matrix3DComposeParams;
class NormalsCalculator;


class MapGeometryParser
{
	PropLibRegistry *propLibRegistry;
	TexturedPropsRegistry *texturedPropsRegistry;
	AABB mapBounds;

	QVector<std::shared_ptr<Object3D>> objects;
	QVector<std::shared_ptr<Object3D>> sprites;
	QVector<std::shared_ptr<Occluder>> occluders;
	QVector<std::shared_ptr<Object3D>> meshes;

	std::shared_ptr<Object3D> christmasTree;
	QVector<std::shared_ptr<Object3D>> christmasTreeToys;

	QVector<std::shared_ptr<Mesh>> billboards;

	QVector<std::shared_ptr<Object3D>> meshProps;

	QMap<std::shared_ptr<Mesh>, PropMesh*> propObjectByMesh;
	QMap<std::shared_ptr<Mesh>, QString> textureNameByMesh;

	NormalsCalculator *normalsCalculator;

public:
	MapGeometryParser(PropLibRegistry *propLibRegistry);
	AABB *getMapBounds();
	void clear();
	QVector<std::shared_ptr<Object3D>> getObjects();
	QVector<std::shared_ptr<Object3D>> getSprites();
	QVector<std::shared_ptr<Occluder>> getOccluders();
	QVector<std::shared_ptr<Mesh>> getBillboards();
	QVector<TexturedPropsCollection*> getTexturedPropsCollections();
	std::shared_ptr<Object3D> getChristmasTree();
	QVector<std::shared_ptr<Object3D>> getChristmasTreeToys();
	void parse(const QDomElement &xml);

private:
	void makeBSPs();
	void parseProps(const QDomElement &element);
	void parseProp(const QDomElement &element);
	PropObject* getPropObject(const QDomElement &element);
	void parseMesh(const QDomElement &element, PropMesh *propMesh);
	void parseBillboard(const QDomElement &element, PropMesh *propMesh);
	void parseMeshProp(const QDomElement &element, PropMesh *propMesh);
	void parseSprite(const QDomElement &element, PropSprite *propSprite);
	void createOccluders(PropMesh *propMesh, const Matrix3DComposeParams &components);
};

#endif // MAPGEOMETRYPARSER_H
