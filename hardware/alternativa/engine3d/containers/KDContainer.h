#ifndef KDCONTAINER_H
#define KDCONTAINER_H

#include "ConflictContainer.h"
#include <QVector>
#include <QMap>

class KDNode;
class Occluder;
class VertexBufferResource;
class IndexBufferResource;
class Material;
class Device;
class Vertex;
class Decal;


class KDContainer : public  ConflictContainer
{
	QMap<std::shared_ptr<Material>, bool> materials;

	Shared<Vertex> boundVertexList;

	QVector<std::shared_ptr<Object3D>> transparent;

	std::shared_ptr<Object3D> opaqueList;

	int transparentLength;
	int numOccluders;

	bool batched;

	QVector<Shared<Vertex>> occluders;

	double nearPlaneX;
	double nearPlaneY;
	double nearPlaneZ;

	double farPlaneX;
	double farPlaneY;
	double farPlaneZ;

	double leftPlaneX;
	double leftPlaneY;
	double leftPlaneZ;

	double rightPlaneX;
	double rightPlaneY;
	double rightPlaneZ;

	double topPlaneX;
	double topPlaneY;
	double topPlaneZ;

	double bottomPlaneX;
	double bottomPlaneY;
	double bottomPlaneZ;

	double nearPlaneOffset;
	double farPlaneOffset;
	double leftPlaneOffset;
	double rightPlaneOffset;
	double topPlaneOffset;
	double bottomPlaneOffset;


public:
	KDContainer();

	void collectPlanes(const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, Faces*, void* = nullptr) override;
	void createTree(QVector<std::shared_ptr<Object3D>> &_arg_1, QVector<std::shared_ptr<Occluder>> *_arg_2);
	void destroyTree();
	void updateBounds(Object3D *_arg_1, Object3D *_arg_2 = nullptr) override;
	void draw(Camera3D *camera) override;

	std::shared_ptr<Object3D> clone() const;

	std::shared_ptr<Decal> createDecal(const Vector3D *_arg_1,
									   const Vector3D *_arg_2,
									   double _arg_3,
									   double _arg_4,
									   double _arg_5,
									   double _arg_6,
									   std::shared_ptr<Material> _arg_7);

	bool ignoreChildrenInCollider;
	std::shared_ptr<KDNode> root;
	QVector<VertexBufferResource*> receiversVertexBuffers;
	QVector<IndexBufferResource*> receiversIndexBuffers;

private:
	void collectPlanesNode(KDNode*, const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, Faces*, void*);
	void uploadResources(Device *device);
	int cullingInContainer(int _arg_1, double _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7);
	void calculateCameraPlanes(double _arg_1, double _arg_2);
	void updateOccluders(Camera3D *camera);
	void drawNode(KDNode *_arg_1, int _arg_2, Camera3D *_arg_3, Shared<VG> _arg_4);
	bool occludeGeometry(Camera3D *_arg_1, Shared<VG> _arg_2);
};

std::shared_ptr<KDContainer> new_KDContainer();

#endif // KDCONTAINER_H
