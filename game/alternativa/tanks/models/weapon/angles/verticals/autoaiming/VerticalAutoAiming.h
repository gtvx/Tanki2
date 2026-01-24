#ifndef VERTICALAUTOAIMING_H
#define VERTICALAUTOAIMING_H

class VerticalAutoAiming
{
public:
	virtual double getElevationAngleUp() = 0;
	virtual int getNumRaysUp() = 0;
	virtual double getElevationAngleDown() = 0;
	virtual int getNumRaysDown() = 0;
	virtual double getMaxAngle() = 0;
};

#endif // VERTICALAUTOAIMING_H
