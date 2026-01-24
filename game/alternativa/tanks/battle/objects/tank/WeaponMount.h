#ifndef WEAPONMOUNT_H
#define WEAPONMOUNT_H

class Matrix3;
class Body;

class WeaponMount
{
public:
	virtual void reset() = 0;
	virtual void setMaxTurnSpeed(double turretRotationSpeed , bool) = 0;
	virtual void updatePhysics(Body*) = 0;
	virtual double getTurretPhysicsDirection() = 0;
	virtual void rotate(double, Matrix3*) = 0;
	virtual void interpolate(double, int) = 0;
	virtual double getTurretInterpolatedDirection() = 0;
	virtual double getBarrelInterpolatedElevation() = 0;
	virtual void setBarrelElevation(double) = 0;
};

#endif // WEAPONMOUNT_H
