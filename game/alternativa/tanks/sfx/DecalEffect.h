#ifndef DECALEFFECT_H
#define DECALEFFECT_H

#include "game/alternativa/tanks/utils/objectpool/PooledObject.h"
#include "GraphicEffect.h"
#include <memory>

class Decal;
class Vector3;
class TextureMaterial;
class BattleService;

class DecalEffect : public PooledObject, public GraphicEffect
{
	std::shared_ptr<Decal> decal;
	Vector3 *position;
	Vector3 *projectionOrigin;
	std::shared_ptr<TextureMaterial> material;
	double radius;
	int lifeTime;
	int fadeTime;
	int fadeTimeLeft;

public:
	static BattleService *battleService;

	DecalEffect(Pool *pool);

	void init(const Vector3 *position,
			  std::shared_ptr<TextureMaterial> material,
			  const Vector3 *projectionOrigin,
			  double radius,
			  int lifeTime,
			  int time);


	void addedToScene(Scene3DContainer *container) override;
	bool play(int _arg_1, GameCamera *camera) override;
	void destroy() override;
	void kill() override;
};


#endif // DECALEFFECT_H
