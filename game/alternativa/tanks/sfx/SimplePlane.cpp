#include "SimplePlane.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"


std::shared_ptr<SimplePlane> new_SimplePlane(double v1, double v2, double v3, double v4)
{
	return std::make_shared<SimplePlane>(v1, v2, v3, v4);
}


SimplePlane::SimplePlane(double v1, double v2, double v3, double v4)
{
	this->originX = v3;
	this->originY = v4;
	bound.boundMinX = -v3 * v1;
	bound.boundMaxX = bound.boundMinX + v1;
	bound.boundMinY = -v4 * v2;
	bound.boundMaxY = bound.boundMinY + v2;
	bound.boundMinZ = 0;
	bound.boundMaxZ = 0;


	QVector<double> _local_5 = {
		bound.boundMinX,
		bound.boundMinY,
		0,
		bound.boundMaxX,
		bound.boundMinY,
		0,
		bound.boundMaxX,
		bound.boundMaxY,
		0,
		bound.boundMinX,
		bound.boundMaxY,
		0
	};


	QVector<double> _local_6 = {0, 1, 1, 1, 1, 0, 0, 0};

	QVector<int> _local_7 = {4, 0, 1, 2, 3};

	addVerticesAndFaces(_local_5, _local_6, _local_7, true);

	calculateFacesNormals();

	this->writeVertices();
}


void SimplePlane::writeVertices()
{
	QVector<Shared<Vertex>> _local_1 = getVertices();
	this->a = _local_1[0];
	this->b = _local_1[1];
	this->c = _local_1[2];
	this->d = _local_1[3];
}


void SimplePlane::setUVs(double _arg_1,
						 double _arg_2,
						 double _arg_3,
						 double _arg_4,
						 double _arg_5,
						 double _arg_6,
						 double _arg_7,
						 double _arg_8)
{
	this->a->u = _arg_1;
	this->a->v = _arg_2;
	this->b->u = _arg_3;
	this->b->v = _arg_4;
	this->c->u = _arg_5;
	this->c->v = _arg_6;
	this->d->u = _arg_7;
	this->d->v = _arg_8;
}


void SimplePlane::setWidth(double value)
{
	bound.boundMinX = this->a->x = (this->d->x = (-(this->originX) * value));
	bound.boundMaxX = this->b->x = (this->c->x = (bound.boundMinX + value));
}


double SimplePlane::length()
{
	return bound.boundMaxY - bound.boundMinY;
}


void SimplePlane::setLength(double value)
{
	bound.boundMinY = (this->a->y = (this->b->y = (-(this->originY) * value)));
	bound.boundMaxY = (this->d->y = (this->c->y = (bound.boundMinY + value)));
}


void SimplePlane::resize(double width, double length)
{
	this->setWidth(width);
	this->setLength(length);
}
