#include "EllipsoidCollider.h"
#include "flash/geom/Vector3D.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "MyMath.h"
#include "Wrapper.h"
#include "Vertex.h"
#include "Face.h"
#include "Faces.h"



EllipsoidCollider::EllipsoidCollider(double radiusX, double radiusY, double radiusZ)
{
	this->matrix = new_Object3D();
	this->src = new Vector3D();
	this->displ = new Vector3D();
	this->dest = new Vector3D();
	this->collisionPoint = new Vector3D();
	this->collisionPlane = new Vector3D();
	this->vCenter = new Vector3D();
	this->vA = new Vector3D();
	this->vB = new Vector3D();
	this->vC = new Vector3D();
	this->vD = new Vector3D();
	this->radiusX = radiusX;
	this->radiusY = radiusY;
	this->radiusZ = radiusZ;
	this->threshold = 0.001;
	this->facesLength = 0;
	this->radius = 0;

	this->src->reset();
	this->displ->reset();
	this->dest->reset();
	this->collisionPoint->reset();
	this->collisionPlane->reset();
	this->vCenter->reset();
	this->vA->reset();
	this->vB->reset();
	this->vC ->reset();
	this->vD->reset();

}


bool EllipsoidCollider::getCollision(const Vector3D *param1,
									 const Vector3D *param2,
									 Vector3D *param3,
									 Vector3D *param4,
									 Object3D *param5)
{

	if (param2->getLength() <= this->threshold)
	{
		return false;
	}


	this->prepare(param1, param2, param5, nullptr, true);

	if (this->facesLength > 0)
	{
		if (this->checkCollision())
		{
			this->matrix->composeMatrix();
			param3->x = (this->matrix->matrix4.ma * this->collisionPoint->x + this->matrix->matrix4.mb * this->collisionPoint->y + this->matrix->matrix4.mc * this->collisionPoint->z + this->matrix->matrix4.md);
			param3->y = (this->matrix->matrix4.me * this->collisionPoint->x + this->matrix->matrix4.mf * this->collisionPoint->y + this->matrix->matrix4.mg * this->collisionPoint->z + this->matrix->matrix4.mh);
			param3->z = (this->matrix->matrix4.mi * this->collisionPoint->x + this->matrix->matrix4.mj * this->collisionPoint->y + this->matrix->matrix4.mk * this->collisionPoint->z + this->matrix->matrix4.ml);

			double _loc7_;
			double _loc8_;
			double _loc9_;
			if (this->collisionPlane->x < this->collisionPlane->y)
			{
				if (this->collisionPlane->x < this->collisionPlane->z)
				{
					_loc7_ = 0;
					_loc8_ = -this->collisionPlane->z;
					_loc9_ = this->collisionPlane->y;
				}
				else
				{
					_loc7_ = -this->collisionPlane->y;
					_loc8_ = this->collisionPlane->x;
					_loc9_ = 0;
				}
			}
			else if (this->collisionPlane->y < this->collisionPlane->z)
			{
				_loc7_ = this->collisionPlane->z;
				_loc8_ = 0;
				_loc9_ = -this->collisionPlane->x;
			}
			else
			{
				_loc7_ = -this->collisionPlane->y;
				_loc8_ = this->collisionPlane->x;
				_loc9_ = 0;
			}
			double _loc10_ = this->collisionPlane->z * _loc8_ - this->collisionPlane->y * _loc9_;
			double _loc11_ = this->collisionPlane->x * _loc9_ - this->collisionPlane->z * _loc7_;
			double _loc12_ = this->collisionPlane->y * _loc7_ - this->collisionPlane->x * _loc8_;
			double _loc13_ = this->matrix->matrix4.ma * _loc7_ + this->matrix->matrix4.mb * _loc8_ + this->matrix->matrix4.mc * _loc9_;
			double _loc14_ = this->matrix->matrix4.me * _loc7_ + this->matrix->matrix4.mf * _loc8_ + this->matrix->matrix4.mg * _loc9_;
			double _loc15_ = this->matrix->matrix4.mi * _loc7_ + this->matrix->matrix4.mj * _loc8_ + this->matrix->matrix4.mk * _loc9_;
			double _loc16_ = this->matrix->matrix4.ma * _loc10_ + this->matrix->matrix4.mb * _loc11_ + this->matrix->matrix4.mc * _loc12_;
			double _loc17_ = this->matrix->matrix4.me * _loc10_ + this->matrix->matrix4.mf * _loc11_ + this->matrix->matrix4.mg * _loc12_;
			double _loc18_ = this->matrix->matrix4.mi * _loc10_ + this->matrix->matrix4.mj * _loc11_ + this->matrix->matrix4.mk * _loc12_;
			param4->x = (_loc15_ * _loc17_ - _loc14_ * _loc18_);
			param4->y = (_loc13_ * _loc18_ - _loc15_ * _loc16_);
			param4->z = (_loc14_ * _loc16_ - _loc13_ * _loc17_);
			param4->normalize();
			param4->w = (param3->x * param4->x + param3->y * param4->y + param3->z * param4->z);
			this->faces.clear();
			facesLength = 0;
			return true;
		}

		this->faces.clear();
		facesLength = 0;
		return false;
	}

	return false;
}


void EllipsoidCollider::prepare(const Vector3D *param1, const Vector3D *param2, Object3D *param3, void *param4, bool param5)
{
	//Dictionary *param4

	this->radius = this->radiusX;

	if (this->radiusY > this->radius)
	{
		this->radius = this->radiusY;
	}

	if (this->radiusZ > this->radius)
	{
		this->radius = this->radiusZ;
	}

	this->matrix->scaleX = this->radiusX / this->radius;
	this->matrix->scaleY = this->radiusY / this->radius;
	this->matrix->scaleZ = this->radiusZ / this->radius;
	this->matrix->x = param1->x;
	this->matrix->y = param1->y;
	this->matrix->z = param1->z;
	this->matrix->composeMatrix();
	this->matrix->invertMatrix();
	this->src->x = 0;
	this->src->y = 0;
	this->src->z = 0;
	this->displ->setX(this->matrix->matrix4.ma * param2->getX() + this->matrix->matrix4.mb * param2->getY() + this->matrix->matrix4.mc * param2->getZ());
	this->displ->setY(this->matrix->matrix4.me * param2->getX() + this->matrix->matrix4.mf * param2->getY() + this->matrix->matrix4.mg * param2->getZ());
	this->displ->setZ(this->matrix->matrix4.mi * param2->getX() + this->matrix->matrix4.mj * param2->getY() + this->matrix->matrix4.mk * param2->getZ());
	this->dest->x = (this->src->x + this->displ->x);
	this->dest->y = (this->src->y + this->displ->y);
	this->dest->z = (this->src->z + this->displ->z);

	double v;


	if (param5)
	{
		this->vCenter->setX(this->displ->getX() / 2.);
		this->vCenter->setY(this->displ->getY() / 2.);
		this->vCenter->setZ(this->displ->getZ() / 2.);
		v = this->radius + this->displ->getLength() / 2.;
	}
	else
	{
		this->vCenter->setX(0);
		this->vCenter->setY(0);
		this->vCenter->setZ(0);
		v = this->radius + this->displ->getLength();
	}

	this->vA->reset(-v, -v, -v);
	this->vB->reset(v, -v, -v);
	this->vC->reset(v, v, -v);
	this->vD->reset(-v, v, -v);

	param3->composeAndAppend(this->matrix.get());

	Faces p;
	p.faces = &this->faces;

	param3->collectPlanes(this->vCenter, this->vA, this->vB, this->vC, this->vD, &p, param4);
	this->facesLength = this->faces.count();
}


bool EllipsoidCollider::checkCollision()
{
	double _local_2 = this->displ->getLength();

	double _local_1 = 1;

	for (int _local_3 = 0; _local_3 < this->facesLength; _local_3++)
	{
		Weak<Face> _local_4 = this->faces[_local_3];
		Shared<Wrapper> _local_5 = _local_4->wrapper;
		Shared<Vertex> _local_6 = _local_5->vertex;
		_local_5 = _local_5->next;
		Shared<Vertex> _local_7 = _local_5->vertex;
		_local_5 = _local_5->next;
		Shared<Vertex> _local_8 = _local_5->vertex;

		double _local_9 = (_local_7->cameraX - _local_6->cameraX);
		double _local_10 = (_local_7->cameraY - _local_6->cameraY);
		double _local_11 = (_local_7->cameraZ - _local_6->cameraZ);
		double _local_12 = (_local_8->cameraX - _local_6->cameraX);
		double _local_13 = (_local_8->cameraY - _local_6->cameraY);
		double _local_14 = (_local_8->cameraZ - _local_6->cameraZ);

		double _local_15 = ((_local_14 * _local_10) - (_local_13 * _local_11));
		double _local_16 = ((_local_12 * _local_11) - (_local_14 * _local_9));
		double _local_17 = ((_local_13 * _local_9) - (_local_12 * _local_10));
		double _local_18 = (((_local_15 * _local_15) + (_local_16 * _local_16)) + (_local_17 * _local_17));

		if (_local_18 > 0.001)
		{
			_local_18 = (1 / MyMath::sqrt(_local_18));
			_local_15 = (_local_15 * _local_18);
			_local_16 = (_local_16 * _local_18);
			_local_17 = (_local_17 * _local_18);
		}
		else
		{
			continue;
		}

		double _local_19 = (((_local_6->cameraX * _local_15) + (_local_6->cameraY * _local_16)) + (_local_6->cameraZ * _local_17));
		double _local_20 = ((((this->src->x * _local_15) + (this->src->y * _local_16)) + (this->src->z * _local_17)) - _local_19);

		double _local_21;
		double _local_22;
		double _local_23;

		if (_local_20 < this->radius)
		{
			_local_21 = this->src->x - (_local_15 * _local_20);
			_local_22 = this->src->y - (_local_16 * _local_20);
			_local_23 = this->src->z - (_local_17 * _local_20);
		}
		else
		{
			double v0 = _local_20 - this->radius;

			double v1 = this->dest->x * _local_15;
			double v2 = this->dest->y * _local_16;
			double v3 = this->dest->z * _local_17;

			double v5 = _local_20 - v1;
			double v7 = v5 - v2;
			double v8 = v7 - v3;
			double v10 = v8 + _local_19;

			double _local_33;

			if (v10 == 0)
			{
				_local_33 = 0;
				//continue;
			}
			else
			{
				_local_33 = (v0 / v10);
			}

			_local_21 = ((this->src->x + (this->displ->x * _local_33)) - (_local_15 * this->radius));
			_local_22 = ((this->src->y + (this->displ->y * _local_33)) - (_local_16 * this->radius));
			_local_23 = ((this->src->z + (this->displ->z * _local_33)) - (_local_17 * this->radius));
		}

		double _local_24 = 0; //MyMath::nan();
		double _local_25 = 0; //MyMath::nan();
		double _local_26 = 0; //MyMath::nan();

		bool valid = false;

		double _local_27 = 1E22;

		bool _local_28 = true;

		Shared<Wrapper> _local_29 = _local_4->wrapper;

		while (_local_29 != nullptr)
		{
			_local_6 = _local_29->vertex;
			_local_7 = ((_local_29->next != nullptr) ? _local_29->next->vertex : _local_4->wrapper->vertex);
			_local_9 = (_local_7->cameraX - _local_6->cameraX);
			_local_10 = (_local_7->cameraY - _local_6->cameraY);
			_local_11 = (_local_7->cameraZ - _local_6->cameraZ);
			_local_12 = (_local_21 - _local_6->cameraX);
			_local_13 = (_local_22 - _local_6->cameraY);
			_local_14 = (_local_23 - _local_6->cameraZ);
			double _local_34 = ((_local_14 * _local_10) - (_local_13 * _local_11));
			double _local_35 = ((_local_12 * _local_11) - (_local_14 * _local_9));
			double _local_36 = ((_local_13 * _local_9) - (_local_12 * _local_10));
			if ((((_local_34 * _local_15) + (_local_35 * _local_16)) + (_local_36 * _local_17)) < 0)
			{
				double _local_37 = (((_local_9 * _local_9) + (_local_10 * _local_10)) + (_local_11 * _local_11));
				double _local_38 = ((((_local_34 * _local_34) + (_local_35 * _local_35)) + (_local_36 * _local_36)) / _local_37);
				if (_local_38 < _local_27)
				{
					_local_37 = MyMath::sqrt(_local_37);
					_local_9 = (_local_9 / _local_37);
					_local_10 = (_local_10 / _local_37);
					_local_11 = (_local_11 / _local_37);
					double _local_33 = (((_local_9 * _local_12) + (_local_10 * _local_13)) + (_local_11 * _local_14));
					if (_local_33 < 0)
					{
						double _local_39 = (((_local_12 * _local_12) + (_local_13 * _local_13)) + (_local_14 * _local_14));
						if (_local_39 < _local_27)
						{
							_local_27 = _local_39;
							_local_24 = _local_6->cameraX;
							_local_25 = _local_6->cameraY;
							_local_26 = _local_6->cameraZ;
							valid = true;
						}
					}
					else
					{
						if (_local_33 > _local_37)
						{
							_local_12 = (_local_21 - _local_7->cameraX);
							_local_13 = (_local_22 - _local_7->cameraY);
							_local_14 = (_local_23 - _local_7->cameraZ);
							double _local_39 = (((_local_12 * _local_12) + (_local_13 * _local_13)) + (_local_14 * _local_14));
							if (_local_39 < _local_27)
							{
								_local_27 = _local_39;
								_local_24 = _local_7->cameraX;
								_local_25 = _local_7->cameraY;
								_local_26 = _local_7->cameraZ;
								valid = true;
							}
						}
						else
						{
							_local_27 = _local_38;
							_local_24 = (_local_6->cameraX + (_local_9 * _local_33));
							_local_25 = (_local_6->cameraY + (_local_10 * _local_33));
							_local_26 = (_local_6->cameraZ + (_local_11 * _local_33));
							valid = true;
						}
					}
				}
				_local_28 = false;
			}
			_local_29 = _local_29->next;
		}

		if (_local_28)
		{
			_local_24 = _local_21;
			_local_25 = _local_22;
			_local_26 = _local_23;
			valid = true;
		}

		if (valid == false)
		{
			continue;
		}


		double _local_30 = (this->src->x - _local_24);
		double _local_31 = (this->src->y - _local_25);
		double _local_32 = (this->src->z - _local_26);

		if ((((_local_30 * this->displ->x) + (_local_31 * this->displ->y)) + (_local_32 * this->displ->z)) <= 0)
		{
			double _local_40 = (-(this->displ->x) / _local_2);
			double _local_41 = (-(this->displ->y) / _local_2);
			double _local_42 = (-(this->displ->z) / _local_2);
			double _local_43 = (((_local_30 * _local_30) + (_local_31 * _local_31)) + (_local_32 * _local_32));
			double _local_44 = (((_local_30 * _local_40) + (_local_31 * _local_41)) + (_local_32 * _local_42));
			double _local_45 = (((this->radius * this->radius) - _local_43) + (_local_44 * _local_44));

			if (_local_45 > 0)
			{
				double _local_46 = ((_local_44 - MyMath::sqrt(_local_45)) / _local_2);
				if (_local_46 < _local_1)
				{
					_local_1 = _local_46;
					this->collisionPoint->x = _local_24;
					this->collisionPoint->y = _local_25;
					this->collisionPoint->z = _local_26;

					if (_local_28)
					{
						this->collisionPlane->x = _local_15;
						this->collisionPlane->y = _local_16;
						this->collisionPlane->z = _local_17;
						this->collisionPlane->w = _local_19;
					}
					else
					{
						if (_local_43 == 0)
						{
							this->collisionPlane->reset();
						}
						else
						{

							_local_43 = MyMath::sqrt(_local_43);
							this->collisionPlane->x = _local_30 / _local_43;
							this->collisionPlane->y = _local_31 / _local_43;
							this->collisionPlane->z = _local_32 / _local_43;
						}


						this->collisionPlane->w =
								(this->collisionPoint->x * this->collisionPlane->x) +
								(this->collisionPoint->y * this->collisionPlane->y) +
								(this->collisionPoint->z * this->collisionPlane->z);
					}
				}
			}
		}
	}

	return _local_1 < 1;
}
