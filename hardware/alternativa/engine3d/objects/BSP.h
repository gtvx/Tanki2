#ifndef BSP_H
#define BSP_H

#include <QVector>
#include "../core/Object3D.h"
#include "shared.h"
#include "../core/Vertex.h"
#include "../core/Face.h"

class Face;
class Node;
class Vertex;
class Mesh;
class Material;


class BSP : public Object3D
{
public:

	QVector<Shared<Face>> faces;

	std::shared_ptr<Node> root;

	Shared<Vertex> vertexList;

	double threshold;

	int clipping;

	bool splitAnalysis;

	BSP();


	virtual void clonePropertiesFrom(const Object3D *src);

	virtual std::shared_ptr<Object3D> clone() const;
	void createTree(std::shared_ptr<Mesh> _arg_1, bool _arg_2 = false);

	void destroyTree();

	void setMaterialToAllFaces(std::shared_ptr<Material> material);

	void deleteResources();

	void updateBounds(Object3D *_arg_1, Object3D *_arg_2 = nullptr) override;

	void collectPlanes(const Vector3D *_arg_1, const Vector3D *_arg_2, const Vector3D *_arg_3, const Vector3D *_arg_4, const Vector3D *_arg_5, Faces *_arg_6, void *_arg_7) override;

	virtual void prepareResources();

private:
	std::shared_ptr<Node> createNode(Shared<Face> face);
	void collectPlanesNode(std::shared_ptr<Node> _arg_1, const Vector3D *_arg_2, Faces *_arg_3);
};

std::shared_ptr<BSP> new_BSP();

#endif // BSP_H
