#include "AnimatedSpriteEffect.h"
#include "game/alternativa/tanks/engine3d/AnimatedSprite3D.h"
#include "Object3DPositionProvider.h"
#include "game/alternativa/tanks/engine3d/TextureAnimation.h"
#include "game/alternativa/tanks/battle/scene3d/scene3dcontainer/Scene3DContainer.h"


AnimatedSpriteEffect::AnimatedSpriteEffect(Pool *pool) :
	PooledObject(pool)
{
	this->sprite = new_AnimatedSprite3D(1, 1);
	this->container = nullptr;
}


void AnimatedSpriteEffect::initSprite(double width,
							double height,
							double rotation,
							double originX,
							double originY,
							ColorTransform *colorTransform,
							std::shared_ptr<TextureAnimation> textureAnimation,
							double softAttenuation,
							std::string blendMode)
{
	this->sprite->width = width;
	this->sprite->height = height;
	this->sprite->rotation = rotation;
	this->sprite->originX = originX;
	this->sprite->originY = originY;
	this->sprite->blendMode = blendMode;
	this->sprite->colorTransform = colorTransform;
	this->sprite->softAttenuation = softAttenuation;
	this->sprite->setAnimationData(textureAnimation.get());
}


void AnimatedSpriteEffect::init(double _arg_1,
								double _arg_2,
								std::shared_ptr<TextureAnimation> _arg_3,
								double _arg_4,
								Object3DPositionProvider *_arg_5,
								double _arg_6,
								double _arg_7,
								ColorTransform *_arg_8,
								double _arg_9,
								std::string _arg_10)
{
	this->initSprite(_arg_1, _arg_2, _arg_4, _arg_6, _arg_7, _arg_8, _arg_3, _arg_9, _arg_10);
	_arg_5->initPosition(this->sprite.get());
	this->framesPerMs = 0.001 * _arg_3->fps;
	this->positionProvider = _arg_5;
	this->currentFrame = 0;
	this->loopsCount = 1;
}


void AnimatedSpriteEffect::initLooped(double _arg_1,
									  double _arg_2,
									  std::shared_ptr<TextureAnimation> _arg_3,
									  double _arg_4,
									  Object3DPositionProvider *_arg_5,
									  double _arg_6,
									  double _arg_7,
									  ColorTransform *_arg_8,
									  double _arg_9,
									  std::string _arg_10,
									  int _arg_11)
{
	this->init(_arg_1, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9, _arg_10);
	this->loopsCount = _arg_11;
}


void AnimatedSpriteEffect::addedToScene(Scene3DContainer *container)
{
	this->container = container;
	container->addChild(this->sprite);
}


bool AnimatedSpriteEffect::play(int _arg_1, GameCamera *_arg_2)
{
	this->sprite->setFrameIndex(this->currentFrame);
	this->currentFrame = this->currentFrame + (_arg_1 * this->framesPerMs);
	this->positionProvider->updateObjectPosition(this->sprite.get(), _arg_2, _arg_1);
	if ((this->loopsCount > 0) && (this->currentFrame >= this->sprite->getNumFrames()))
	{
		this->loopsCount--;
		if (this->loopsCount == 0)
		{
			return false;
		}
		this->currentFrame = this->currentFrame - this->sprite->getNumFrames();
	}
	return true;
}


void AnimatedSpriteEffect::destroy()
{
	this->container->removeChild(this->sprite);
	this->container = nullptr;
	this->sprite->clear();
	this->positionProvider->destroy();
	this->positionProvider = nullptr;
	recycle();
}


void AnimatedSpriteEffect::kill()
{
	this->loopsCount = 1;
	this->currentFrame = this->sprite->getNumFrames();
}
