#include "StaticObject3DPositionProvider.h"
#include "game/alternativa/tanks/camera/GameCamera.h"


StaticObject3DPositionProvider::StaticObject3DPositionProvider(Pool *pool) :
	PooledObject(pool)
{

}


void StaticObject3DPositionProvider::init(const Vector3 *position, double offsetToCamera)
{
	this->position.copy(position);
	this->offsetToCamera = offsetToCamera;
}


void StaticObject3DPositionProvider::initPosition(Object3D*)
{

}


void StaticObject3DPositionProvider::updateObjectPosition(Object3D *object, GameCamera *camera, int)
{
	Vector3 toCamera;
	toCamera.x = camera->x - this->position.x;
	toCamera.y = camera->y - this->position.y;
	toCamera.z = camera->z - this->position.z;
	toCamera.normalize();
	object->x = this->position.x + (this->offsetToCamera * toCamera.x);
	object->y = this->position.y + (this->offsetToCamera * toCamera.y);
	object->z = this->position.z + (this->offsetToCamera * toCamera.z);
}


void StaticObject3DPositionProvider::destroy()
{
	recycle();
}
