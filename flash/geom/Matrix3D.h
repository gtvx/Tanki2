#ifndef MATRIX3D_H
#define MATRIX3D_H


#include "Vector3D.h"


enum class Matrix3DOrientation
{
	EULER_ANGLES,
	AXIS_ANGLE,
};

class Matrix3DComposeParams
{
public:
	Vector3D translation;
	Vector3D rotation;
	Vector3D scaling;
};


class Matrix3D
{

public:
	union {
		struct {
			double m00;
			double m01;
			double m02;
			double m03;

			double m10;
			double m11;
			double m12;
			double m13;

			double m20;
			double m21;
			double m22;
			double m23;

			double m30;
			double m31;
			double m32;
			double m33;
		};

		double rawData[16];
	};



	void set(double a1,
			 double a2,
			 double a3,
			 double a4,
			 double a5,
			 double a6,
			 double a7,
			 double a8,
			 double a9,
			 double a10,
			 double a11,
			 double a12,
			 double a13,
			 double a14,
			 double a15,
			 double a16)
	{
		rawData[0] = a1;
		rawData[1] = a2;
		rawData[2] = a3;
		rawData[3] = a4;

		rawData[4] = a5;
		rawData[5] = a6;
		rawData[6] = a7;
		rawData[7] = a8;

		rawData[8] = a9;
		rawData[9] = a10;
		rawData[10] = a11;
		rawData[11] = a12;

		rawData[12] = a13;
		rawData[13] = a14;
		rawData[14] = a15;
		rawData[15] = a16;
	}



	void identity();

	bool recompose(const Matrix3DComposeParams &params, Matrix3DOrientation orientationStyle);
	void decompose(Matrix3DComposeParams &params, Matrix3DOrientation orientationStyle) const;

	QString toString() const;

	void append(const Matrix3D &lhs, bool m33 = false);

	void appendRotation(double theta, const Vector3D *axis);
	void appendTranslation(double tx, double ty, double tz);

	void getAxisRotation(double x, double y, double z, double degrees, Matrix3D *result) const;

};

#endif // MATRIX3D_H
