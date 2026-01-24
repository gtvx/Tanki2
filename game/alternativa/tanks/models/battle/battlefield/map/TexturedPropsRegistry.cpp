#include "TexturedPropsRegistry.h"
#include "game/alternativa/proplib/objects/PropMesh.h"
#include "TexturedMeshesCollection.h"
#include "TexturedSpritesCollection.h"
#include "TexturedBSPsCollection.h"
#include "game/alternativa/proplib/objects/PropSprite.h"


//TexturedBSPsCollection
//TexturedMeshesCollection
//TexturedSpritesCollection

namespace
{
	typedef TexturedPropsCollection* (*NewObject)(PropObject *propObject, const QString &name);
}


static TexturedPropsCollection* getCollection(QMap<PropObject*, QMap<QString, TexturedPropsCollection*>*> &dictionary_1,
											  PropObject *propObject,
											  const QString &name,
											  NewObject T)
{
	QMap<QString, TexturedPropsCollection*> *dictionary = dictionary_1[propObject];

	if (dictionary == nullptr)
	{
		dictionary = new QMap<QString, TexturedPropsCollection*>();
		dictionary_1[propObject] = dictionary;
	}

	TexturedPropsCollection *texturedPropsCollection = dictionary->value(name);

	if (texturedPropsCollection == nullptr)
	{
		texturedPropsCollection = T(propObject, name);
		dictionary->insert(name, texturedPropsCollection);
	}

	return texturedPropsCollection;
}


TexturedPropsRegistry::TexturedPropsRegistry()
{

}


//BSP
static TexturedPropsCollection* NewTexturedBSPsCollection(PropObject *propObject, const QString &name)
{
	return new TexturedBSPsCollection((PropMesh*)propObject, name);
}

void TexturedPropsRegistry::addBSP(PropMesh *propMesh, const QString &name, std::shared_ptr<BSP> bsp)
{
	this->getBSPsCollection(propMesh, name)->addBsp(bsp);
}

TexturedBSPsCollection* TexturedPropsRegistry::getBSPsCollection(PropMesh *propMesh, const QString &name)
{
	return (TexturedBSPsCollection*)getCollection(this->bspCollections, propMesh, name, &NewTexturedBSPsCollection);
}
//BSP



//Mesh
static TexturedPropsCollection* NewTexturedMeshesCollection(PropObject *propObject, const QString &name)
{
	return new TexturedMeshesCollection((PropMesh*)propObject, name);
}

void TexturedPropsRegistry::addMesh(PropMesh *propMesh, const QString &name, std::shared_ptr<Mesh> &mesh, const QString _arg_4)
{
	getMeshesCollection(propMesh, name)->add(mesh, _arg_4);
}

TexturedMeshesCollection* TexturedPropsRegistry::getMeshesCollection(PropMesh *propMesh, const QString &name)
{
	return (TexturedMeshesCollection*)getCollection(this->meshCollections, propMesh, name, &NewTexturedMeshesCollection);
}
//Mesh



//Sprite3D
static TexturedPropsCollection* NewTexturedSpritesCollection(PropObject *propObject, const QString &name)
{
	return new TexturedSpritesCollection((PropSprite*)propObject, name);
}

void TexturedPropsRegistry::addSprite3D(PropSprite *propSprite, std::shared_ptr<Sprite3D> sprite3D)
{
	this->getSpritesCollection(propSprite)->addSprite3D(sprite3D);
}

TexturedSpritesCollection* TexturedPropsRegistry::getSpritesCollection(PropSprite *propSprite)
{
	return (TexturedSpritesCollection*)getCollection(this->spriteCollections, propSprite, nullptr, &NewTexturedSpritesCollection);
}
//Sprite3D



static void collect(QMap<PropObject*, QMap<QString, TexturedPropsCollection*>*> &list, QVector<TexturedPropsCollection*> &out)
{
	QMapIterator<PropObject*, QMap<QString, TexturedPropsCollection*>*> i(list);
	while (i.hasNext())
	{
		i.next();
		QMap<QString, TexturedPropsCollection*> *v = i.value();

		if (v != nullptr)
		{
			QMapIterator<QString, TexturedPropsCollection*> i(*v);
			while (i.hasNext())
			{
				i.next();
				out.append(i.value());
			}
		}
	}
}



QVector<TexturedPropsCollection*> TexturedPropsRegistry::getCollections()
{
	QVector<TexturedPropsCollection*> list;
	collect(this->bspCollections, list);
	collect(this->meshCollections, list);
	collect(this->spriteCollections, list);
	return list;
}



void TexturedPropsRegistry::clear()
{
	//this->clearCollections(this->meshCollections);
	//this->clearCollections(this->bspCollections);
	//this->clearCollections(this->spriteCollections);
}


/*
void TexturedPropsRegistry::clearCollections(Dictionary *_arg_1)
{
	var _local_2:*;
	for (_local_2 in _arg_1)
	{
		clearDictionary(_arg_1[_local_2]);
		delete _arg_1[_local_2];
	}
}
*/
