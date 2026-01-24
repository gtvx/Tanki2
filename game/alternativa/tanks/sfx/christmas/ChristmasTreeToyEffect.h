#ifndef CHRISTMASTREETOYEFFECT_H
#define CHRISTMASTREETOYEFFECT_H

#include "game/alternativa/tanks/utils/objectpool/PooledObject.h"
#include "../GraphicEffect.h"
#include <memory>

class Scene3DContainer;
class Sprite3D;
class Object3DPositionProvider;
class Vector3;
class GameCamera;

class ChristmasTreeToyEffect : public PooledObject, virtual public GraphicEffect
{
	//private var sprite:Sprite3D;
	//private var omni:OmniLight = new OmniLight(0, 0, 0);
	//private var container:Scene3DContainer;
	//private var positionProvider:Object3DPositionProvider;
	//private var alive:Boolean;
	//private var time:Number;

public:
	ChristmasTreeToyEffect(Pool *pool);

	void addedToScene(Scene3DContainer *_arg_1);
	void init(std::shared_ptr<Sprite3D> _arg_1, Object3DPositionProvider *_arg_2, Vector3 *_arg_3);
	bool play(int _arg_1, GameCamera *_arg_2);
	void destroy();
	void kill();
};

#endif // CHRISTMASTREETOYEFFECT_H
