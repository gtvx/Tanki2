#include "Object3D.h"
#include "MyMath.h"
#include "flash/geom/Matrix3D.h"
#include "flash/geom/Vector3D.h"
#include "game/projects/tanks/client/battlefield/types/Vector3d.h"
#include "hardware/alternativa/engine3d/core/Object3DContainer.h"
#include "Object3D_isType.h"
#include "Camera3D.h"
#include "Vertex.h"
#include "VG.h"
#include "utils/float_array.h"
#include <QString>



void Bound::copy(const Bound *src)
{
	this->boundMinX = src->boundMinX;
	this->boundMinY = src->boundMinY;
	this->boundMinZ = src->boundMinZ;
	this->boundMaxX = src->boundMaxX;
	this->boundMaxY = src->boundMaxY;
	this->boundMaxZ = src->boundMaxZ;
}


std::shared_ptr<Object3D> new_Object3D()
{
	return std::make_shared<Object3D>();
}


std::shared_ptr<Object3D> Object3D::clone() const
{
	auto out = new_Object3D();
	out->clonePropertiesFrom(this);
	return out;
}


void Object3D::clonePropertiesFrom(const Object3D *src)
{
	this->x = src->x;
	this->y = src->y;
	this->z = src->z;

	this->rotationX = src->rotationX;
	this->rotationY = src->rotationY;
	this->rotationZ = src->rotationZ;

	this->scaleX = src->scaleX;
	this->scaleY = src->scaleY;
	this->scaleZ = src->scaleZ;

	this->alpha = src->alpha;

	this->im.copy(&src->im);
	this->bound.copy(&src->bound);

	this->matrix4.copy(&src->matrix4);
	this->mouseEnabled = src->mouseEnabled;
	this->transformId = src->transformId;

	this->shadowMapAlphaThreshold = src->shadowMapAlphaThreshold;

	this->name = src->name;

	this->name_class = src->name_class;

	this->_parent = src->_parent;
	this->next = src->next;

	this->blendMode = src->blendMode;

	this->colorTransform = src->colorTransform;

	this->useShadowMap = useShadowMap;

	float_array_set_8(colorConst, 0, 0, 0, 1, 0, 0, 0, 1);
}


Object3D::Object3D()
{
	this->_parent = nullptr;
	this->next = nullptr;
	this->mouseEnabled = true;
	this->transformId = 0;
	this->bound.boundMinX = 1.0e22;
	this->bound.boundMinY = 1.0e22;
	this->bound.boundMinZ = 1.0e22;
	this->bound.boundMaxX = -1.0e22;
	this->bound.boundMaxY = -1.0e22;
	this->bound.boundMaxZ = -1.0e22;
	this->scaleX = 1;
	this->scaleY = 1;
	this->scaleZ = 1;
	this->shadowMapAlphaThreshold = 1;
	this->culling = 0;
	this->useDepth = false;
	this->useLight = true;
	this->alpha = 1;
	this->useShadowMap = true;

	this->colorTransform = nullptr;
	this->concatenatedColorTransform = nullptr;

	concatenatedAlpha = 1;
	depthMapAlphaThreshold = 1;

	name_class = "Object3D";

	visible = true;

	im.toIdentity();
	matrix4.toIdentity();

	rotationX = 0;
	rotationY = 0;
	rotationZ = 0;
	x = 0;
	y = 0;
	z = 0;

	softAttenuation = 0;

	blendMode = "normal";
	concatenatedBlendMode = "normal";
}


void Object3D::calculateBounds()
{
	this->bound.boundMinX = 1.0e22;
	this->bound.boundMinY = 1.0e22;
	this->bound.boundMinZ = 1.0e22;
	this->bound.boundMaxX = -1.0e22;
	this->bound.boundMaxY = -1.0e22;
	this->bound.boundMaxZ = -1.0e22;
	this->updateBounds(this, nullptr);
	if (this->bound.boundMinX > this->bound.boundMaxX)
	{
		this->bound.boundMinX = -1.0e22;
		this->bound.boundMinY = -1.0e22;
		this->bound.boundMinZ = -1.0e22;
		this->bound.boundMaxX = 1.0e22;
		this->bound.boundMaxY = 1.0e22;
		this->bound.boundMaxZ = 1.0e22;
	}
}


void Object3D::updateBounds(Object3D *, Object3D *)
{

}


void Object3D::setParent(std::shared_ptr<Object3DContainer> param1)
{
	this->_parent = param1;
}


std::shared_ptr<Object3DContainer> Object3D::getParent()
{
	return this->_parent;
}


void Object3D::get_concatenatedMatrix(Matrix4 *out) const
{
	Object3D tA, tB;

	tA.composeMatrixFromSource(this);

	const Object3D *o = this;

	while (o->_parent != nullptr)
	{
		o = o->_parent.get();
		tB.composeMatrixFromSource(o);
		tA.appendMatrix(&tB);
	}

	out->copy(&tA.matrix4);
}


void Object3D::composeMatrixFromSource(const Object3D *param1)
{
	double _loc2_ = MyMath::cos(param1->rotationX);
	double _loc3_ = MyMath::sin(param1->rotationX);
	double _loc4_ = MyMath::cos(param1->rotationY);
	double _loc5_ = MyMath::sin(param1->rotationY);
	double _loc6_ = MyMath::cos(param1->rotationZ);
	double _loc7_ = MyMath::sin(param1->rotationZ);
	double _loc8_ = _loc6_ * _loc5_;
	double _loc9_ = _loc7_ * _loc5_;
	double _loc10_ = _loc4_ * param1->scaleX;
	double _loc11_ = _loc3_ * param1->scaleY;
	double _loc12_ = _loc2_ * param1->scaleY;
	double _loc13_ = _loc2_ * param1->scaleZ;
	double _loc14_ = _loc3_ * param1->scaleZ;
	this->matrix4.ma = _loc6_ * _loc10_;
	this->matrix4.mb = _loc8_ * _loc11_ - _loc7_ * _loc12_;
	this->matrix4.mc = _loc8_ * _loc13_ + _loc7_ * _loc14_;
	this->matrix4.md = param1->x;
	this->matrix4.me = _loc7_ * _loc10_;
	this->matrix4.mf = _loc9_ * _loc11_ + _loc6_ * _loc12_;
	this->matrix4.mg = _loc9_ * _loc13_ - _loc6_ * _loc14_;
	this->matrix4.mh = param1->y;
	this->matrix4.mi = -_loc5_ * param1->scaleX;
	this->matrix4.mj = _loc4_ * _loc11_;
	this->matrix4.mk = _loc4_ * _loc13_;
	this->matrix4.ml = param1->z;
}


void Object3D::appendMatrix(const Object3D *param1)
{
	double _loc2_ = this->matrix4.ma;
	double _loc3_ = this->matrix4.mb;
	double _loc4_ = this->matrix4.mc;
	double _loc5_ = this->matrix4.md;
	double _loc6_ = this->matrix4.me;
	double _loc7_ = this->matrix4.mf;
	double _loc8_ = this->matrix4.mg;
	double _loc9_ = this->matrix4.mh;
	double _loc10_ = this->matrix4.mi;
	double _loc11_ = this->matrix4.mj;
	double _loc12_ = this->matrix4.mk;
	double _loc13_ = this->matrix4.ml;
	this->matrix4.ma = param1->matrix4.ma * _loc2_ + param1->matrix4.mb * _loc6_ + param1->matrix4.mc * _loc10_;
	this->matrix4.mb = param1->matrix4.ma * _loc3_ + param1->matrix4.mb * _loc7_ + param1->matrix4.mc * _loc11_;
	this->matrix4.mc = param1->matrix4.ma * _loc4_ + param1->matrix4.mb * _loc8_ + param1->matrix4.mc * _loc12_;
	this->matrix4.md = param1->matrix4.ma * _loc5_ + param1->matrix4.mb * _loc9_ + param1->matrix4.mc * _loc13_ + param1->matrix4.md;
	this->matrix4.me = param1->matrix4.me * _loc2_ + param1->matrix4.mf * _loc6_ + param1->matrix4.mg * _loc10_;
	this->matrix4.mf = param1->matrix4.me * _loc3_ + param1->matrix4.mf * _loc7_ + param1->matrix4.mg * _loc11_;
	this->matrix4.mg = param1->matrix4.me * _loc4_ + param1->matrix4.mf * _loc8_ + param1->matrix4.mg * _loc12_;
	this->matrix4.mh = param1->matrix4.me * _loc5_ + param1->matrix4.mf * _loc9_ + param1->matrix4.mg * _loc13_ + param1->matrix4.mh;
	this->matrix4.mi = param1->matrix4.mi * _loc2_ + param1->matrix4.mj * _loc6_ + param1->matrix4.mk * _loc10_;
	this->matrix4.mj = param1->matrix4.mi * _loc3_ + param1->matrix4.mj * _loc7_ + param1->matrix4.mk * _loc11_;
	this->matrix4.mk = param1->matrix4.mi * _loc4_ + param1->matrix4.mj * _loc8_ + param1->matrix4.mk * _loc12_;
	this->matrix4.ml = param1->matrix4.mi * _loc5_ + param1->matrix4.mj * _loc9_ + param1->matrix4.mk * _loc13_ + param1->matrix4.ml;
}


void Object3D::composeMatrix()
{
	double _loc1_ = MyMath::cos(this->rotationX);
	double _loc2_ = MyMath::sin(this->rotationX);
	double _loc3_ = MyMath::cos(this->rotationY);
	double _loc4_ = MyMath::sin(this->rotationY);
	double _loc5_ = MyMath::cos(this->rotationZ);
	double _loc6_ = MyMath::sin(this->rotationZ);
	double _loc7_ = _loc5_ * _loc4_;
	double _loc8_ = _loc6_ * _loc4_;
	double _loc9_ = _loc3_ * this->scaleX;
	double _loc10_ = _loc2_ * this->scaleY;
	double _loc11_ = _loc1_ * this->scaleY;
	double _loc12_ = _loc1_ * this->scaleZ;
	double _loc13_ = _loc2_ * this->scaleZ;
	this->matrix4.ma = _loc5_ * _loc9_;
	this->matrix4.mb = _loc7_ * _loc10_ - _loc6_ * _loc11_;
	this->matrix4.mc = _loc7_ * _loc12_ + _loc6_ * _loc13_;
	this->matrix4.md = this->x;
	this->matrix4.me = _loc6_ * _loc9_;
	this->matrix4.mf = _loc8_ * _loc10_ + _loc5_ * _loc11_;
	this->matrix4.mg = _loc8_ * _loc12_ - _loc5_ * _loc13_;
	this->matrix4.mh = this->y;
	this->matrix4.mi = -_loc4_ * this->scaleX;
	this->matrix4.mj = _loc3_ * _loc10_;
	this->matrix4.mk = _loc3_ * _loc12_;
	this->matrix4.ml = this->z;
}


void Object3D::invertMatrix()
{
	double _loc1_ = this->matrix4.ma;
	double _loc2_ = this->matrix4.mb;
	double _loc3_ = this->matrix4.mc;
	double _loc4_ = this->matrix4.md;
	double _loc5_ = this->matrix4.me;
	double _loc6_ = this->matrix4.mf;
	double _loc7_ = this->matrix4.mg;
	double _loc8_ = this->matrix4.mh;
	double _loc9_ = this->matrix4.mi;
	double _loc10_ = this->matrix4.mj;
	double _loc11_ = this->matrix4.mk;
	double _loc12_ = this->matrix4.ml;
	double _loc13_ = 1. / (-_loc3_ * _loc6_ * _loc9_ + _loc2_ * _loc7_ * _loc9_ + _loc3_ * _loc5_ * _loc10_ - _loc1_ * _loc7_ * _loc10_ - _loc2_ * _loc5_ * _loc11_ + _loc1_ * _loc6_ * _loc11_);
	this->matrix4.ma = (-_loc7_ * _loc10_ + _loc6_ * _loc11_) * _loc13_;
	this->matrix4.mb = (_loc3_ * _loc10_ - _loc2_ * _loc11_) * _loc13_;
	this->matrix4.mc = (-_loc3_ * _loc6_ + _loc2_ * _loc7_) * _loc13_;
	this->matrix4.md = (_loc4_ * _loc7_ * _loc10_ - _loc3_ * _loc8_ * _loc10_ - _loc4_ * _loc6_ * _loc11_ + _loc2_ * _loc8_ * _loc11_ + _loc3_ * _loc6_ * _loc12_ - _loc2_ * _loc7_ * _loc12_) * _loc13_;
	this->matrix4.me = (_loc7_ * _loc9_ - _loc5_ * _loc11_) * _loc13_;
	this->matrix4.mf = (-_loc3_ * _loc9_ + _loc1_ * _loc11_) * _loc13_;
	this->matrix4.mg = (_loc3_ * _loc5_ - _loc1_ * _loc7_) * _loc13_;
	this->matrix4.mh = (_loc3_ * _loc8_ * _loc9_ - _loc4_ * _loc7_ * _loc9_ + _loc4_ * _loc5_ * _loc11_ - _loc1_ * _loc8_ * _loc11_ - _loc3_ * _loc5_ * _loc12_ + _loc1_ * _loc7_ * _loc12_) * _loc13_;
	this->matrix4.mi = (-_loc6_ * _loc9_ + _loc5_ * _loc10_) * _loc13_;
	this->matrix4.mj = (_loc2_ * _loc9_ - _loc1_ * _loc10_) * _loc13_;
	this->matrix4.mk = (-_loc2_ * _loc5_ + _loc1_ * _loc6_) * _loc13_;
	this->matrix4.ml = (_loc4_ * _loc6_ * _loc9_ - _loc2_ * _loc8_ * _loc9_ - _loc4_ * _loc5_ * _loc10_ + _loc1_ * _loc8_ * _loc10_ + _loc2_ * _loc5_ * _loc12_ - _loc1_ * _loc6_ * _loc12_) * _loc13_;
}


void Object3D::composeAndAppend(const Object3D *param1)
{
	double _loc2_ = MyMath::cos(this->rotationX);
	double _loc3_ = MyMath::sin(this->rotationX);
	double _loc4_ = MyMath::cos(this->rotationY);
	double _loc5_ = MyMath::sin(this->rotationY);
	double _loc6_ = MyMath::cos(this->rotationZ);
	double _loc7_ = MyMath::sin(this->rotationZ);
	double _loc8_ = _loc6_ * _loc5_;
	double _loc9_ = _loc7_ * _loc5_;
	double _loc10_ = _loc4_ * this->scaleX;
	double _loc11_ = _loc3_ * this->scaleY;
	double _loc12_ = _loc2_ * this->scaleY;
	double _loc13_ = _loc2_ * this->scaleZ;
	double _loc14_ = _loc3_ * this->scaleZ;
	double _loc15_ = _loc6_ * _loc10_;
	double _loc16_ = _loc8_ * _loc11_ - _loc7_ * _loc12_;
	double _loc17_ = _loc8_ * _loc13_ + _loc7_ * _loc14_;
	double _loc18_ = this->x;
	double _loc19_ = _loc7_ * _loc10_;
	double _loc20_ = _loc9_ * _loc11_ + _loc6_ * _loc12_;
	double _loc21_ = _loc9_ * _loc13_ - _loc6_ * _loc14_;
	double _loc22_ = this->y;
	double _loc23_ = -_loc5_ * this->scaleX;
	double _loc24_ = _loc4_ * _loc11_;
	double _loc25_ = _loc4_ * _loc13_;
	double _loc26_ = this->z;
	this->matrix4.ma = param1->matrix4.ma * _loc15_ + param1->matrix4.mb * _loc19_ + param1->matrix4.mc * _loc23_;
	this->matrix4.mb = param1->matrix4.ma * _loc16_ + param1->matrix4.mb * _loc20_ + param1->matrix4.mc * _loc24_;
	this->matrix4.mc = param1->matrix4.ma * _loc17_ + param1->matrix4.mb * _loc21_ + param1->matrix4.mc * _loc25_;
	this->matrix4.md = param1->matrix4.ma * _loc18_ + param1->matrix4.mb * _loc22_ + param1->matrix4.mc * _loc26_ + param1->matrix4.md;
	this->matrix4.me = param1->matrix4.me * _loc15_ + param1->matrix4.mf * _loc19_ + param1->matrix4.mg * _loc23_;
	this->matrix4.mf = param1->matrix4.me * _loc16_ + param1->matrix4.mf * _loc20_ + param1->matrix4.mg * _loc24_;
	this->matrix4.mg = param1->matrix4.me * _loc17_ + param1->matrix4.mf * _loc21_ + param1->matrix4.mg * _loc25_;
	this->matrix4.mh = param1->matrix4.me * _loc18_ + param1->matrix4.mf * _loc22_ + param1->matrix4.mg * _loc26_ + param1->matrix4.mh;
	this->matrix4.mi = param1->matrix4.mi * _loc15_ + param1->matrix4.mj * _loc19_ + param1->matrix4.mk * _loc23_;
	this->matrix4.mj = param1->matrix4.mi * _loc16_ + param1->matrix4.mj * _loc20_ + param1->matrix4.mk * _loc24_;
	this->matrix4.mk = param1->matrix4.mi * _loc17_ + param1->matrix4.mj * _loc21_ + param1->matrix4.mk * _loc25_;
	this->matrix4.ml = param1->matrix4.mi * _loc18_ + param1->matrix4.mj * _loc22_ + param1->matrix4.mk * _loc26_ + param1->matrix4.ml;

}


void Object3D::collectPlanes(const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, Faces*, void*)
{

}


void Object3D::calculateSphere(const Vector3D *param1, const Vector3D *param2, const Vector3D *param3, const Vector3D *param4, const Vector3D *param5, Vector3D *param6)
{
	this->calculateInverseMatrix();

	double _loc7_ = this->im.a * param1->x + this->im.b * param1->y + this->im.c * param1->z + this->im.d;
	double _loc8_ = this->im.e * param1->x + this->im.f * param1->y + this->im.g * param1->z + this->im.h;
	double _loc9_ = this->im.i * param1->x + this->im.j * param1->y + this->im.k * param1->z + this->im.l;
	double _loc10_ = this->im.a * param2->x + this->im.b * param2->y + this->im.c * param2->z + this->im.d;
	double _loc11_ = this->im.e * param2->x + this->im.f * param2->y + this->im.g * param2->z + this->im.h;
	double _loc12_ = this->im.i * param2->x + this->im.j * param2->y + this->im.k * param2->z + this->im.l;
	double _loc13_ = this->im.a * param3->x + this->im.b * param3->y + this->im.c * param3->z + this->im.d;
	double _loc14_ = this->im.e * param3->x + this->im.f * param3->y + this->im.g * param3->z + this->im.h;
	double _loc15_ = this->im.i * param3->x + this->im.j * param3->y + this->im.k * param3->z + this->im.l;
	double _loc16_ = this->im.a * param4->x + this->im.b * param4->y + this->im.c * param4->z + this->im.d;
	double _loc17_ = this->im.e * param4->x + this->im.f * param4->y + this->im.g * param4->z + this->im.h;
	double _loc18_ = this->im.i * param4->x + this->im.j * param4->y + this->im.k * param4->z + this->im.l;
	double _loc19_ = this->im.a * param5->x + this->im.b * param5->y + this->im.c * param5->z + this->im.d;
	double _loc20_ = this->im.e * param5->x + this->im.f * param5->y + this->im.g * param5->z + this->im.h;
	double _loc21_ = this->im.i * param5->x + this->im.j * param5->y + this->im.k * param5->z + this->im.l;
	double _loc22_ = _loc10_ - _loc7_;
	double _loc23_ = _loc11_ - _loc8_;
	double _loc24_ = _loc12_ - _loc9_;
	double _loc25_ = _loc22_ * _loc22_ + _loc23_ * _loc23_ + _loc24_ * _loc24_;
	_loc22_ = _loc13_ - _loc7_;
	_loc23_ = _loc14_ - _loc8_;
	_loc24_ = _loc15_ - _loc9_;
	double _loc26_ = _loc22_ * _loc22_ + _loc23_ * _loc23_ + _loc24_ * _loc24_;
	if (_loc26_ > _loc25_)
	{
		_loc25_ = _loc26_;
	}
	_loc22_ = _loc16_ - _loc7_;
	_loc23_ = _loc17_ - _loc8_;
	_loc24_ = _loc18_ - _loc9_;
	_loc26_ = _loc22_ * _loc22_ + _loc23_ * _loc23_ + _loc24_ * _loc24_;
	if (_loc26_ > _loc25_)
	{
		_loc25_ = _loc26_;
	}
	_loc22_ = _loc19_ - _loc7_;
	_loc23_ = _loc20_ - _loc8_;
	_loc24_ = _loc21_ - _loc9_;
	_loc26_ = _loc22_ * _loc22_ + _loc23_ * _loc23_ + _loc24_ * _loc24_;
	if (_loc26_ > _loc25_)
	{
		_loc25_ = _loc26_;
	}

	param6->x = _loc7_;
	param6->y = _loc8_;
	param6->z = _loc9_;
	param6->w = MyMath::sqrt(_loc25_);
}


void Object3D::calculateInverseMatrix()
{
	double _loc1_ = 1 / (-this->matrix4.mc * this->matrix4.mf * this->matrix4.mi + this->matrix4.mb * this->matrix4.mg * this->matrix4.mi + this->matrix4.mc * this->matrix4.me * this->matrix4.mj - this->matrix4.ma * this->matrix4.mg * this->matrix4.mj - this->matrix4.mb * this->matrix4.me * this->matrix4.mk + this->matrix4.ma * this->matrix4.mf * this->matrix4.mk);
	this->im.a = (-this->matrix4.mg * this->matrix4.mj + this->matrix4.mf * this->matrix4.mk) * _loc1_;
	this->im.b = (this->matrix4.mc * this->matrix4.mj - this->matrix4.mb * this->matrix4.mk) * _loc1_;
	this->im.c = (-this->matrix4.mc * this->matrix4.mf + this->matrix4.mb * this->matrix4.mg) * _loc1_;
	this->im.d = (this->matrix4.md * this->matrix4.mg * this->matrix4.mj - this->matrix4.mc * this->matrix4.mh * this->matrix4.mj - this->matrix4.md * this->matrix4.mf * this->matrix4.mk + this->matrix4.mb * this->matrix4.mh * this->matrix4.mk + this->matrix4.mc * this->matrix4.mf * this->matrix4.ml - this->matrix4.mb * this->matrix4.mg * this->matrix4.ml) * _loc1_;
	this->im.e = (this->matrix4.mg * this->matrix4.mi - this->matrix4.me * this->matrix4.mk) * _loc1_;
	this->im.f = (-this->matrix4.mc * this->matrix4.mi + this->matrix4.ma * this->matrix4.mk) * _loc1_;
	this->im.g = (this->matrix4.mc * this->matrix4.me - this->matrix4.ma * this->matrix4.mg) * _loc1_;
	this->im.h = (this->matrix4.mc * this->matrix4.mh * this->matrix4.mi - this->matrix4.md * this->matrix4.mg * this->matrix4.mi + this->matrix4.md * this->matrix4.me * this->matrix4.mk - this->matrix4.ma * this->matrix4.mh * this->matrix4.mk - this->matrix4.mc * this->matrix4.me * this->matrix4.ml + this->matrix4.ma * this->matrix4.mg * this->matrix4.ml) * _loc1_;
	this->im.i = (-this->matrix4.mf * this->matrix4.mi + this->matrix4.me * this->matrix4.mj) * _loc1_;
	this->im.j = (this->matrix4.mb * this->matrix4.mi - this->matrix4.ma * this->matrix4.mj) * _loc1_;
	this->im.k = (-this->matrix4.mb * this->matrix4.me + this->matrix4.ma * this->matrix4.mf) * _loc1_;
	this->im.l = (this->matrix4.md * this->matrix4.mf * this->matrix4.mi - this->matrix4.mb * this->matrix4.mh * this->matrix4.mi - this->matrix4.md * this->matrix4.me * this->matrix4.mj + this->matrix4.ma * this->matrix4.mh * this->matrix4.mj + this->matrix4.mb * this->matrix4.me * this->matrix4.ml - this->matrix4.ma * this->matrix4.mf * this->matrix4.ml) * _loc1_;
}


bool Object3D::boundIntersectSphere(const Vector3D *param1, double param2, double param3, double param4, double param5, double param6, double param7)
{
	return (((((((param1->x + param1->w) > param2) && ((param1->x - param1->w) < param5)) &&
			   ((param1->y + param1->w) > param3)) && ((param1->y - param1->w) < param6)) &&
			 ((param1->z + param1->w) > param4)) && ((param1->z - param1->w) < param7));
}


void Object3D::getMatrix(Matrix3D *m)
{
	Object3D tA;
	tA.composeMatrixFromSource(this);
	m->rawData[0] = tA.matrix4.ma;
	m->rawData[1] = tA.matrix4.me;
	m->rawData[2] = tA.matrix4.mi;
	m->rawData[3] = 0;
	m->rawData[4] = tA.matrix4.mb;
	m->rawData[5] = tA.matrix4.mf;
	m->rawData[6] = tA.matrix4.mj;
	m->rawData[7] = 0;
	m->rawData[8] = tA.matrix4.mc;
	m->rawData[9] = tA.matrix4.mg;
	m->rawData[10] = tA.matrix4.mk;
	m->rawData[11] = 0;
	m->rawData[12] = tA.matrix4.md;
	m->rawData[13] = tA.matrix4.mh;
	m->rawData[14] = tA.matrix4.ml;
	m->rawData[15] = 1;
}


void Object3D::setMatrix(const Matrix3D *m)
{
	Matrix3DComposeParams params;
	m->decompose(params, Matrix3DOrientation::EULER_ANGLES);
	this->x = params.translation.x;
	this->y = params.translation.y;
	this->z = params.translation.z;
	this->rotationX = params.rotation.x;
	this->rotationY = params.rotation.y;
	this->rotationZ = params.rotation.z;
	this->scaleX = params.scaling.x;
	this->scaleY = params.scaling.y;
	this->scaleZ = params.scaling.z;
}


namespace
{
	struct LocalVertex
	{
		double x, y, z;
		double cameraX, cameraY, cameraZ;
	};
}


int Object3D::cullingInCamera(Camera3D *_arg_1, int _arg_2)
{
	if (_arg_1->occludedAll)
	{
		return -1;
	}

	LocalVertex boundVertexList[8];

	int _local_3 = _arg_1->numOccluders;
	if (((_arg_2 > 0) || (_local_3 > 0)))
	{


		LocalVertex *_local_4;

		_local_4 = &boundVertexList[0];
		_local_4->x = this->bound.boundMinX;
		_local_4->y = this->bound.boundMinY;
		_local_4->z = this->bound.boundMinZ;
		_local_4 = &boundVertexList[1];
		_local_4->x = this->bound.boundMaxX;
		_local_4->y = this->bound.boundMinY;
		_local_4->z = this->bound.boundMinZ;
		_local_4 = &boundVertexList[2];
		_local_4->x = this->bound.boundMinX;
		_local_4->y = this->bound.boundMaxY;
		_local_4->z = this->bound.boundMinZ;
		_local_4 = &boundVertexList[3];
		_local_4->x = this->bound.boundMaxX;
		_local_4->y = this->bound.boundMaxY;
		_local_4->z = this->bound.boundMinZ;
		_local_4 = &boundVertexList[4];
		_local_4->x = this->bound.boundMinX;
		_local_4->y = this->bound.boundMinY;
		_local_4->z = this->bound.boundMaxZ;
		_local_4 = &boundVertexList[5];
		_local_4->x = this->bound.boundMaxX;
		_local_4->y = this->bound.boundMinY;
		_local_4->z = this->bound.boundMaxZ;
		_local_4 = &boundVertexList[6];
		_local_4->x = this->bound.boundMinX;
		_local_4->y = this->bound.boundMaxY;
		_local_4->z = this->bound.boundMaxZ;
		_local_4 = &boundVertexList[7];
		_local_4->x = this->bound.boundMaxX;
		_local_4->y = this->bound.boundMaxY;
		_local_4->z = this->bound.boundMaxZ;

		_local_4 = boundVertexList;

		for (int i = 0; i < 8; i++)
		{
			_local_4 = &boundVertexList[i];
			double _local_5 = _local_4->x;
			double _local_6 = _local_4->y;
			double _local_7 = _local_4->z;
			_local_4->cameraX = ((((this->matrix4.ma * _local_5) + (this->matrix4.mb * _local_6)) + (this->matrix4.mc * _local_7)) + this->matrix4.md);
			_local_4->cameraY = ((((this->matrix4.me * _local_5) + (this->matrix4.mf * _local_6)) + (this->matrix4.mg * _local_7)) + this->matrix4.mh);
			_local_4->cameraZ = ((((this->matrix4.mi * _local_5) + (this->matrix4.mj * _local_6)) + (this->matrix4.mk * _local_7)) + this->matrix4.ml);
			//_local_4 = _local_4->next;
		}
	}

	if (_arg_2 > 0)
	{
		if ((_arg_2 & 0x01))
		{
			double _local_10 = _arg_1->nearClipping;

			bool _local_8 = false;
			bool _local_9 = false;

			for (int i = 0; i < 8; i++)
			{
				LocalVertex *_local_4 = &boundVertexList[i];

				if (_local_4->cameraZ > _local_10)
				{
					_local_8 = true;
					if (_local_9) break;
				}
				else
				{
					_local_9 = true;
					if (_local_8) break;
				}
				//_local_4 = _local_4->next;
			}

			if (_local_9)
			{
				if ((!(_local_8)))
				{
					return (-1);
				}
			}
			else
			{
				_arg_2 = (_arg_2 & 0x3E);
			}
		}

		if ((_arg_2 & 0x02))
		{
			double _local_11 = _arg_1->farClipping;
			//Vertex *_local_4 = boundVertexList;
			bool _local_8 = false;
			bool _local_9 = false;

			for (int i = 0; i < 8; i++)
			{
				LocalVertex *_local_4 = &boundVertexList[i];

				if (_local_4->cameraZ < _local_11)
				{
					_local_8 = true;
					if (_local_9) break;
				}
				else
				{
					_local_9 = true;
					if (_local_8) break;
				}
				//_local_4 = _local_4->next;
			}
			if (_local_9)
			{
				if ((!(_local_8)))
				{
					return (-1);
				}
			}
			else
			{
				_arg_2 = (_arg_2 & 0x3D);
			}
		}

		if ((_arg_2 & 0x04))
		{
			//Vertex *_local_4 = boundVertexList;
			bool  _local_8 = false;
			bool _local_9 = false;

			for (int i = 0; i < 8; i++)
			{
				LocalVertex *_local_4 = &boundVertexList[i];

				if (-(_local_4->cameraX) < _local_4->cameraZ)
				{
					_local_8 = true;
					if (_local_9) break;
				}
				else
				{
					_local_9 = true;
					if (_local_8) break;
				}
				//_local_4 = _local_4->next;
			}
			if (_local_9)
			{
				if ((!(_local_8)))
				{
					return (-1);
				}
			}
			else
			{
				_arg_2 = (_arg_2 & 0x3B);
			}
		}

		if ((_arg_2 & 0x08))
		{
			//Vertex *_local_4 = boundVertexList;
			bool _local_8 = false;
			bool _local_9 = false;

			for (int i = 0; i < 8; i++)
			{
				LocalVertex *_local_4 = &boundVertexList[i];

				if (_local_4->cameraX < _local_4->cameraZ)
				{
					_local_8 = true;
					if (_local_9) break;
				}
				else
				{
					_local_9 = true;
					if (_local_8) break;
				}
				//_local_4 = _local_4->next;
			}
			if (_local_9)
			{
				if ((!(_local_8)))
				{
					return (-1);
				}
			}
			else
			{
				_arg_2 = (_arg_2 & 0x37);
			}
		}

		if ((_arg_2 & 0x10))
		{
			//Vertex *_local_4 = boundVertexList;
			bool _local_8 = false;
			bool _local_9 = false;

			for (int i = 0; i < 8; i++)
			{
				LocalVertex *_local_4 = &boundVertexList[i];

				if (-(_local_4->cameraY) < _local_4->cameraZ)
				{
					_local_8 = true;
					if (_local_9) break;
				}
				else
				{
					_local_9 = true;
					if (_local_8) break;
				}
				//_local_4 = _local_4->next;
			}
			if (_local_9)
			{
				if ((!(_local_8)))
				{
					return (-1);
				}
			}
			else
			{
				_arg_2 = (_arg_2 & 0x2F);
			}
		}
		if ((_arg_2 & 0x20))
		{
			//Vertex *_local_4 = boundVertexList;
			bool _local_8 = false;
			bool _local_9 = false;
			for (int i = 0; i < 8; i++)
			{
				LocalVertex *_local_4 = &boundVertexList[i];
				if (_local_4->cameraY < _local_4->cameraZ)
				{
					_local_8 = true;
					if (_local_9) break;
				}
				else
				{
					_local_9 = true;
					if (_local_8) break;
				}
				//_local_4 = _local_4->next;
			}
			if (_local_9)
			{
				if ((!(_local_8)))
				{
					return (-1);
				}
			}
			else
			{
				_arg_2 = (_arg_2 & 0x1F);
			}
		}
	}

	if (_local_3 > 0)
	{
		int _local_12 = 0;
		while (_local_12 < _local_3)
		{
			Vertex *_local_13 = _arg_1->occluders[_local_12].get();
			while (_local_13 != nullptr)
			{
#if 0
				Vertex *_local_4 = boundVertexList;
				while (_local_4 != nullptr)
				{

					if ((((_local_13->cameraX * _local_4->cameraX) +
						  (_local_13->cameraY * _local_4->cameraY)) +
						 (_local_13->cameraZ * _local_4->cameraZ)) >= 0)
						break;

					_local_4 = _local_4->next;
				}

				if (_local_4 != nullptr)
					break;
#endif

				bool found = false;

				for (int i = 0; i < 8; i++)
				{
					LocalVertex *_local_4 = &boundVertexList[i];

					if ((((_local_13->cameraX * _local_4->cameraX) +
						  (_local_13->cameraY * _local_4->cameraY)) +
						 (_local_13->cameraZ * _local_4->cameraZ)) >= 0)
					{
						found = true;
						break;
					}
				}


				if (found == true)
				{
					break;
				}

				_local_13 = _local_13->next.get();
			}

			if (_local_13 == nullptr)
			{
				return (-1);
			}
			_local_12++;
		}

	}

	this->culling = _arg_2;

	return _arg_2;
}


Shared<VG> Object3D::getVG(Camera3D *camera)
{
	(void)camera;
	return nullptr;
}


void Object3D::draw(Camera3D *camera)
{
	(void)camera;
}


void Object3D::concat(Object3DContainer *container)
{
	this->concatenatedAlpha = container->concatenatedAlpha * this->alpha;

	if (container->concatenatedBlendMode != "normal")
	{
		this->concatenatedBlendMode = container->concatenatedBlendMode;
	}
	else
	{
		this->concatenatedBlendMode = this->blendMode;
	}


	if (container->concatenatedColorTransform != nullptr)
	{
		if (this->colorTransform != nullptr)
		{
			//this->concatenatedColorTransform = new ColorTransform();
			//this->concatenatedColorTransform->redMultiplier = container->concatenatedColorTransform->redMultiplier;
			//this->concatenatedColorTransform->greenMultiplier = container->concatenatedColorTransform->greenMultiplier;
			//this->concatenatedColorTransform->blueMultiplier = container->concatenatedColorTransform->blueMultiplier;
			//this->concatenatedColorTransform->redOffset = container->concatenatedColorTransform->redOffset;
			//this->concatenatedColorTransform->greenOffset = container->concatenatedColorTransform->greenOffset;
			//this->concatenatedColorTransform->blueOffset = container->concatenatedColorTransform->blueOffset;
			//this->concatenatedColorTransform->concat(this->colorTransform);
		}
		else
		{
			this->concatenatedColorTransform = container->concatenatedColorTransform;
		}
	}
	else
	{
		this->concatenatedColorTransform = this->colorTransform;
	}

	if (this->concatenatedColorTransform != nullptr)
	{
		//this->colorConst[0] = this->concatenatedColorTransform->redMultiplier;
		//this->colorConst[1] = this->concatenatedColorTransform->greenMultiplier;
		//this->colorConst[2] = this->concatenatedColorTransform->blueMultiplier;
		this->colorConst[3] = this->concatenatedAlpha;
		//this->colorConst[4] = this->concatenatedColorTransform->redOffset / 255;
		//this->colorConst[5] = this->concatenatedColorTransform->greenOffset / 255;
		//this->colorConst[6] = this->concatenatedColorTransform->blueOffset / 255;
	}
	else
	{
		this->colorConst[3] = this->concatenatedAlpha;
	}
}


void Object3D::copyAndAppend(Object3D *_arg_1, Object3D *_arg_2)
{
	this->matrix4.ma = (((_arg_2->matrix4.ma * _arg_1->matrix4.ma) + (_arg_2->matrix4.mb * _arg_1->matrix4.me)) + (_arg_2->matrix4.mc * _arg_1->matrix4.mi));
	this->matrix4.mb = (((_arg_2->matrix4.ma * _arg_1->matrix4.mb) + (_arg_2->matrix4.mb * _arg_1->matrix4.mf)) + (_arg_2->matrix4.mc * _arg_1->matrix4.mj));
	this->matrix4.mc = (((_arg_2->matrix4.ma * _arg_1->matrix4.mc) + (_arg_2->matrix4.mb * _arg_1->matrix4.mg)) + (_arg_2->matrix4.mc * _arg_1->matrix4.mk));
	this->matrix4.md = ((((_arg_2->matrix4.ma * _arg_1->matrix4.md) + (_arg_2->matrix4.mb * _arg_1->matrix4.mh)) + (_arg_2->matrix4.mc * _arg_1->matrix4.ml)) + _arg_2->matrix4.md);
	this->matrix4.me = (((_arg_2->matrix4.me * _arg_1->matrix4.ma) + (_arg_2->matrix4.mf * _arg_1->matrix4.me)) + (_arg_2->matrix4.mg * _arg_1->matrix4.mi));
	this->matrix4.mf = (((_arg_2->matrix4.me * _arg_1->matrix4.mb) + (_arg_2->matrix4.mf * _arg_1->matrix4.mf)) + (_arg_2->matrix4.mg * _arg_1->matrix4.mj));
	this->matrix4.mg = (((_arg_2->matrix4.me * _arg_1->matrix4.mc) + (_arg_2->matrix4.mf * _arg_1->matrix4.mg)) + (_arg_2->matrix4.mg * _arg_1->matrix4.mk));
	this->matrix4.mh = ((((_arg_2->matrix4.me * _arg_1->matrix4.md) + (_arg_2->matrix4.mf * _arg_1->matrix4.mh)) + (_arg_2->matrix4.mg * _arg_1->matrix4.ml)) + _arg_2->matrix4.mh);
	this->matrix4.mi = (((_arg_2->matrix4.mi * _arg_1->matrix4.ma) + (_arg_2->matrix4.mj * _arg_1->matrix4.me)) + (_arg_2->matrix4.mk * _arg_1->matrix4.mi));
	this->matrix4.mj = (((_arg_2->matrix4.mi * _arg_1->matrix4.mb) + (_arg_2->matrix4.mj * _arg_1->matrix4.mf)) + (_arg_2->matrix4.mk * _arg_1->matrix4.mj));
	this->matrix4.mk = (((_arg_2->matrix4.mi * _arg_1->matrix4.mc) + (_arg_2->matrix4.mj * _arg_1->matrix4.mg)) + (_arg_2->matrix4.mk * _arg_1->matrix4.mk));
	this->matrix4.ml = ((((_arg_2->matrix4.mi * _arg_1->matrix4.md) + (_arg_2->matrix4.mj * _arg_1->matrix4.mh)) + (_arg_2->matrix4.mk * _arg_1->matrix4.ml)) + _arg_2->matrix4.ml);
}
