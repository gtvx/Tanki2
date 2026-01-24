#ifndef VECTOR3_H
#define VECTOR3_H

class Matrix4;
class Matrix3;
class QString;

class Vector3
{
public:

	static Vector3 *ZERO;
	static Vector3 *X_AXIS;
	static Vector3 *Y_AXIS;
	static Vector3 *Z_AXIS;
	static Vector3 *DOWN;

	union
	{
		struct
		{
			double x, y, z;
		};

		struct
		{
			double X, Y, Z;
		};
	};

	Vector3()
	{
		//this->X = 0;
		//this->Y = 0;
		//this->Z = 0;
	}

	Vector3(double X, double Y, double Z)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	void reverse();
	double dot(const Vector3*);

	void reset(double x = 0, double y = 0, double z = 0)
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

	inline void addX(double x) { this->X += x; }
	inline void addY(double y) { this->Y += y; }
	inline void addZ(double z) { this->Z += z; }

	inline double getX() const { return X; }
	inline double getY() const { return Y; }
	inline double getZ() const { return Z; }

	inline void setX(double x) { this->X = x; }
	inline void setY(double y) { this->Y = y; }
	inline void setZ(double z) { this->Z = z; }

	double lengthSqr() const;
	double length() const;
	void setLength(double);
	void copy(const Vector3*);
	Vector3 *clone() const;
	void cross2(const Vector3*, const Vector3*);
	void normalize();
	bool isFiniteVector() const;
	void scale(double);
	void setLengthAlongDirection(const Vector3*, double);
	void add(Vector3*);
	void cross(const Vector3*);
	void transform4(const Matrix4*);
	void diff(const Vector3*, const Vector3*);
	void subtract(const Vector3*);
	void transformTransposed3(const Matrix3*);
	void addScaled(double, const Vector3*);
	void deltaTransform4(const Matrix4*);
	void sum(const Vector3*, const Vector3*);
	void transform3(const Matrix3*);
	QString toString() const;
	void interpolate(double v, const Vector3 *a, const Vector3 *b);
};

#endif // VECTOR3_H
