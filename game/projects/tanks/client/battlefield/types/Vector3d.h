#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3d
{
	double X, Y, Z;
public:

	Vector3d()
	{

	}

	Vector3d(double x, double y, double z)
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

	void copy(const Vector3d *other)
	{
		this->X = other->X;
		this->Y = other->Y;
		this->Z = other->Z;
	}

	inline double getX() const { return X; }
	inline double getY() const { return Y; }
	inline double getZ() const { return Z; }
	inline void setX(double X) { this->X = X; }
	inline void setY(double Y) { this->Y = Y; }
	inline void setZ(double Z) { this->Z = Z; }
};

#endif // VECTOR3D_H
