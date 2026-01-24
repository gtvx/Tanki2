#ifndef PHYSICSUTILS_H
#define PHYSICSUTILS_H

class Vector3;
class Matrix3;

class PhysicsUtils
{
public:
	static void setBoxInvInertia(double, const Vector3*, Matrix3*);
	static void getCylinderInvInertia(double, double, double, Matrix3*);
};

#endif // PHYSICSUTILS_H
