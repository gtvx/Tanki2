#ifndef MATRIX3_H
#define MATRIX3_H

class Vector3;
class Quaternion;

class Matrix3
{
public:
	Matrix3(){}

	Matrix3(double m00,
			double m01,
			double m02,
			double m10,
			double m11,
			double m12,
			double m20,
			double m21,
			double m22);

	void setDefault();

	static const Matrix3 ZERO;
	static const Matrix3 IDENTITY;

	union
	{
		struct {
			double m00, m01, m02;
			double m10, m11, m12;
			double m20, m21, m22;
		};
		struct {
			double x_x, x_y, x_z;
			double y_x, y_y, y_z;
			double z_x, z_y, z_z;
		};
	};

	void copy(const Matrix3*);
	void append(const Matrix3*);
	void prependTransposed(const Matrix3*);
	void transformVector(const Vector3*, Vector3*) const;
	void setRotationMatrix(double , double, double);
	void fromAxisAngle(const Vector3 *, double);
	void rotationMatrixToQuaternion(Quaternion*) const;
	void setDirectionVector(const Vector3*);
	void setAxis(const Vector3*, const Vector3*, const Vector3*);
	void getEulerAngles(Vector3*) const;
};

#endif // MATRIX3_H
