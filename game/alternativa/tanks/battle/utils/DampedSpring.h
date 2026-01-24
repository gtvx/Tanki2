#ifndef DAMPEDSPRING_H
#define DAMPEDSPRING_H

class DampedSpring
{
	double a;
	double b;
	double epsilon;
	double velocity;
	double acceleration;
	double difference(double, double, double);

public:
	double value;
	DampedSpring();
	DampedSpring(double, double, double epsilon);
	void reset(double = 0.0, double = 0.0);
	void resetValue(double);
	void update(double, double);
};

#endif // DAMPEDSPRING_H
