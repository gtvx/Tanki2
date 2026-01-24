#ifndef AABB_H
#define AABB_H

class QString;

class AABB
{
public:
	double minX, minY, minZ;
	double maxX, maxY, maxZ;

	//AABB();
	void infinity();
	void addBoundBox(const AABB*);
	bool intersects(const AABB*, double);
	void copyFrom(const AABB*);
	void addPoint(double x, double y, double z);

	QString toString() const;
};

#endif // AABB_H
