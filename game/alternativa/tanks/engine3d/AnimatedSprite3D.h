#ifndef ANIMATEDSPRITE3D_H
#define ANIMATEDSPRITE3D_H

#include "hardware/alternativa/engine3d/objects/Sprite3D.h"
#include <QVector>

class UVFrame;
class TextureAnimation;

class AnimatedSprite3D : public Sprite3D
{
	QVector<UVFrame*> uvFrames;
	int numFrames;
	double fps;
	double currentFrame;

public:

	bool looped;

	AnimatedSprite3D(double width, double height, std::shared_ptr<Material> material = nullptr);
	void update(double _arg_1);

	void setFrameIndex(int index);

	void clear();

	int getNumFrames();

	void setAnimationData(TextureAnimation *textureAnimation);

	double getFps();

private:
	void setFrame(UVFrame *uvFrame);
};



std::shared_ptr<AnimatedSprite3D> new_AnimatedSprite3D(double width, double height, std::shared_ptr<Material> material = nullptr);

#endif // ANIMATEDSPRITE3D_H
