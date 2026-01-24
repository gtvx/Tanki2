#ifndef STATICOBJECT3DPOSITIONPROVIDER_H
#define STATICOBJECT3DPOSITIONPROVIDER_H

#include "game/alternativa/tanks/utils/objectpool/PooledObject.h"
#include "game/alternativa/math/Vector3.h"
#include "Object3DPositionProvider.h"

class Object3D;
class GameCamera;

class StaticObject3DPositionProvider : public PooledObject, public Object3DPositionProvider
{
	Vector3 position;
	double offsetToCamera;

public:
	StaticObject3DPositionProvider(Pool *pool);

	void init(const Vector3 *position, double offsetToCamera);

	void initPosition(Object3D *) override;
	void updateObjectPosition(Object3D *object, GameCamera *camera, int) override;
	void destroy() override;
};

#endif // STATICOBJECT3DPOSITIONPROVIDER_H
