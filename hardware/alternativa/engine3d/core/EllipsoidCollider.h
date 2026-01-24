#ifndef ELLIPSOIDCOLLIDER_H
#define ELLIPSOIDCOLLIDER_H

#include <QVector>
#include "Face.h"

class Object3D;
class Vector3d;
class Vector3D;
class Dictionary;
class Face;

class EllipsoidCollider
{
	std::shared_ptr<Object3D> matrix;
	Vector3D *src;
	Vector3D *displ;
	Vector3D *dest;
	Vector3D *collisionPoint;
	Vector3D *collisionPlane;
	Vector3D *vCenter;
	Vector3D *vA;
	Vector3D *vB;
	Vector3D *vC;
	Vector3D *vD;
	double radiusX;
	double radiusY;
	double radiusZ;
	QVector<Shared<Face>> faces;
	int facesLength;
	double radius;


public:
	double threshold;

	EllipsoidCollider(double radiusX, double radiusY, double radiusZ);
	bool getCollision(const Vector3D*, const Vector3D*, Vector3D*, Vector3D*, Object3D*);


private:
	void prepare(const Vector3D*, const Vector3D*, Object3D*, void*, bool);
	bool checkCollision();
};

#endif // ELLIPSOIDCOLLIDER_H
