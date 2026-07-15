#ifndef MAPTEXTURESBUILDER_H
#define MAPTEXTURESBUILDER_H

#include <QVector>
#include "game/alternativa/utils/textureutils/ITextureConstructorListener.h"
#include <memory>

class TextureMaterialRegistry;
class TexturedPropsCollection;
class IndexedTextureConstructor;
class BitmapData;

class MapTexturesBuilder : public ITextureConstructorListener
{
	TextureMaterialRegistry *materialRegistry;
	int maxBatchSize;
	int totalCounter;
	int lastCollectionIndex;
	bool killed;


	QVector<TexturedPropsCollection*> texturedPropsCollections;
	QVector<IndexedTextureConstructor*> *constructors;

	QVector<std::shared_ptr<BitmapData>> textures;

public:
	MapTexturesBuilder(TextureMaterialRegistry *materialRegistry, int maxBatchSize);

	void run(const QVector<TexturedPropsCollection *> &list);
	QVector<std::shared_ptr<BitmapData>> getTextures();

private:
	void createTextureConstructors();
	void runConstructors();
	void runConstructor(IndexedTextureConstructor *_arg_1);

	void onTextureReady(TextureConstructor*) override;
	void assignMaterialToProps(IndexedTextureConstructor *_arg_1);
	void complete();
};

#endif // MAPTEXTURESBUILDER_H
