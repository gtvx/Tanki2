#include "SoaringChecker.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/physics/PhysicsMaterial.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/tanks/battle/objects/tank/TankConst.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "game/alternativa/physics/ShapeContact.h"
#include "TanksCollisionDetector.h"
#include "game/alternativa/physics/ShapeContact.h"
#include "TankBody.h"
#include "MyMath.h"
#include <QVector>

extern BattleService *battleService;
static const double HALF_WHEEL_SIZE = 12.5;
static MyList<ShapeContact*> contacts;


SoaringChecker::SoaringChecker(TankBody *tankBody)
{
	soaring = false;
	wasSoaring = false;
	staticIsNear = false;
	this->tank = tankBody->body->tank;
	Vector3 hs;
	hs.copy(this->tank->getMainCollisionBox()->constHS());
	hs.addZ(HALF_WHEEL_SIZE);
	this->testSoaringCollisionBox = new CollisionBox(&hs, CollisionGroup::EMPTY2, PhysicsMaterial::DEFAULT_MATERIAL);
	this->testSoaringCollisionBox->body = this->tank->getBody();
}


void SoaringChecker::updateSoaring()
{
	this->wasSoaring = this->soaring;
	this->updateSoaringCollisionBox();
	battleService->getBattleRunner()->getCollisionDetector()->getShapeContactsWithStatic(this->testSoaringCollisionBox, &contacts);
	double v1 = 0;
	for (int i = 0; i < contacts.count(); i++)
	{
		ShapeContact *shape_contact = contacts.at(i);
		double v2 = shape_contact->normal.dot(shape_contact->shape1 == this->testSoaringCollisionBox ? Vector3::Z_AXIS : Vector3::DOWN);
		v1 = MyMath::max(v1, v2);
		shape_contact->dispose();
	}
	this->staticIsNear = contacts.count() > 0;
	contacts.clear();
	this->soaring = v1 < TankConst::MAX_SLOPE_ANGLE_COS;
}


void SoaringChecker::updateSoaringCollisionBox()
{
	CollisionBox *_loc1_ = this->tank->getMainCollisionBox();
	Matrix4 *m = &this->testSoaringCollisionBox->transform;
	m->copy(&_loc1_->transform);
	m->m03 -= m->m02 * HALF_WHEEL_SIZE;
	m->m13 -= m->m12 * HALF_WHEEL_SIZE;
	m->m23 -= m->m22 * HALF_WHEEL_SIZE;
	this->testSoaringCollisionBox->calculateAABB();
}


bool SoaringChecker::isSoaring()
{
	return this->soaring;
}


bool SoaringChecker::isJumpBegin()
{
	return this->soaring && !this->wasSoaring;
}


bool SoaringChecker::isJumpEnd()
{
	return this->wasSoaring && !this->soaring;
}


bool SoaringChecker::isElasticStaticCollisionWhenSoaring()
{
	return this->soaring && this->staticIsNear;
}
