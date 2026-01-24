#include "PolygonsIntersectionUtils.h"
#include "game/alternativa/physics/collision/CollisionShape.h"
#include "game/alternativa/physics/ShapeContact.h"
#include "CollidersVertex.h"
#include "flash/geom/Point.h"


static void calculateFaceNormal(const CollidersVertex *param1, Vector3 *param2)
{
	const CollidersVertex *_loc3_ = &param1[0];
	const CollidersVertex *_loc4_ = &param1[1];
	const CollidersVertex *_loc5_ = &param1[2];
	const Vector3 *_loc6_ = &_loc3_->transformed;
	const Vector3 *_loc7_ = &_loc4_->transformed;
	const Vector3 *_loc8_ = &_loc5_->transformed;
	double _loc9_ = _loc7_->getX() - _loc6_->getX();
	double _loc10_ = _loc7_->getY() - _loc6_->getY();
	double _loc11_ = _loc7_->getZ() - _loc6_->getZ();
	double _loc12_ = _loc8_->getX() - _loc6_->getX();
	double _loc13_ = _loc8_->getY() - _loc6_->getY();
	double _loc14_ = _loc8_->getZ() - _loc6_->getZ();
	param2->setX(_loc10_ * _loc14_ - _loc11_ * _loc13_);
	param2->setY(_loc11_ * _loc12_ - _loc9_ * _loc14_);
	param2->setZ(_loc9_ * _loc13_ - _loc10_ * _loc12_);
	param2->normalize();
}


static void fillProjectedPoints(CollidersVertex *colliders_vertex_list, int count, Point *points)
{
	for (int i = 0; i < count; i++)
	{
		CollidersVertex *collidersVertex = &colliders_vertex_list[i];
		Point *point = &points[i];
		point->x = collidersVertex->transformed.getX();
		point->y = collidersVertex->transformed.getY();
	}
}


static void copyPoints(const Point *src, Point *dest, int count)
{
	for (int i = 0; i < count; i++)
	{
		const Point *psrc = &src[i];
		Point *pdest = &dest[i];
		pdest->x = psrc->x;
		pdest->y = psrc->y;
	}
}


static bool arePointsCCW(const Point *param1, const Point *param2, const Point *param3)
{
	double _loc4_ = param2->x - param1->x;
	double _loc5_ = param2->y - param1->y;
	double _loc6_ = param3->x - param1->x;
	double _loc7_ = param3->y - param1->y;
	return _loc4_ * _loc7_ - _loc5_ * _loc6_ > 0;
}


void static calculateIntersection(const Point *param1, const Point *param2, const Point *param3, const Point *param4, Point *param5)
{
	double _loc6_ = param2->x - param1->x;
	double _loc7_ = param2->y - param1->y;
	double _loc8_ = param4->x - param3->x;
	double _loc9_ = param4->y - param3->y;
	double _loc10_ = param3->x - param1->x;
	double _loc11_ = param3->y - param1->y;
	double _loc12_ = ((_loc6_ * _loc11_) - (_loc7_ * _loc10_)) / ((_loc7_ * _loc8_) - (_loc6_ * _loc9_));
	param5->x = param3->x + _loc12_ * _loc8_;
	param5->y = param3->y + _loc12_ * _loc9_;
}


static void setPoint(const Point *src, Point *dest, int index)
{
	Point *point = &dest[index];
	point->x = src->x;
	point->y = src->y;
}


static int clip(const Point *param1, const Point *param2, const Point *param3, Point *param4, int max)
{

	Point const* _loc6_ = &param3[max - 1];
	bool _loc7_ = arePointsCCW(param1, param2, _loc6_);
	int count = 0;
	for (int i = 0; i < max; i++)
	{
		Point const* _loc10_ = &param3[i];
		bool _loc11_ = arePointsCCW(param1, param2, _loc10_);
		if (_loc11_)
		{
			if (!_loc7_)
			{
				Point point;
				calculateIntersection(param1, param2, _loc6_, _loc10_, &point);
				setPoint(&point, param4, count++);
			}
			setPoint(_loc10_, param4, count++);
		}
		else if (_loc7_)
		{
			Point point;
			calculateIntersection(param1, param2, _loc6_, _loc10_, &point);
			setPoint(&point, param4, count++);
		}
		_loc7_ = _loc11_;
		_loc6_ = _loc10_;
	}
	return count;
}


static int findPolygonsIntersection(Point *param1, int count, Point *param3, int param4, Point *param5)
{
	Point _points1[8];
	Point _points2[8];

	Point *points1 = _points1;
	Point *points2 = _points2;

	int c = param4;
	copyPoints(param3, points1, c);

	Point *p = &param1[count - 1];
	for (int i = 0; i < count; i++)
	{
		Point *p2 = &param1[i];
		c = clip(p, p2, points1, points2, c);
		if (c == 0)
			break;

		p = p2;
		Point *temp = points1;
		points1 = points2;
		points2 = temp;
	}

	copyPoints(points1, param5, c);
	return c;
}


static double getFaceZ(Point *param1, Vector3 *param2, Vector3 *param3)
{
	double v = param2->dot(param3);
	return ((v - param1->x * param3->getX()) - (param1->y * param3->getY())) / param3->getZ();
}


void PolygonsIntersectionUtils::findContacts(CollisionShape *param1, CollidersVertex *param2, int param3, CollisionShape *param4,
											 CollidersVertex *param5, int param6, Matrix4 *param7, MyList<ShapeContact*> *param8)
{
	Point intersection[8];
	Point projectedPoints1[8];
	Point projectedPoints2[8];
	Vector3 normal1;
	Vector3 normal2;

	calculateFaceNormal(param2, &normal1);
	calculateFaceNormal(param5, &normal2);


	fillProjectedPoints(param2, param3, projectedPoints1);
	fillProjectedPoints(param5, param6, projectedPoints2);

	int _loc9_ = findPolygonsIntersection(projectedPoints1, param3, projectedPoints2, param6, intersection);
	int _loc10_ = 0;

	while (_loc10_ < _loc9_)
	{
		Point *_loc11_ = &intersection[_loc10_];
		double _loc12_ = getFaceZ(_loc11_, &param2[0].transformed, &normal1);
		double _loc13_ = getFaceZ(_loc11_, &param5[0].transformed, &normal2);
		if (_loc13_ > _loc12_)
		{
			ShapeContact *_loc14_ = ShapeContact::create();
			_loc14_->shape1 = param1;
			_loc14_->shape2 = param4;
			Vector3 *_loc15_ = &_loc14_->position;
			_loc15_->setX(_loc11_->x);
			_loc15_->setY(_loc11_->y);
			_loc15_->setZ(0.5 * (_loc12_ + _loc13_));
			_loc15_->transform4(param7);
			_loc14_->penetration = _loc13_ - _loc12_;
			_loc14_->normal.setX(param7->m02);
			_loc14_->normal.setY(param7->m12);
			_loc14_->normal.setZ(param7->m22);
			param8->append(_loc14_);
		}
		_loc10_++;
	}
}
