#include "MarginalCollider.h"
#include "game/alternativa/math/Vector3.h"
#include "MyMath.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/physics/PhysicsMaterial.h"
#include "game/alternativa/math/Matrix4.h"
#include "game/alternativa/tanks/physics/TanksCollisionDetector.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"


static const double BOX_HALFSIZE = 6.5;
static const double COS_ONE_DEGREE = MyMath::cos((MyMath::PI / 180));
static CollisionBox *collisionBox = new CollisionBox(new Vector3(1, 1, 1), CollisionGroup::STATIC, PhysicsMaterial::DEFAULT_MATERIAL);

extern BattleService *battleService;



MarginalCollider::MarginalCollider()
{

}


bool MarginalCollider::segmentWithStaticIntersection(const Vector3 *_arg_1, const Vector3 *_arg_2)
{
	setupCollisionBox(_arg_1, _arg_2);
	TanksCollisionDetector *t = battleService->getBattleRunner()->getCollisionDetector();
	return t->testStaticCollision(collisionBox);
}


void MarginalCollider::setupCollisionBox(const Vector3 *a, const Vector3 *b)
{
	Vector3 halfSize;
	halfSize.diff(a, b);
	halfSize.setY(halfSize.length() / 2);
	halfSize.setX(BOX_HALFSIZE);
	halfSize.setZ(BOX_HALFSIZE);

	collisionBox->setHS(&halfSize);

	Vector3 midPoint;
	midPoint.sum(a, b);
	midPoint.scale(0.5);
	Matrix4 *m = &collisionBox->transform;
	m->toIdentity();
	m->setPosition(&midPoint);

	Vector3 direction;
	direction.diff(b, a);
	direction.normalize();

	double v = direction.dot(Vector3::Y_AXIS);

	if (MyMath::abs(direction.getY()) < COS_ONE_DEGREE)
	{
		Vector3 axis;
		axis.cross2(Vector3::Y_AXIS, &direction);
		axis.normalize();
		m->fromAxisAngle(&axis, MyMath::acos(v));
	}

	collisionBox->calculateAABB();
}

