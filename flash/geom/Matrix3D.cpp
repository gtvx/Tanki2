#include "Matrix3D.h"
#include "Vector3D.h"
#include "MyMath.h"
#include <QString>


void Matrix3D::identity()
{
	set(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}


void Matrix3D::append(const Matrix3D& lhs, bool m33)
{
	double r00 = this->rawData[0];
	double r01 = this->rawData[4];
	double r02 = this->rawData[8];

	double r10 = this->rawData[1];
	double r11 = this->rawData[5];
	double r12 = this->rawData[9];

	double r20 = this->rawData[2];
	double r21 = this->rawData[6];
	double r22 = this->rawData[10];


	double l00 = lhs.rawData[0];
	double l01 = lhs.rawData[4];
	double l02 = lhs.rawData[8];

	double l10 = lhs.rawData[1];
	double l11 = lhs.rawData[5];
	double l12 = lhs.rawData[9];

	double l20 = lhs.rawData[2];
	double l21 = lhs.rawData[6];
	double l22 = lhs.rawData[10];

	if (m33)
	{
		this->rawData[0] = r00 * l00 + r10 * l01 + r20 * l02;
		this->rawData[1] = r00 * l10 + r10 * l11 + r20 * l12;
		this->rawData[2] = r00 * l20 + r10 * l21 + r20 * l22;
		this->rawData[4] = r01 * l00 + r11 * l01 + r21 * l02;
		this->rawData[5] = r01 * l10 + r11 * l11 + r21 * l12;
		this->rawData[6] = r01 * l20 + r11 * l21 + r21 * l22;
		this->rawData[8] = r02 * l00 + r12 * l01 + r22 * l02;
		this->rawData[9] = r02 * l10 + r12 * l11 + r22 * l12;
		this->rawData[10] = r02 * l20 + r12 * l21 + r22 * l22;
	}
	else
	{
		double r03 = this->rawData[12];
		double r13 = this->rawData[13];
		double r23 = this->rawData[14];
		double r33 = this->rawData[15];
		double r30 = this->rawData[3];
		double r31 = this->rawData[7];
		double r32 = this->rawData[11];

		double l03 = lhs.rawData[12];
		double l13 = lhs.rawData[13];
		double l23 = lhs.rawData[14];
		double l33 = lhs.rawData[15];
		double l30 = lhs.rawData[3];
		double l31 = lhs.rawData[7];
		double l32 = lhs.rawData[11];

		this->rawData[0] = r00 * l00 + r10 * l01 + r20 * l02 + r30 * l03;
		this->rawData[1] = r00 * l10 + r10 * l11 + r20 * l12 + r30 * l13;
		this->rawData[2] = r00 * l20 + r10 * l21 + r20 * l22 + r30 * l23;
		this->rawData[3] = r00 * l30 + r10 * l31 + r20 * l32 + r30 * l33;
		this->rawData[4] = r01 * l00 + r11 * l01 + r21 * l02 + r31 * l03;
		this->rawData[5] = r01 * l10 + r11 * l11 + r21 * l12 + r31 * l13;
		this->rawData[6] = r01 * l20 + r11 * l21 + r21 * l22 + r31 * l23;
		this->rawData[7] = r01 * l30 + r11 * l31 + r21 * l32 + r31 * l33;
		this->rawData[8] = r02 * l00 + r12 * l01 + r22 * l02 + r32 * l03;
		this->rawData[9] = r02 * l10 + r12 * l11 + r22 * l12 + r32 * l13;
		this->rawData[10] = r02 * l20 + r12 * l21 + r22 * l22 + r32 * l23;
		this->rawData[11] = r02 * l30 + r12 * l31 + r22 * l32 + r32 * l33;
		this->rawData[12] = r03 * l00 + r13 * l01 + r23 * l02 + r33 * l03;
		this->rawData[13] = r03 * l10 + r13 * l11 + r23 * l12 + r33 * l13;
		this->rawData[14] = r03 * l20 + r13 * l21 + r23 * l22 + r33 * l23;
		this->rawData[15] = r03 * l30 + r13 * l31 + r23 * l32 + r33 * l33;
	}
}


void Matrix3D::decompose(Matrix3DComposeParams &params, Matrix3DOrientation orientationStyle) const
{
	float a = this->rawData[0];
	float e = this->rawData[1];
	float i = this->rawData[2];
	float b = this->rawData[4];
	float f = this->rawData[5];
	float j = this->rawData[6];
	float c = this->rawData[8];
	float g = this->rawData[9];
	float k = this->rawData[10];
	float tx = MyMath::sqrt(a * a + e * e + i * i);
	float ty = MyMath::sqrt(b * b + f * f + j * j);
	float tz = MyMath::sqrt(c * c + g * g + k * k);
	float tw = 0;
	float scaleX = tx;
	float scaleY = ty;
	float scaleZ = tz;

	if (a * (f * k - j * g) - e * (b * k - j * c) + i * (b * g - f * c) < 0)
		scaleZ = -scaleZ;

	a /= scaleX;
	e /= scaleX;
	i /= scaleX;
	b /= scaleY;
	f /= scaleY;
	j /= scaleY;
	c /= scaleZ;
	g /= scaleZ;
	k /= scaleZ;

	if (orientationStyle == Matrix3DOrientation::EULER_ANGLES)
	{
		tx = MyMath::atan2(j, k);
		ty = MyMath::atan2(-i, MyMath::sqrt(a * a + e * e));
		float s1 = MyMath::sin(tx);
		float c1 = MyMath::cos(tx);
		tz = MyMath::atan2(s1 * c - c1 * b, c1 * f - s1 * g);
	}
	else if (orientationStyle == Matrix3DOrientation::AXIS_ANGLE)
	{
		tw = MyMath::acos((a + f + k - 1) / 2);
		float len = MyMath::sqrt((j - g) * (j - g) + (c - i) * (c - i) + (e - b) * (e - b));
		if (len != 0)
		{
			tx = (j - g) / len;
			ty = (c - i) / len;
			tz = (e - b) / len;
		}
		else
		{
			tx = ty = tz = 0;
		}
	}
	else
	{
		float tr = a + f + k;
		if (tr > 0)
		{
			tw = MyMath::sqrt(1 + tr) / 2;
			tr = 4 * tw;
			tx = (j - g) / tr;
			ty = (c - i) / tr;
			tz = (e - b) / tr;
		}
		else if ((a > f) && (a > k))
		{
			tx = MyMath::sqrt(1 + a - f - k) / 2;
			tr = 4 * tx;
			tw = (j - g) / tr;
			ty = (e + b) / tr;
			tz = (c + i) / tr;
		}
		else if (f > k)
		{
			ty = MyMath::sqrt(1 + f - a - k) / 2;
			tr = 4 * ty;
			tx = (e + b) / tr;
			tw = (c - i) / tr;
			tz = (j + g) / tr;
		}
		else
		{
			tz = MyMath::sqrt(1 + k - a - f) / 2;
			tr = 4 * tz;
			tx = (c + i) / tr;
			ty = (j + g) / tr;
			tw = (e - b) / tr;
		}
	}

	params.translation.x = this->rawData[12];
	params.translation.y = this->rawData[13];
	params.translation.z = this->rawData[14];
	params.translation.w = 1;

	params.rotation.x = tx;
	params.rotation.y = ty;
	params.rotation.z = tz;
	params.rotation.w = tw;

	params.scaling.x = scaleX;
	params.scaling.y = scaleY;
	params.scaling.z = scaleZ;
	params.scaling.w = 1;
}


bool Matrix3D::recompose(const Matrix3DComposeParams &params, Matrix3DOrientation orientationStyle)
{
	float scaleX = params.scaling.x, scaleY = params.scaling.y, scaleZ = params.scaling.z;
	if (scaleX == 0 || scaleY == 0 || scaleZ == 0)
		return false;

	if (orientationStyle == Matrix3DOrientation::EULER_ANGLES)
	{
		float cx = MyMath::cos(params.rotation.x);
		float cy = MyMath::cos(params.rotation.y);
		float cz = MyMath::cos(params.rotation.z);
		float sx = MyMath::sin(params.rotation.x);
		float sy = MyMath::sin(params.rotation.y);
		float sz = MyMath::sin(params.rotation.z);
		this->rawData[0] = cy * cz * scaleX;
		this->rawData[1] = cy * sz * scaleX;
		this->rawData[2] = -sy * scaleX;
		this->rawData[4] = (sx * sy * cz - cx * sz) * scaleY;
		this->rawData[5] = (sx * sy * sz + cx * cz) * scaleY;
		this->rawData[6] = sx * cy * scaleY;
		this->rawData[8] = (cx * sy * cz + sx * sz) * scaleZ;
		this->rawData[9] = (cx * sy * sz - sx * cz) * scaleZ;
		this->rawData[10] = cx * cy * scaleZ;
	}
	else
	{
		float x = params.rotation.x;
		float y = params.rotation.y;
		float z = params.rotation.z;
		float w = params.rotation.w;
		if (orientationStyle == Matrix3DOrientation::AXIS_ANGLE)
		{
			float hw = w * .5f;
			float shw = MyMath::sin(hw);
			x *= shw;
			y *= shw;
			z *= shw;
			w = MyMath::cos(hw);
		}
		float t = 2 * x;
		float xx2 = t * x;
		float xy2 = t * y;
		float xz2 = t * z;
		float xw2 = t * w;
		t = 2 * y;
		float yy2 = t * y;
		float yz2 = t * z;
		float yw2 = t * w;
		t = 2 * z;
		float zz2 = t * z;
		float zw2 = t * w;
		t = 1 - zz2;
		this->rawData[0] = (t - yy2) * scaleX;
		this->rawData[1] = (xy2 + zw2) * scaleX;
		this->rawData[2] = (xz2 - yw2) * scaleX;
		this->rawData[4] = (xy2 - zw2) * scaleY;
		this->rawData[5] = (t - xx2) * scaleY;
		this->rawData[6] = (yz2 + xw2) * scaleY;
		this->rawData[8] = (xz2 + yw2) * scaleZ;
		this->rawData[9] = (yz2 - xw2) * scaleZ;
		this->rawData[10] = (1 - xx2 - yy2) * scaleZ;
	}

	this->rawData[3] = this->rawData[7] = this->rawData[11] = 0;
	this->rawData[12] = params.translation.x;
	this->rawData[13] = params.translation.y;
	this->rawData[14] = params.translation.z;
	this->rawData[15] = 1;

	return true;
}


void Matrix3D::appendRotation(double degrees, const Vector3D *axis)
{
	Matrix3D matrix;
	matrix.identity();
	getAxisRotation(axis->x, axis->y, axis->z, degrees, &matrix);
	append(matrix);
}


void Matrix3D::getAxisRotation(double x, double y, double z, double degrees, Matrix3D *result) const
{
	double rad = degrees * (MyMath::PI / 180);

	double c = MyMath::cos(rad);
	double s = MyMath::sin(rad);

	float t = 1 - c;
	result->rawData[0] = c + x * x * t;
	result->rawData[5] = c + y * y * t;
	result->rawData[10] = c + z * z * t;

	float tmp1 = x * y * t;
	float tmp2 = z * s;
	result->rawData[1] = tmp1 + tmp2;
	result->rawData[4] = tmp1 - tmp2;

	tmp1 = x * z * t;
	tmp2 = y * s;
	result->rawData[8] = tmp1 + tmp2;
	result->rawData[2] = tmp1 - tmp2;

	tmp1 = y * z * t;
	tmp2 = x * s;
	result->rawData[9] = tmp1 - tmp2;
	result->rawData[6] = tmp1 + tmp2;
}


void Matrix3D::appendTranslation(double x, double y, double z)
{
	rawData[12] += x;
	rawData[13] += y;
	rawData[14] += z;
}


QString Matrix3D::toString() const
{
	auto number = [](double value) {
		return QString::number(value, 'g', 12);
	};

	return number(this->m00) + " " + number(this->m01) + " " + number(this->m02) + " " + number(this->m03) + "\n" +
			number(this->m10) + " " + number(this->m11) + " " + number(this->m12) + " " + number(this->m13) + "\n" +
			number(this->m20) + " " + number(this->m21) + " " + number(this->m22) + " " + number(this->m23) + "\n" +
			number(this->m30) + " " + number(this->m31) + " " + number(this->m32) + " " + number(this->m33);
}
