#include "WeaponUtils.h"
#include "game/alternativa/math/Matrix4.h"
#include "game/alternativa/math/Vector3.h"
#include "AllGlobalGunParams.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "flash/geom/Matrix3D.h"


void WeaponUtils::calculateMainGunParams(const Object3D *object3D, const Vector3 *param2, AllGlobalGunParams *allGlobalGunParams)
{
	Matrix4 turretMatrix;
	turretMatrix.m03 = 0;
	turretMatrix.m13 = 0;
	turretMatrix.m23 = 0;

	object3D->get_concatenatedMatrix(&turretMatrix);

	turretMatrix.transformVector(param2, &allGlobalGunParams->muzzlePosition);

	Vector3 localBarrelOrigin;
	localBarrelOrigin.setX(param2->getX());
	localBarrelOrigin.setY(0);
	localBarrelOrigin.setZ(param2->getZ());

	turretMatrix.transformVector(&localBarrelOrigin, &allGlobalGunParams->barrelOrigin);

	allGlobalGunParams->elevationAxis.setX(turretMatrix.x_x);
	allGlobalGunParams->elevationAxis.setY(turretMatrix.y_x);
	allGlobalGunParams->elevationAxis.setZ(turretMatrix.z_x);

	allGlobalGunParams->direction.setX(turretMatrix.x_y);
	allGlobalGunParams->direction.setY(turretMatrix.y_y);
	allGlobalGunParams->direction.setZ(turretMatrix.z_y);
}

/*
void WeaponUtils::calculateBasicGunParams(Object3D *param1, Vector3 *param2, BasicGlobalGunParams *param3)
{
	Matrix4 turretMatrix;
	turretMatrix.setMatrix(param1.x,param1.y,param1.z,param1.rotationX,param1.rotationY,param1.rotationZ);
	turretMatrix.transformVector(param2,param3.muzzlePosition);
	param3.direction.x = turretMatrix.m01;
	param3.direction.y = turretMatrix.m11;
	param3.direction.z = turretMatrix.m21;
}
*/
