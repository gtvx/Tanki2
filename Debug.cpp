#include "Debug.h"
#include "Canvas.h"
#include "Graphics.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include "hardware/alternativa/engine3d/core/Wrapper.h"
#include "game/alternativa/physics/collision/types/AABB.h"
#include "game/projects/tanks/client/battlefield/types/Vector3d.h"
#include "game/alternativa/math/Vector3.h"
#include "MyMath.h"


namespace
{
	struct LocalVertex
	{
		double x, y, z;
		double cameraX, cameraY, cameraZ;
	};
}


void Debug_drawBounds(Canvas *canvas, Bound *b, double viewSizeX, double viewSizeY, const Matrix4 *m, uint32_t color)
{
	LocalVertex boundVertexList[8];


	LocalVertex *_loc12_ = &boundVertexList[0];
	_loc12_->x = b->boundMinX;
	_loc12_->y = b->boundMinY;
	_loc12_->z = b->boundMinZ;

	LocalVertex *_loc13_ = &boundVertexList[1];
	_loc13_->x = b->boundMaxX;
	_loc13_->y = b->boundMinY;
	_loc13_->z = b->boundMinZ;

	LocalVertex *_loc14_ = &boundVertexList[2];
	_loc14_->x = b->boundMinX;
	_loc14_->y = b->boundMaxY;
	_loc14_->z = b->boundMinZ;

	LocalVertex *_loc15_ = &boundVertexList[3];
	_loc15_->x = b->boundMaxX;
	_loc15_->y = b->boundMaxY;
	_loc15_->z = b->boundMinZ;

	LocalVertex *_loc16_ = &boundVertexList[4];
	_loc16_->x = b->boundMinX;
	_loc16_->y = b->boundMinY;
	_loc16_->z = b->boundMaxZ;

	LocalVertex *_loc17_ = &boundVertexList[5];
	_loc17_->x = b->boundMaxX;
	_loc17_->y = b->boundMinY;
	_loc17_->z = b->boundMaxZ;

	LocalVertex *_loc18_ = &boundVertexList[6];
	_loc18_->x = b->boundMinX;
	_loc18_->y = b->boundMaxY;
	_loc18_->z = b->boundMaxZ;

	LocalVertex *_loc19_ = &boundVertexList[7];
	_loc19_->x = b->boundMaxX;
	_loc19_->y = b->boundMaxY;
	_loc19_->z = b->boundMaxZ;


	for (int i = 0; i < 8; i++)
	{
		LocalVertex *v = &boundVertexList[i];

		v->cameraX = (m->ma * v->x) + (m->mb * v->y) + (m->mc * v->z) + m->md;
		v->cameraY = (m->me * v->x) + (m->mf * v->y) + (m->mg * v->z) + m->mh;
		v->cameraZ = (m->mi * v->x) + (m->mj * v->y) + (m->mk * v->z) + m->ml;
		if (v->cameraZ <= 0)
		{
			return;
		}
	}


	for (int i = 0; i < 8; i++)
	{
		LocalVertex *v = &boundVertexList[i];

		double _loc23_ = 1 / v->cameraZ;
		v->cameraX = v->cameraX * viewSizeX * _loc23_;
		v->cameraY = v->cameraY * viewSizeY * _loc23_;
	}

	canvas->gfx->moveTo(_loc12_->cameraX, _loc12_->cameraY);
	canvas->gfx->lineTo(_loc13_->cameraX, _loc13_->cameraY);
	canvas->gfx->lineTo(_loc15_->cameraX, _loc15_->cameraY);
	canvas->gfx->lineTo(_loc14_->cameraX, _loc14_->cameraY);
	canvas->gfx->lineTo(_loc12_->cameraX, _loc12_->cameraY);


	canvas->gfx->moveTo(_loc16_->cameraX, _loc16_->cameraY);
	canvas->gfx->lineTo(_loc17_->cameraX, _loc17_->cameraY);
	canvas->gfx->lineTo(_loc19_->cameraX, _loc19_->cameraY);
	canvas->gfx->lineTo(_loc18_->cameraX, _loc18_->cameraY);
	canvas->gfx->lineTo(_loc16_->cameraX, _loc16_->cameraY);

	canvas->gfx->moveTo(_loc12_->cameraX, _loc12_->cameraY);
	canvas->gfx->lineTo(_loc16_->cameraX, _loc16_->cameraY);

	canvas->gfx->moveTo(_loc13_->cameraX, _loc13_->cameraY);
	canvas->gfx->lineTo(_loc17_->cameraX, _loc17_->cameraY);

	canvas->gfx->moveTo(_loc15_->cameraX, _loc15_->cameraY);
	canvas->gfx->lineTo(_loc19_->cameraX, _loc19_->cameraY);

	canvas->gfx->moveTo(_loc14_->cameraX, _loc14_->cameraY);
	canvas->gfx->lineTo(_loc18_->cameraX, _loc18_->cameraY);


	canvas->gfx->end(color);
}


void Debug_drawRect(Canvas *canvas, Bound *b, double viewSizeX, double viewSizeY, const Matrix4 *m, uint32_t color, int *z)
{
	*z = -1;

	LocalVertex boundVertexList[4];


	LocalVertex *v1 = &boundVertexList[0];
	v1->x = b->boundMinX;
	v1->y = b->boundMinY;
	v1->z = b->boundMinZ;

	LocalVertex *v2 = &boundVertexList[1];
	v2->x = b->boundMaxX;
	v2->y = b->boundMinY;
	v2->z = b->boundMinZ;

	LocalVertex *v3 = &boundVertexList[2];
	v3->x = b->boundMinX;
	v3->y = b->boundMaxY;
	v3->z = b->boundMinZ;

	LocalVertex *v4 = &boundVertexList[3];
	v4->x = b->boundMaxX;
	v4->y = b->boundMaxY;
	v4->z = b->boundMinZ;

	for (int i = 0; i < 4; i++)
	{
		LocalVertex *v = &boundVertexList[i];

		v->cameraX = (m->ma * v->x) + (m->mb * v->y) + (m->mc * v->z) + m->md;
		v->cameraY = (m->me * v->x) + (m->mf * v->y) + (m->mg * v->z) + m->mh;
		v->cameraZ = (m->mi * v->x) + (m->mj * v->y) + (m->mk * v->z) + m->ml;
		if (v->cameraZ <= 0)
		{
			return;
		}
		//qDebug() << toString(v->cameraX, v->cameraY, v->cameraZ);
	}


	double centerX =  m->ma + m->mb + m->mc + m->md;
	double centerY =  m->me + m->mf + m->mg + m->mh;
	double centerZ =  m->mi + m->mj + m->mk + m->ml;

	{
		double b = 1. / centerZ;
		centerX *= viewSizeX * b;
		centerY *= viewSizeY * b;
	}

	for (int i = 0; i < 4; i++)
	{
		LocalVertex *v = &boundVertexList[i];
		double b = 1. / v->cameraZ;
		v->cameraX *= viewSizeX * b;
		v->cameraY *= viewSizeY * b;
	}


	//qDebug() << a->cameraX <<  a->cameraY;
	canvas->gfx->moveTo(v1->cameraX, v1->cameraY);
	//canvas->gfx->drawString("1", a->cameraX, a->cameraY);

	canvas->gfx->lineTo(v2->cameraX, v2->cameraY);
	//canvas->gfx->drawString("2", _loc13_->cameraX, _loc13_->cameraY);

	canvas->gfx->lineTo(v4->cameraX, v4->cameraY);
	//canvas->gfx->drawString("3", _loc15_->cameraX, _loc15_->cameraY);

	canvas->gfx->lineTo(v3->cameraX, v3->cameraY);
	//canvas->gfx->drawString("4", _loc14_->cameraX, _loc14_->cameraY);

	canvas->gfx->lineTo(v1->cameraX, v1->cameraY);
	//canvas->gfx->drawString("5", v1->cameraX, v1->cameraY);


	int left = v1->cameraX;
	int right = v1->cameraX;

	int up = v1->cameraY;
	int down = v1->cameraY;

	for (int i = 1; i < 4; i++)
	{
		LocalVertex *v = &boundVertexList[i];
		const int x = v->cameraX;
		const int y = v->cameraY;

		if (left > x)
			left = x;
		if (right < x)
			right = x;

		if (up > y)
			up = y;
		if (down < y)
			down = y;
	}

	canvas->gfx->end(color);
}




static bool get(const Vector3d *v, double viewSizeX, double viewSizeY, const Matrix4 *m, double *_x, double *_y)
{
	double X = (m->ma * v->getX()) + (m->mb * v->getY()) + (m->mc * v->getZ()) + m->md;
	double Y = (m->me * v->getX()) + (m->mf * v->getY()) + (m->mg * v->getZ()) + m->mh;
	double Z = (m->mi * v->getX()) + (m->mj * v->getY()) + (m->mk * v->getZ()) + m->ml;
	if (Z <= 0) {
		return false;
	}

	double c = 1. / Z;
	*_x = X * viewSizeX * c;
	*_y = Y * viewSizeY * c;
	return true;
}


static bool get(const Vector3 *v, double viewSizeX, double viewSizeY, const Matrix4 *m, double *_x, double *_y)
{
	double X = (m->ma * v->getX()) + (m->mb * v->getY()) + (m->mc * v->getZ()) + m->md;
	double Y = (m->me * v->getX()) + (m->mf * v->getY()) + (m->mg * v->getZ()) + m->mh;
	double Z = (m->mi * v->getX()) + (m->mj * v->getY()) + (m->mk * v->getZ()) + m->ml;
	if (Z <= 0) {
		return false;
	}

	double c = 1. / Z;
	*_x = X * viewSizeX * c;
	*_y = Y * viewSizeY * c;
	return true;
}


void Debug_draw_point(Canvas *canvas, double viewSizeX, double viewSizeY, const Matrix4 *m, Wrapper *wrapper, uint32_t color)
{
	bool first = true;
	while (wrapper != nullptr) {
		Vertex *vertex = wrapper->vertex.get();

		Vector3d v;
		v.setX(vertex->x);
		v.setY(vertex->y);
		v.setZ(vertex->z);
		double X, Y;
		if (!get(&v, viewSizeX, viewSizeY, m, &X, &Y)) {
			wrapper = wrapper->next.get();
			continue;
		}

		if (first == true) {
			canvas->gfx->moveTo(X, Y);
			first = false;
		} else {
			canvas->gfx->lineTo(X, Y);
		}

		wrapper = wrapper->next.get();
	}

	canvas->gfx->end(color);
}


void Debug_draw_point(Canvas *canvas, double viewSizeX, double viewSizeY, const Matrix4 *m, const Vector3 *v, uint32_t color)
{
	Vector3 v0(0, 0, 0);

	double X1, Y1;
	double X2, Y2;

	if (!get(&v0, viewSizeX, viewSizeY, m, &X1, &Y1))
		return;

	if (!get(v, viewSizeX, viewSizeY, m, &X2, &Y2))
		return;

	canvas->gfx->moveTo(X1, Y1);
	canvas->gfx->lineTo(X2, Y2);


	canvas->gfx->end(color);
}


void Debug_draw_point(Canvas *canvas, double viewSizeX, double viewSizeY, const Matrix4 *m, const Vector3 *a,const Vector3 *b, uint32_t color)
{
	double X1, Y1;
	double X2, Y2;

	if (!get(a, viewSizeX, viewSizeY, m, &X1, &Y1))
		return;

	if (!get(b, viewSizeX, viewSizeY, m, &X2, &Y2))
		return;

	canvas->gfx->moveTo(X1, Y1);
	canvas->gfx->lineTo(X2, Y2);

	canvas->gfx->end(color);
}

//static QString s0("0");
//static QString s1("1");
//static QString s2("2");

void Debug_drawTriangle(Canvas *canvas, Vector3 *v0, Vector3 *v1, Vector3 *v2, double viewSizeX, double viewSizeY, const Matrix4 *m, uint32_t color)
{
	double X0, Y0;
	double X1, Y1;
	double X2, Y2;

	if (!get(v0, viewSizeX, viewSizeY, m, &X0, &Y0))
		return;

	if (!get(v1, viewSizeX, viewSizeY, m, &X1, &Y1))
		return;

	if (!get(v2, viewSizeX, viewSizeY, m, &X2, &Y2))
		return;

	canvas->gfx->moveTo(X0, Y0);
	canvas->gfx->lineTo(X1, Y1);
	canvas->gfx->lineTo(X2, Y2);
	canvas->gfx->lineTo(X0, Y0);

	//canvas->gfx->drawString(s0, X0, Y0);
	//canvas->gfx->drawString(s1, X1 + 10, Y1);
	//canvas->gfx->drawString(s2, X2 + 20, Y2);

	canvas->gfx->end(color);
}

