#ifndef PROPLOD_H
#define PROPLOD_H

class PropObject;

class PropLOD
{
public:

	PropObject *prop;
	double distance;

	PropLOD(PropObject *prop, double distance)
	{
		this->prop = prop;
		this->distance = distance;
	}
};

#endif // PROPLOD_H
