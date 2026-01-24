#ifndef SIMPLEPLANE_H
#define SIMPLEPLANE_H

#include "hardware/alternativa/engine3d/objects/Mesh.h"


class SimplePlane : public Mesh
{
	double originX;
	double originY;

protected:
	Shared<Vertex> a;
	Shared<Vertex> b;
	Shared<Vertex> c;
	Shared<Vertex> d;

public:
	SimplePlane(double v1, double v2, double v3, double v4);

	void setUVs(double _arg_1, double _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7, double _arg_8);
	double length();
	void resize(double width, double length);
	void setWidth(double value);
	void setLength(double value);

private:
	void writeVertices();
};

std::shared_ptr<SimplePlane> new_SimplePlane(double v1, double v2, double v3, double v4);

#endif // SIMPLEPLANE_H
