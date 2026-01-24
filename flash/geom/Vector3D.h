#ifndef FLASH_VECTOR3D_H
#define FLASH_VECTOR3D_H

class QString;

class Vector3D
{
public:

	static const Vector3D *X_AXIS;
	static const Vector3D *Y_AXIS;
	static const Vector3D *Z_AXIS;

	double x;
	double y;
	double z;
	double w;

	Vector3D()
	{

	}

	Vector3D(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline double getX() const { return x; }
	inline double getY() const { return y; }
	inline double getZ() const { return z; }
	inline double getW() const { return w; }

	inline void setX(double X) { this->x = X; }
	inline void setY(double Y) { this->y = Y; }
	inline void setZ(double Z) { this->z = Z; }
	inline void setW(double W) { this->w = W; }

	double getLength() const;
	double normalize();

	void reset(double x = 0, double y = 0, double z = 0, double w = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	QString toString() const;
};

#endif // FLASH_VECTOR3D_H
