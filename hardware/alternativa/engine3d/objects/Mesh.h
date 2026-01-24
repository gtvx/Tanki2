#ifndef MESH_H
#define MESH_H

#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "../core/Face.h"
#include <QVector>


class Face;
class Vertex;
class Material;
class VertexBufferResource;
class IndexBufferResource;


class Mesh : public Object3D
{
public:
	Mesh();
	Shared<Face> faceList;
	Shared<Vertex> vertexList;
	Shared<Face> transparentList;

	VertexBufferResource *vertexBuffer;
	IndexBufferResource *indexBuffer;

	QVector<std::shared_ptr<Material>> opaqueMaterials;

	QVector<int> opaqueNums;
	QVector<int> opaqueBegins;

	double threshold;

	int sorting;
	int clipping;
	int numTriangles;
	int numOpaqueTriangles;
	int opaqueLength;

	void calculateFacesNormals(bool param1 = true);

	void updateBounds(Object3D *param1, Object3D *param2 = nullptr) override;

	void optimizeForDynamicBSP(int param1 = 1);

	void weldVertices(double = 0, double = 0);

	void weldFaces(double = 0, double = 0, double = 0, bool = false);

	void calculateVerticesNormalsByAngle(double, double = 0);

	void setMaterialToAllFaces(std::shared_ptr<Material>);

	void collectPlanes(const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, Faces*, void* = nullptr) override;

	std::shared_ptr<Object3D> clone() const override;

	QVector<Shared<Face>> getFaces();

	QVector<Shared<Vertex>> getVertices();

	Shared<Vertex> addVertex(double x, double y, double z, double u = 0, double v = 0, void *id = nullptr);

	void deleteResources();

	Shared<Vertex> removeVertex(Shared<Vertex> _arg_1);

	static void calculateVerticesNormalsBySmoothingGroupsForMeshList(QVector<std::shared_ptr<Object3D> > &_arg_1, double _arg_2 = 0);

	void weldFaces2(double _arg_1, double _arg_2, double _arg_3, bool _arg_4 = false);

	void draw(Camera3D *camera) override;

	void addVerticesAndFaces(const QVector<double> &_arg_1, const QVector<double> &_arg_2, const QVector<int> _arg_3, bool _arg_4 = false, std::shared_ptr<Material> _arg_5 = nullptr);

	Shared<VG> getVG(Camera3D *camera) override;


protected:
	void clonePropertiesFrom(const Object3D*) override;

private:
	void calculateNormalsAndRemoveDegenerateFaces();
	virtual void prepareResources();
	Shared<Face> prepareFaces(Camera3D *camera, Shared<Face> face);
	void drawFaces(Camera3D *_arg_1, Shared<Face>_arg_2);
	void addOpaque(Camera3D *camera);
};

std::shared_ptr<Mesh> new_Mesh();

#endif // MESH_H
