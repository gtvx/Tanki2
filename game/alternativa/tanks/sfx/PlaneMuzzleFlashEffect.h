#ifndef PLANEMUZZLEFLASHEFFECT_H
#define PLANEMUZZLEFLASHEFFECT_H

#include "game/alternativa/tanks/utils/objectpool/PooledObject.h"
#include "GraphicEffect.h"
#include <memory>

class SimplePlane;
class Object3D;
class Vector3;
class TextureMaterial;


class PlaneMuzzleFlashEffect : public PooledObject, public GraphicEffect
{
	std::shared_ptr<SimplePlane> plane;

	int timetoLive;
	std::shared_ptr<Object3D> turret;
	Vector3 *localMuzzlePosition;
	Scene3DContainer *container;


	void addedToScene(Scene3DContainer *container) override;
	bool play(int, GameCamera*) override;
	void destroy() override;
	void kill() override;

public:
	PlaneMuzzleFlashEffect(Pool *pool);
	void init(const Vector3 *position, std::shared_ptr<Object3D> turret, std::shared_ptr<TextureMaterial> material, int timetoLive, double width, double length);

};

#endif // PLANEMUZZLEFLASHEFFECT_H
