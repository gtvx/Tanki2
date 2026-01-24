#include "PhysicsUtils.h"
#include "game/alternativa/math/Matrix3.h"
#include "game/alternativa/math/Vector3.h"
#include "_global.h"
#include "Error.h"


void PhysicsUtils::setBoxInvInertia(double mass, const Vector3 *param2, Matrix3 *param3)
{
	if (mass <= 0)
		throw ArgumentError();

	param3->copy(&Matrix3::ZERO);
	if (mass == POSITIVE_INFINITY())
		return;

	double X = param2->getX() * param2->getX();
	double Y = param2->getY() * param2->getY();
	double Z = param2->getZ() * param2->getZ();
	param3->x_x = 3. / (mass * (Y + Z));
	param3->y_y = 3. / (mass * (Z + X));
	param3->z_z = 3. / (mass * (X + Y));
}


void PhysicsUtils::getCylinderInvInertia(double param1, double param2, double param3, Matrix3 *param4)
{
	if (param1 <= 0)
		throw ArgumentError();

	param4->copy(&Matrix3::ZERO);
	if (param1 == POSITIVE_INFINITY())
		return;

	param4->x_x = (param4->y_y = (1 / (param1 * (((param3 * param3) / 12) + ((param2 * param3) / 4)))));
	param4->z_z = (2 / ((param1 * param2) * param2));
}
