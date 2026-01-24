#include "TankState.h"
#include "Vector3d.h"


TankState::TankState(const Vector3d *angularVelocity,
					 int chassisControl,
					 int chassisTurnSpeedNumber,
					 const Vector3d *linearVelocity,
					 const Vector3d *orientation,
					 const Vector3d *position)
{
	if (angularVelocity != nullptr)
		this->_angularVelocity.copy(angularVelocity);

	this->_chassisControl = chassisControl;

	this->_chassisTurnSpeedNumber = chassisTurnSpeedNumber;

	if (linearVelocity != nullptr)
		this->_linearVelocity.copy(linearVelocity);

	if (orientation != nullptr)
		this->_orientation.copy(orientation);

	if (position != nullptr)
		this->_position.copy(position);
}


void TankState::set_orientation(const Vector3d *orientation)
{
   this->_orientation.copy(orientation);
}


Vector3d* TankState::get_orientation()
{
   return &this->_orientation;
}


void TankState::set_position(const Vector3d *position)
{
   this->_position.copy(position);
}


Vector3d* TankState::get_position()
{
   return &this->_position;
}


void TankState::set_chassisControl(int chassisControl)
{
   this->_chassisControl = chassisControl;
}


int TankState::get_chassisControl()
{
   return this->_chassisControl;
}


void TankState::set_chassisTurnSpeedNumberl(int chassisTurnSpeedNumber)
{
   this->_chassisTurnSpeedNumber = chassisTurnSpeedNumber;
}


int TankState::get_chassisTurnSpeedNumber()
{
   return this->_chassisTurnSpeedNumber;
}
