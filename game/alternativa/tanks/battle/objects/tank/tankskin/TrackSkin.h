#ifndef TRACKSKIN_H
#define TRACKSKIN_H

#include <QVector>
#include <memory>
#include "shared.h"

class UVMatrixProvider;
class Face;
class Vertex;
class Material;

class TrackSkin
{
	UVMatrixProvider *uvsProvider;
	QVector<Shared<Face>> faces;
	QVector<Shared<Vertex>> vertices;
	double ratio;
	double distance;

public:
	TrackSkin();
	void init();
	void move(double value);
	void setMaterial(std::shared_ptr<Material> material);
	void addFace(Shared<Face> face);
};

#endif // TRACKSKIN_H
