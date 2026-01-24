#ifndef TANKSCOLLISIONDETECTOR_H
#define TANKSCOLLISIONDETECTOR_H

#include <QVector>
#include "game/alternativa/physics/collision/CollisionDetector.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "MinMax.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/physics/collision/types/AABB.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "MyList.h"


class TankBody;
class ShapeContact;
class CollisionKdNode;
class CollisionShape;
class CollisionKdTree;
class Body;
class Collider;
class AABB;


class TanksCollisionDetector : public CollisionDetector
{
	MyList<TankBody*> tankBodies;
	RayHit _dynamicRayHit;
	Vector3 _normal;
	double threshold;
	Body *staticBody;
	Collider *colliders[10];
	MyList<ShapeContact*> shapeContacts;

public:

	CollisionKdTree *tree;

	TanksCollisionDetector();

	MyList<TankBody*>* getTankBodies();

	void addTankBody(TankBody *tankbody);
	void buildKdTree(MyList<CollisionShape *>*);
	void getShapeContactsWithStatic(CollisionShape*, MyList<ShapeContact *>*);
	bool testStaticCollision(CollisionShape *collisionShape);
	bool testCollision(CollisionShape *a, CollisionShape *b);
	void removeTankBody(TankBody *tankBody);

	void getBodyContacts(MyList<BodyContact*> *param1) override;
	bool raycast(const Vector3 *pos, const Vector3 *dir, int collisionGroup, double time, IRayCollisionFilter *filter, RayHit *rayHit) override;
	bool raycastStatic(const Vector3 *pos, const Vector3 *dir, int collisionGroup, double time, IRayCollisionFilter *filter, RayHit *rayHit) override;

private:
	void getContactsWithStatic(TankBody*, MyList<BodyContact *> *);
	void getContactsWithOtherBodies(TankBody*, int, MyList<BodyContact*> *);
	void getShapeNodeCollisions(CollisionKdNode*, CollisionShape *, MyList<ShapeContact *> *);
	void getContacts(CollisionShape *, CollisionShape *, MyList<ShapeContact *> *);
	bool raycastDynamic(const Vector3 *pos, const Vector3 *dir, int collisionGroup, double time, IRayCollisionFilter *filter, RayHit *rayHit);
	bool getRayBoundBoxIntersection(const Vector3*, const Vector3 *, const AABB *, MinMax *);
	bool testRayAgainstNode(CollisionKdNode*, const Vector3 *, Vector3 *, const Vector3 *, int, double , double , IRayCollisionFilter *filter, RayHit *rayHit);
	bool getRayNodeIntersection(const Vector3*, const Vector3*, int , MyList<CollisionShape *> *, QVector<int> *, IRayCollisionFilter *filter, RayHit *rayHit);
	void createStaticBody();
	void setCollider(CollisionShape::Type, CollisionShape::Type, Collider*);
	bool testShapeNodeCollision(CollisionShape *collisionShape, CollisionKdNode *node);
};

#endif // TANKSCOLLISIONDETECTOR_H
