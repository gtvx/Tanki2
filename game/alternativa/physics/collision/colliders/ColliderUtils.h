#ifndef COLLIDERUTILS_H
#define COLLIDERUTILS_H

enum class FaceSide;
class Vector3;
class Matrix4;
class CollisionBox;
class CollidersVertex;
class CollisionRect;
class CollisionTriangle;

class ColliderUtils
{
public:
	static void buildContactBasis(const Vector3*, const Matrix4*, const Matrix4*, Matrix4*);
	static void getBoxFaceVerticesInCCWOrder(const CollisionBox*, const Vector3*, const FaceSide, CollidersVertex*);
	static void transformFaceToReferenceSpace(const Matrix4*, const Matrix4*, CollidersVertex*, int);
	static void getRectFaceInCCWOrder(const CollisionRect*, const Vector3*, CollidersVertex*);
	static void getTriangleFaceInCCWOrder(CollisionTriangle*, Vector3*, CollidersVertex*);
};

#endif // COLLIDERUTILS_H
