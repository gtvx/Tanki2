#ifndef BATTLEUTILS_H
#define BATTLEUTILS_H

class Body;
class Vector3d;
class Vector3;
class AllGlobalGunParams;
class IGameObject;
class Vector3D;

class BattleUtils
{
public:
	static bool isTankBody(Body *body);
	static double toClientScale(double value);
	static bool isFiniteVector3d(const Vector3d *vector3d);
	static void copyToVector3(const Vector3d *vector3d, Vector3 *vector3);
	static void copyToVector3d(const Vector3 *vector3, Vector3d *vector3d);
	static void copyToVector3D(const Vector3 *vector3, Vector3D *vector3D);
	static double getDirectionAngle(const Vector3*);
	static void fillDirectionVector(Vector3*, double);
	static bool isLocalTank(IGameObject *object);
	static bool isTurretAboveGround(Body *body, AllGlobalGunParams *allGlobalGunParams);
	static void globalToLocal(Body *body, Vector3*);
};

#endif // BATTLEUTILS_H
