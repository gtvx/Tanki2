#ifndef BODY_H
#define BODY_H

class BodyState;
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/physics/collision/types/AABB.h"
#include "game/alternativa/math/Matrix3.h"
#include "BodyState.h"
#include <QVector>

class CollisionShape;
class EncryptedNumber;
class Tank;
class BodyCollisionFilter;
class Matrix4;
class PhysicsScene;

class Body
{
public:
	Body(double mass, const Matrix3 *invInertia, double maxSpeedXY);

	double mass;
	BodyState state;
	double invMass;
	void clearAccumulators();
	void calcDerivedData();
	bool canFreeze;
	bool frozen;
	int freezeCounter;
	Vector3 acceleration;
	void calcAccelerations();
	void saveState();
	bool movable;
	Vector3 forceAccum;
	Vector3 torqueAccum;
	AABB aabb;
	Matrix3 baseMatrix;
	Matrix3 invInertiaWorld;
	int numCollisionShapes;
	Matrix3 invInertia;
	QVector<CollisionShape*> *collisionShapes;
	Vector3 angularAcceleration;
	void integrateVelocity(double);
	void integratePosition(double);
	void integratePseudoVelocity(double);
	Vector3 pseudoVelocity;
	Vector3 pseudoAngularVelocity;
	Vector3 previousVelocity;
	bool slipperyMode;
	void applyWorldPseudoImpulseAtLocalPoint(Vector3*, Vector3*, double);
	void applyWorldImpulseAtLocalPoint(Vector3*, Vector3*, double);
	int id;
	BodyState prevState;
	Tank *tank;
	BodyCollisionFilter *postCollisionFilter;

	void addCollisionShape(CollisionShape *, const Matrix4 * = nullptr);

	PhysicsScene *scene;

	void setMaxSpeedXY(double maxSpeedXY);
	void interpolate(double , Vector3 *, Quaternion *);
	void addForceXYZ(double x, double y, double z);
	void addWorldForceAtLocalPoint(const Vector3 *param1, Vector3 *);
	void addWorldForceScaled(const Vector3 *param1, const Vector3 *, double);
private:
	void integrateAngularVelocity(double);
	void integrateLinearVelocity(double);
	double minZAcceleration;
	double maxZSpeedDelta;
	EncryptedNumber *maxSpeedXY;
};

#endif // BODY_H
