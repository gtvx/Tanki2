#ifndef OCCLUDER_H
#define OCCLUDER_H

#include "../core/Object3D.h"

class Mesh;
class Vertex;
class Face;


class Edge
{
public:
	std::shared_ptr<Edge> next;
	Shared<Vertex> a;
	Shared<Vertex> b;
	Shared<Face> left;
	Shared<Face> right;
};


class Occluder : public Object3D
{
public:

	Shared<Face> faceList;
	std::shared_ptr<Edge> edgeList;
	Shared<Vertex> vertexList;

	Occluder();

	void createForm(std::shared_ptr<Mesh> mesh, bool c = false);

	void destroyForm();

	void clonePropertiesFrom(const Object3D *src) override;

	std::shared_ptr<Object3D> clone() const override;

	void updateBounds(Object3D *_arg_1, Object3D *_arg_2 = nullptr) override;

private:
	QString calculateEdges();
};

std::shared_ptr<Occluder> new_Occluder();

#endif // OCCLUDER_H
