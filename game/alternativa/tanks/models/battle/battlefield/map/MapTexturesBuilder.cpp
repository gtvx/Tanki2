#include "MapTexturesBuilder.h"
#include "TexturedPropsCollection.h"
#include "game/alternativa/tanks/engine3d/IndexedTextureConstructor.h"
#include "game/alternativa/utils/TextureMaterialRegistry.h"
#include "Error.h"


MapTexturesBuilder::MapTexturesBuilder(TextureMaterialRegistry *materialRegistry, int maxBatchSize)
{
	this->materialRegistry = materialRegistry;
	this->maxBatchSize = maxBatchSize;
	this->killed = false;
}


void MapTexturesBuilder::run(const QVector<TexturedPropsCollection*> &list)
{
	this->texturedPropsCollections = list; //list.concat();
	this->totalCounter = 0;
	this->lastCollectionIndex = 0;
	this->createTextureConstructors();
	this->runConstructors();
}


void MapTexturesBuilder::createTextureConstructors()
{
	this->constructors = new QVector<IndexedTextureConstructor*>();
	this->constructors->reserve(this->maxBatchSize);

	for (int i = 0; i < this->maxBatchSize; i++)
	{
		this->constructors->append(new IndexedTextureConstructor());
	}
}


void MapTexturesBuilder::runConstructors()
{
	for (IndexedTextureConstructor *item : *this->constructors)
	{
		this->runConstructor(item);
	}
}


void MapTexturesBuilder::runConstructor(IndexedTextureConstructor *t)
{
	if (this->lastCollectionIndex < this->texturedPropsCollections.length())
	{
		t->index = this->lastCollectionIndex;
		TexturedPropsCollection *p = this->texturedPropsCollections[this->lastCollectionIndex++];

		TextureByteData *data = p->getTextureData();

		if (data == nullptr)
		{
			throw Error("MapTexturesBuilder::runConstructor");
		}

		t->createTexture(data, this);
	}
}


void MapTexturesBuilder::onTextureReady(TextureConstructor *_arg_1)
{
	IndexedTextureConstructor *_local_2 = (IndexedTextureConstructor*)(_arg_1);
	if (this->killed)
	{
		_local_2->disposeTexture();
	}
	else
	{
		this->textures.append(_local_2->texture());
		this->assignMaterialToProps(_local_2);
		this->totalCounter++;
		if (this->totalCounter == this->texturedPropsCollections.length())
		{
			this->complete();
		}
		else
		{
			this->runConstructor(_local_2);
		}
	}
}


void MapTexturesBuilder::assignMaterialToProps(IndexedTextureConstructor *_arg_1)
{
	TexturedPropsCollection *_local_2 = this->texturedPropsCollections[_arg_1->index];
	_local_2->setMaterial(this->materialRegistry->getMaterial(_arg_1->texture()));
}


void MapTexturesBuilder::complete()
{
	this->texturedPropsCollections.clear();
	this->constructors = nullptr;
	//dispatchEvent(new Event(Event.COMPLETE));
}


QVector<BitmapData*> MapTexturesBuilder::getTextures()
{
	return this->textures;
}
