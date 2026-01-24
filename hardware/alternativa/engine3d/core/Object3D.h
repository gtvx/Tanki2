#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "game/alternativa/math/Matrix4.h"
#include "game/alternativa/tanks/physics/MinMax.h"
#include <memory>
#include <string>
#include "shared.h"


class Object3DContainer;
class QString;
class Matrix3D;
class Vector3D;
class Camera3D;
class VG;
class ColorTransform;
class Faces;


struct Bound
{
	union
	{
		struct
		{
			double boundMinX;
			double boundMinY;
			double boundMinZ;
			double boundMaxX;
			double boundMaxY;
			double boundMaxZ;
		};

		struct
		{
			double MinX;
			double MinY;
			double MinZ;
			double MaxX;
			double MaxY;
			double MaxZ;
		};
	};

	void copy(const Bound *src);
};


class Object3D : public std::enable_shared_from_this<Object3D>
{
public:

	Object3D();

	Matrix4 matrix4;
	Matrix4 im;
	Bound bound;

	double x, y, z;
	double rotationX, rotationY, rotationZ;
	double scaleX, scaleY, scaleZ;

	double alpha;
	double transformId;
	double softAttenuation;
	double shadowMapAlphaThreshold;
	double concatenatedAlpha;
	double depthMapAlphaThreshold;

	float transformConst[12];
	float colorConst[8];

	bool useDepth;
	bool mouseEnabled;
	bool visible;
	bool useLight;
	bool useShadowMap;

	int culling;

	std::string name;
	std::string name_class;
	std::string concatenatedBlendMode;
	ColorTransform *colorTransform;
	ColorTransform *concatenatedColorTransform;
	std::string blendMode;
	std::shared_ptr<Object3DContainer> _parent;
	std::shared_ptr<Object3D> next;

	void calculateBounds();
	virtual void updateBounds(Object3D*, Object3D* = nullptr);
	void setParent(std::shared_ptr<Object3DContainer> param1);
	std::shared_ptr<Object3DContainer> getParent();
	void composeMatrixFromSource(const Object3D*);
	void appendMatrix(const Object3D*);
	void get_concatenatedMatrix(Matrix4*) const;
	void composeMatrix();
	void invertMatrix();
	void composeAndAppend(const Object3D*);
	virtual void collectPlanes(const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, Faces*, void* = nullptr);
	void calculateSphere(const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, Vector3D*);
	void calculateInverseMatrix();
	bool boundIntersectSphere(const Vector3D*, double, double, double, double, double, double);
	virtual void clonePropertiesFrom(const Object3D *src);
	virtual std::shared_ptr<Object3D> clone() const;
	void getMatrix(Matrix3D *m);
	void setMatrix(const Matrix3D *m);
	virtual int cullingInCamera(Camera3D *_arg_1, int _arg_2);
	virtual Shared<VG> getVG(Camera3D *camera);
	virtual void draw(Camera3D *camera);
	void concat(Object3DContainer*);
	void copyAndAppend(Object3D *_arg_1, Object3D *_arg_2);
};

std::shared_ptr<Object3D> new_Object3D();

#endif // OBJECT3D_H
