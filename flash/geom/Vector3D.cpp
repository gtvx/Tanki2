#include "Vector3D.h"
#include "MyMath.h"
#include <QString>

const Vector3D *Vector3D::X_AXIS = new Vector3D(1, 0, 0);
const Vector3D *Vector3D::Y_AXIS = new Vector3D(0, 1, 0);
const Vector3D *Vector3D::Z_AXIS = new Vector3D(0, 0, 1);


double Vector3D::getLength() const
{
	double r = (this->x * this->x) + (this->y * this->y) + (this->z * this->z);

	if (r <= 0)
		return 0;

	return MyMath::sqrt(r);
}

double Vector3D::normalize()
{
	double len = this->getLength();
	double lenInv = len != 0. ? (1. / len) : 0.;
	this->x = this->x * lenInv;
	this->y = this->y * lenInv;
	this->z = this->z * lenInv;
	return len;
}

QString Vector3D::toString() const
{
	return QString::number(this->x, 'g', 12) + " " +
			QString::number(this->y, 'g', 12) + " " +
			QString::number(this->z, 'g', 12);
}
