#ifndef TURRETGEOMETRYITEM_H
#define TURRETGEOMETRYITEM_H

#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/math/Matrix4.h"

class Object3D;

class TurretGeometryItem
{
	Vector3 halfSize;
	Matrix4 transform;

public:
	TurretGeometryItem(Object3D *param1);
	const Vector3 *getHalfSize() const;
	const Matrix4 *getTransform() const;
};

#endif // TURRETGEOMETRYITEM_H
