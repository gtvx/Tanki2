#include "TurretGeometryItem.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"


TurretGeometryItem::TurretGeometryItem(Object3D *param1)
{
	this->halfSize.setX(param1->bound.boundMaxX - param1->bound.boundMinX);
	this->halfSize.setY(param1->bound.boundMaxY - param1->bound.boundMinY);
	this->halfSize.setZ(param1->bound.boundMaxZ - param1->bound.boundMinZ);

	this->halfSize.scale(0.5);

	Vector3 vector3(param1->bound.boundMaxX + param1->bound.boundMinX,
					param1->bound.boundMinY + param1->bound.boundMaxY,
					param1->bound.boundMinZ + param1->bound.boundMaxZ);

	vector3.scale(0.5);

	this->transform.setMatrix(vector3.getX() + param1->x,
							  vector3.getY() + param1->y,
							  vector3.getZ() + param1->z,
							  param1->rotationX,
							  param1->rotationY,
							  param1->rotationZ);
}


const Vector3* TurretGeometryItem::getHalfSize() const
{
	return &this->halfSize;
}


const Matrix4* TurretGeometryItem::getTransform() const
{
	return &this->transform;
}
