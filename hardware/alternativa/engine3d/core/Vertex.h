#ifndef VERTEX_H
#define VERTEX_H

#include "shared.h"

class QString;

class Vertex : public SharedObject
{
	Vertex();

public:

	Shared<Vertex> next;
	Shared<Vertex> value;

	double x, y, z;
	double u, v;
	double offset;

	double cameraX;
	double cameraY;
	double cameraZ;	
	double normalX;
	double normalY;
	double normalZ;

	int transformId;
	int drawId;
	int index;

	static Shared<Vertex> create();
	void remove();
	QString toString() const;
	static Shared<Vertex> createList(int count);
	void clear();
};

#endif // VERTEX_H
