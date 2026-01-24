#ifndef QUATERNION_H
#define QUATERNION_H

class Vector3;
class Matrix3;
class Matrix4;
class QString;

class Quaternion
{
public:
	double w, x, y, z;

	void reset();
	void getZAxis(Vector3*) const;
	void getYAxis(Vector3 *_arg_1) const;
	void toMatrix3(Matrix3*) const;
	void addScaledVector(const Vector3*, double);
	void copy(const Quaternion*);
	bool isFiniteQuaternion() const;
	void setFromEulerAnglesXYZ(double x, double y, double z);
	void normalize();
	void toMatrix4(Matrix4*) const;
	Vector3 *getEulerAngles(Vector3*) const;
	void setFromAxisAngleComponents(double, double, double, double);
	void append(const Quaternion*);
	void setFromEulerAngles(const Vector3*);
	void slerp(const Quaternion*, const Quaternion*, double);

	QString toString() const;
};

#endif // QUATERNION_H
