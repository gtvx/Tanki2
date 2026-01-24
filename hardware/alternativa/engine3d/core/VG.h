#ifndef VG_H
#define VG_H

#include <memory>
#include "shared.h"

class Face;
class Object3D;
class Camera3D;
class Vertex;

class VG : public SharedObject
{
	VG();

public:

	Shared<Face> faceStruct;
	std::shared_ptr<Object3D> object;
	Shared<Vertex> boundVertexList; //8
	Shared<Vertex> boundPlaneList; //6
	Shared<VG> next;

	double boundMinX;
	double boundMinY;
	double boundMinZ;
	double boundMaxX;
	double boundMaxY;
	double boundMaxZ;
	double boundMin;
	double boundMax;

	int sorting;
	int numOccluders;
	int space;
	int id;

	bool viewAligned;

	static Shared<VG> create(int id);
	static Shared<VG> create(std::shared_ptr<Object3D> object, Shared<Face> faceStruct, int sorting, int debug, bool viewAligned, int id);
	void remove();
	void calculateAABB(double _arg_1, double _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7, double _arg_8, double _arg_9, double _arg_10, double _arg_11, double _arg_12);
	void calculateOOBB(Object3D *_arg_1);
	void draw(Camera3D *_arg_1, double _arg_2, Object3D *_arg_3);
	void transformStruct(Shared<Face> _arg_1, int _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7, double _arg_8, double _arg_9, double _arg_10, double _arg_11, double _arg_12, double _arg_13, double _arg_14);
	void destroy();
	void split(Camera3D *_arg_1, double _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6);
	void crop(Camera3D *_arg_1, double _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6);

private:
	void calculateAABBcalculateAABBStruct(Shared<Face> _arg_1, int _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7, double _arg_8, double _arg_9, double _arg_10, double _arg_11, double _arg_12, double _arg_13, double _arg_14);
	void calculateAABBStruct(Shared<Face> _arg_1, int _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7, double _arg_8, double _arg_9, double _arg_10, double _arg_11, double _arg_12, double _arg_13, double _arg_14);
	void calculateOOBBStruct(Shared<Face> _arg_1, int _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7, double _arg_8, double _arg_9, double _arg_10, double _arg_11, double _arg_12, double _arg_13, double _arg_14);
	Shared<Face> cropFaceStruct(Camera3D *_arg_1, Shared<Face> _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7, double _arg_8);
	void updateAABBStruct(Shared<Face> _arg_1, int _arg_2);
	void splitFaceStruct(Camera3D *_arg_1, Shared<Face> _arg_2, Shared<Face> _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7, double _arg_8, double _arg_9);
};

#endif // VG_H
