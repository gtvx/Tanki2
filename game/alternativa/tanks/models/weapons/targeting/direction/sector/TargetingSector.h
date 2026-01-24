#ifndef TARGETINGSECTOR_H
#define TARGETINGSECTOR_H

class Tank;

class TargetingSector
{
	double left;
	double right;
	double distance;
	Tank *tank;

public:
	void set(double left, double right, double distance, Tank *tank);
	double getLeft();
	double getRight();
	double getDistance();
	Tank *getTank();
};

#endif // TARGETINGSECTOR_H
