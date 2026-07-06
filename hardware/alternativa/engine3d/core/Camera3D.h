#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "Object3D.h"
#include "Vertex.h"
#include "Face.h"
#include <QVector>


class Wrapper;
class Face;
class Vertex;
class VulkanWindow;
class Material;
class VertexBufferResource;
class IndexBufferResource;
class View;
class ShadowMap;
class Decal;
class VectexNode;
class VulkanUniform;


class Camera3D : public Object3D
{
public:

	Camera3D();

	double viewSizeX;
	double viewSizeY;

	int numDraws;
	int numPolygons;
	int numTriangles;
	int opaqueCount;

	bool softTransparency;
	bool ssao;
	bool deferredLighting;

	double softTransparencyStrength;
	double focalLength;
	double ssaoStrength;
	double deferredLightingStrength;

	void render(int w, int h);
	void composeCameraMatrix();
	//Wrapper *lastWrapper;

	Shared<Face> firstFace;
	Shared<Face> lastFace;

	Shared<Vertex> firstVertex;
	Shared<Vertex> lastVertex;

	View *view;

	ShadowMap *shadowMap;
	double shadowMapStrength;

	double farClipping;
	double fog;

	bool occludedAll;

	double transform[12];

	double correctionX;
	double correctionY;

	double nearClipping;

	double fov;

	float projection[4];
	float fragmentConst[8];

	Matrix4 matrix_g;

	VectexNode **vertexBufferList;
	int vertexCurrentNumber;

	QVector<Shared<Vertex>> occluders;
	int numOccluders;

	QVector<Shared<Face>> transparentFaceLists;
	QVector<Object3D*> transparentObjects;
	int transparentCount;

	QVector<Object3D*> depthObjects;
	int depthCount;

	QVector<float> constants;

	VertexBufferResource *constantsVertexBuffer;
	IndexBufferResource *constantsIndexBuffer;


	QVector<Decal*> decals;
	int decalsCount;

	QVector<Material*> opaqueMaterials;
	QVector<VertexBufferResource*> opaqueVertexBuffers;
	QVector<IndexBufferResource*> opaqueIndexBuffers;
	QVector<int> opaqueFirstIndexes;
	QVector<int> opaqueNumsTriangles;
	QVector<Object3D*> opaqueObjects;
	QVector<Object3D*> transparentBatchObjects;


	VulkanUniform *vulkanUniform;


	void renderVulkan(VulkanWindow *vulkanWindow);

	Shared<Face> cull(Shared<Face> _arg_1, int _arg_2);

	Shared<Face> clip(Shared<Face> _arg_1, int _arg_2);

	Shared<Face> sortByAverageZ(Shared<Face> _arg_1);

	Shared<Face> sortByDynamicBSP(Shared<Face> _arg_1, double _arg_2, Shared<Face> _arg_3 = nullptr);

	void addTransparent(Shared<Face> face, Object3D *object);

	void addOpaque(Material *material,
				   VertexBufferResource *vertex,
				   IndexBufferResource *index,
				   int firstIndex,
				   int numsTriangles,
				   Object3D *object);

	void addDecal(Decal *decal);


	void initVulkan(VulkanWindow *vulkanWindow);

protected:
	void clonePropertiesFrom(const Object3D*) override;

private:
	void drawTransparentList(VulkanWindow *vulkanWindow, Shared<Face> _arg_1, Object3D *_arg_2, bool _arg_3);
	void deferredDestroy();
};

#endif // CAMERA3D_H
