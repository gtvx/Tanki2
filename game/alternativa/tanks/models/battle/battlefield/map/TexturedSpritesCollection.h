#ifndef TEXTUREDSPRITESCOLLECTION_H
#define TEXTUREDSPRITESCOLLECTION_H

#include "TexturedPropsCollection.h"
#include <QVector>

class PropSprite;
class Sprite3D;

class TexturedSpritesCollection : public TexturedPropsCollection
{
	PropSprite *propSprite;

	QVector<std::shared_ptr<Sprite3D>> sprites;

public:
	TexturedSpritesCollection(PropSprite *propSprite, const QString &name);
	void addSprite3D(std::shared_ptr<Sprite3D> sprite3D);

	TextureByteData* getTextureData() override;
	void setMaterial(std::shared_ptr<TextureMaterial> textureMaterial) override;
};

#endif // TEXTUREDSPRITESCOLLECTION_H
