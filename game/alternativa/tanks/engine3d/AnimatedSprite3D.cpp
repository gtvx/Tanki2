#include "AnimatedSprite3D.h"
#include "game/alternativa/tanks/engine3d/UVFrame.h"
#include "MyMath.h"
#include "game/alternativa/tanks/engine3d/TextureAnimation.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"


std::shared_ptr<AnimatedSprite3D> new_AnimatedSprite3D(double width, double height, std::shared_ptr<Material> material)
{
	return std::make_shared<AnimatedSprite3D>(width, height, material);
}



AnimatedSprite3D::AnimatedSprite3D(double width, double height, std::shared_ptr<Material> material) :
	Sprite3D(width, height, material)
{
	//useShadowMap = false;
	useLight = false;

	looped = false;
	numFrames = 0;
	fps = 0;
	currentFrame = 0;
}


void AnimatedSprite3D::setAnimationData(TextureAnimation *textureAnimation)
{
	material = textureAnimation->material;
	material->setTransparent(false);

	this->uvFrames =  textureAnimation->frames;
	this->fps = textureAnimation->fps;
	this->numFrames = this->uvFrames.length();
	this->currentFrame = 0;
	this->setFrameIndex(this->currentFrame);
}


double AnimatedSprite3D::getFps()
{
	return this->fps;
}


int AnimatedSprite3D::getNumFrames()
{
	return this->numFrames;
}


void AnimatedSprite3D::clear()
{
	this->uvFrames.clear();
	material = nullptr;
	this->numFrames = 0;
}


void AnimatedSprite3D::setFrameIndex(int index)
{
	index = index % this->numFrames;
	this->setFrame(this->uvFrames[index]);
}


void AnimatedSprite3D::setFrame(UVFrame *uvFrame)
{
	topLeftU = uvFrame->topLeftU;
	topLeftV = uvFrame->topLeftV;
	bottomRightU = uvFrame->bottomRightU;
	bottomRightV = uvFrame->bottomRightV;
}


void AnimatedSprite3D::update(double _arg_1)
{
	this->currentFrame = (this->currentFrame + (this->fps * _arg_1));
	if (this->currentFrame >= this->numFrames)
	{
		if (this->looped)
		{
			this->currentFrame = MyMath::fmod(this->currentFrame, this->numFrames);
		}
		else
		{
			this->currentFrame = (this->numFrames - 1);
		}
	}
	this->setFrame(this->uvFrames[int(this->currentFrame)]);
}
