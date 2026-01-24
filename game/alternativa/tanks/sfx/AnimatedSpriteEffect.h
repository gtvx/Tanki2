#ifndef ANIMATEDSPRITEEFFECT_H
#define ANIMATEDSPRITEEFFECT_H

#include "game/alternativa/tanks/utils/objectpool/PooledObject.h"
#include "GraphicEffect.h"
#include <memory>
#include <string>

class AnimatedSprite3D;
class TextureAnimation;
class ColorTransform;
class Object3DPositionProvider;

class AnimatedSpriteEffect : public PooledObject, public GraphicEffect
{
	std::shared_ptr<AnimatedSprite3D> sprite;

	double currentFrame;
	double framesPerMs;
	int loopsCount;
	Object3DPositionProvider *positionProvider;
	Scene3DContainer *container;


public:
	AnimatedSpriteEffect(Pool *pool);

	void init(double _arg_1,
			  double _arg_2,
			  std::shared_ptr<TextureAnimation> _arg_3,
			  double _arg_4,
			  Object3DPositionProvider *_arg_5,
			  double _arg_6 = 0.5,
			  double _arg_7 = 0.5,
			  ColorTransform *_arg_8 = nullptr,
			  double _arg_9 = 130,
			  std::string _arg_10 = "normal");

	void initLooped(double _arg_1,
					double _arg_2,
					std::shared_ptr<TextureAnimation> _arg_3,
					double _arg_4,
					Object3DPositionProvider *_arg_5,
					double _arg_6 = 0.5,
					double _arg_7 = 0.5,
					ColorTransform *_arg_8 = nullptr,
					double _arg_9 = 130,
					std::string _arg_10 = "normal",
					int _arg_11 = -1);


	void addedToScene(Scene3DContainer *container);

	void destroy();

	void kill();

	bool play(int _arg_1, GameCamera *_arg_2);

private:
	void initSprite(double width,
					double height,
					double rotation,
					double originX,
					double originY,
					ColorTransform *colorTransform,
					std::shared_ptr<TextureAnimation> textureAnimation,
					double softAttenuation,
					std::string blendMode);
};

#endif // ANIMATEDSPRITEEFFECT_H
