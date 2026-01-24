#ifndef PHYSICSMATERIAL_H
#define PHYSICSMATERIAL_H

class PhysicsMaterial
{
public:
	static const PhysicsMaterial *DEFAULT_MATERIAL;

	PhysicsMaterial(double restitution = 0, double friction = 0.3);

	double restitution;
	double friction;
};

#endif // PHYSICSMATERIAL_H
