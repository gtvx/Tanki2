#ifndef TEXTUREDPROPSREGISTRY_H
#define TEXTUREDPROPSREGISTRY_H

#include <QVector>
#include <QMap>
#include <memory>

class PropMesh;
class QString;
class BSP;
class TexturedBSPsCollection;
class Mesh;
class TexturedMeshesCollection;
class PropSprite;
class Sprite3D;
class TexturedSpritesCollection;
class TexturedPropsCollection;
class Dictionary;
class PropObject;
class Class;



class TexturedPropsRegistry
{
	QMap<PropObject*, QMap<QString, TexturedPropsCollection*>*> meshCollections;
	QMap<PropObject*, QMap<QString, TexturedPropsCollection*>*> bspCollections;
	QMap<PropObject*, QMap<QString, TexturedPropsCollection*>*> spriteCollections;

public:
	TexturedPropsRegistry();
	void addBSP(PropMesh *_arg_1, const QString &_arg_2, std::shared_ptr<BSP> _arg_3);

	void addMesh(PropMesh *_arg_1, const QString &_arg_2, std::shared_ptr<Mesh> &_arg_3, const QString _arg_4 = QString());
	void addSprite3D(PropSprite *_arg_1,  std::shared_ptr<Sprite3D> _arg_2);
	QVector<TexturedPropsCollection*> getCollections();

	void clear();

private:
	TexturedBSPsCollection *getBSPsCollection(PropMesh *_arg_1, const QString &_arg_2);
	TexturedMeshesCollection *getMeshesCollection(PropMesh *_arg_1, const QString &_arg_2);
	TexturedSpritesCollection *getSpritesCollection(PropSprite *_arg_1);
	void clearCollections(Dictionary *_arg_1);
};

#endif // TEXTUREDPROPSREGISTRY_H
