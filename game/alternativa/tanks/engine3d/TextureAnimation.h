#ifndef TEXTUREANIMATION_H
#define TEXTUREANIMATION_H

#include <memory>
#include <QVector>

class TextureMaterial;
class UVFrame;

class TextureAnimation
{
public:
	std::shared_ptr<TextureMaterial> material;
	QVector<UVFrame*> frames;
	double fps;

	TextureAnimation(std::shared_ptr<TextureMaterial> material,
					 const QVector<UVFrame*> &frames,
					 double fps = 0)
	{
		this->material = material;
		this->frames = frames;
		this->fps = fps;
	}
};

#endif // TEXTUREANIMATION_H
