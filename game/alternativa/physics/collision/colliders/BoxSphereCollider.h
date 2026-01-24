#ifndef BOXSPHERECOLLIDER_H
#define BOXSPHERECOLLIDER_H

#include "../Collider.h"
#include "game/alternativa/math/Vector3.h"

class BoxSphereCollider : public Collider
{
public:
    BoxSphereCollider();
	bool haveCollision(CollisionShape*, CollisionShape*) override;
	void getContacts(CollisionShape*, CollisionShape*, MyList<ShapeContact*>*) override;
};

#endif // BOXSPHERECOLLIDER_H
