#include "DecalEffect.h"
#include "game/alternativa/math/Vector3.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/scene3d/BattleScene3D.h"
#include "hardware/alternativa/engine3d/objects/Decal.h"


BattleService *DecalEffect::battleService;


DecalEffect::DecalEffect(Pool *pool) :
	PooledObject(pool)
{
	this->position = new Vector3;
	this->projectionOrigin = new Vector3;
}


void DecalEffect::init(const Vector3 *position,
					   std::shared_ptr<TextureMaterial> material,
					   const Vector3 *projectionOrigin,
					   double radius,
					   int lifeTime,
					   int time)
{
	this->position->copy(position);
	this->projectionOrigin->copy(projectionOrigin);
	this->radius = radius;
	this->material = material;
	this->lifeTime = lifeTime;
	this->fadeTime = time;
	this->fadeTimeLeft = time;
}


void DecalEffect::addedToScene(Scene3DContainer *container)
{
	(void)container;

	this->decal = battleService->getBattleScene3D()->addPermanentDecal(this->position,
																	   this->projectionOrigin,
																	   this->radius,
																	   this->material);
	if (this->decal == nullptr)
	{
		this->kill();
	}
}


bool DecalEffect::play(int tick, GameCamera *camera)
{
	(void)camera;

	if (this->fadeTimeLeft > 0)
	{
		if (this->lifeTime > 0)
		{
			this->lifeTime = this->lifeTime - tick;
		}
		else
		{
			this->fadeTimeLeft = this->fadeTimeLeft - tick;
			if (this->decal != nullptr)
			{
				this->decal->alpha = (double(this->fadeTimeLeft) / double(this->fadeTime));
			}
		}

		return true;
	}

	return false;
}


void DecalEffect::destroy()
{
	if (this->decal != nullptr)
	{
		battleService->getBattleScene3D()->removeDecal(this->decal);
		this->decal = nullptr;
	}

	this->material = nullptr;
	recycle();
}


void DecalEffect::kill()
{
	this->fadeTimeLeft = 0;
}
