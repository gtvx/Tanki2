#include "BoxSphereCollider.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/physics/collision/primitives/CollisionSphere.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/math/Matrix4.h"
#include "game/alternativa/math/Vector3.h"
#include "MyMath.h"
#include "game/alternativa/physics/collision/primitives/CollisionSphere.h"
#include "game/alternativa/physics/ShapeContact.h"


BoxSphereCollider::BoxSphereCollider()
{

}


bool BoxSphereCollider::haveCollision(CollisionShape *param1, CollisionShape *param2)
{
	CollisionBox *box;
	CollisionSphere *sphere;


	if (param1->type != CollisionShape::Type::SPHERE)
    {
		sphere = (CollisionSphere*)param2;
		box = (CollisionBox*)param1;
    }
    else
    {
		sphere = (CollisionSphere*)param1;
		box = (CollisionBox*)param2;
    }


	const Matrix4 *_loc5_ = &sphere->transform;

	Vector3 spherePosition;

	_loc5_->getAxis(3, &spherePosition);
	const Matrix4 *_loc6_ = &box->transform;
	//_loc6_->getAxis(3, &this->boxPosition);

	Vector3 center;
	_loc6_->transformVectorInverse(&spherePosition, &center);
	const Vector3 *_loc7_ = box->constHS();
	double _loc8_ = _loc7_->getX() + sphere->r;
	double _loc9_ = _loc7_->getY() + sphere->r;
	double _loc10_ = _loc7_->getZ() + sphere->r;
	if (center.getX() > _loc8_ || center.getX() < -_loc8_ || center.getY() > _loc9_ || center.getY() < -_loc9_ || center.getZ() > _loc10_ || center.getZ() < -_loc10_)
    {
        return false;
    }

	Vector3 closestPoint;

	if (center.getX() > _loc7_->getX())
    {
		closestPoint.setX(_loc7_->getX());
    }
	else if (center.getX() < -_loc7_->getX())
    {
		closestPoint.setX(-_loc7_->getX());
    }
    else
    {
		closestPoint.setX(center.getX());
    }
	if (center.getY() > _loc7_->getY())
    {
		closestPoint.setY(_loc7_->getY());
    }
	else if (center.getY() < -_loc7_->getY())
    {
		closestPoint.setY(-_loc7_->getY());
    }
    else
    {
		closestPoint.setY(center.getY());
    }
	if (center.getZ() > _loc7_->getZ())
    {
		closestPoint.setZ(_loc7_->getZ());
    }
	else if (center.getZ() < -_loc7_->getZ())
    {
		closestPoint.setZ(-_loc7_->getZ());
    }
    else
    {
		closestPoint.setZ(center.getZ());
    }
	center.subtract(&closestPoint);
	double _loc11_ = center.lengthSqr();
	return _loc11_ <= sphere->r * sphere->r;
}


void BoxSphereCollider::getContacts(CollisionShape *param1, CollisionShape *param2, MyList<ShapeContact *> *param3)
{
	CollisionBox *box;
	CollisionSphere *sphere;
	if (param1->type != CollisionShape::Type::SPHERE)
	{
		sphere = (CollisionSphere*)param2;
		box = (CollisionBox*)param1;
	}
	else
	{
		sphere = (CollisionSphere*)param1;
		box = (CollisionBox*)param2;
	}
	const Matrix4 *_loc6_ = &sphere->transform;

	Vector3 spherePosition;

	_loc6_->getAxis(3, &spherePosition);
	const Matrix4 *_loc7_ = &box->transform;
	//_loc7_->getAxis(3, &this->boxPosition);


	Vector3 center;
	_loc7_->transformVectorInverse(&spherePosition, &center);

	const Vector3 *_loc8_ = box->constHS();
	double _loc9_ = _loc8_->getX() + sphere->r;
	double _loc10_ = _loc8_->getY() + sphere->r;
	double _loc11_ = _loc8_->getZ() + sphere->r;

	if (center.getX() > _loc9_ || center.getX() < -_loc9_ || center.getY() > _loc10_ || center.getY() < -_loc10_ || center.getZ() > _loc11_ || center.getZ() < -_loc11_)
	{
		return;
	}


	Vector3 closestPoint;

	if (center.getX() > _loc8_->getX())
	{
		closestPoint.setX(_loc8_->getX());
	}
	else if (center.getX() < -_loc8_->getX())
	{
		closestPoint.setX(-_loc8_->getX());
	}
	else
	{
		closestPoint.setX(center.getX());
	}

	if (center.getY() > _loc8_->getY())
	{
		closestPoint.setY(_loc8_->getY());
	}
	else if (center.getY() < -_loc8_->getY())
	{
		closestPoint.setY(-_loc8_->getY());
	}
	else
	{
		closestPoint.setY(center.getY());
	}

	if (center.getZ() > _loc8_->getZ())
	{
		closestPoint.setZ(_loc8_->getZ());
	}
	else if (center.getZ() < -_loc8_->getZ())
	{
		closestPoint.setZ(-_loc8_->getZ());
	}
	else
	{
		closestPoint.setZ(center.getZ());
	}

	center.subtract(&closestPoint);
	double _loc12_ = center.lengthSqr();
	if (_loc12_ > sphere->r * sphere->r)
		return;

	ShapeContact *_loc13_ = ShapeContact::create();
	_loc13_->shape1 = sphere;
	_loc13_->shape2 = box;
	_loc13_->normal.copy(&closestPoint);
	_loc13_->normal.transform4(_loc7_);
	_loc13_->normal.subtract(&spherePosition);
	_loc13_->normal.normalize();
	_loc13_->normal.reverse();
	_loc13_->penetration = sphere->r - MyMath::sqrt(_loc12_);
	_loc13_->position.copy(&_loc13_->normal);
	_loc13_->position.scale(-sphere->r);
	_loc13_->position.add(&spherePosition);
	param3->append(_loc13_);
}
