#include "KDContainer.h"
#include "KDNode.h"
#include "flash/geom/Vector3D.h"
#include "../core/Object3D.h"
#include "../objects/Occluder.h"
#include "Object3D_isType.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "hardware/alternativa/engine3d/objects/BSP.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "Error.h"
#include "hardware/alternativa/engine3d/materials/Material.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include "hardware/alternativa/engine3d/objects/BSP.h"
#include "../containers/KDNode.h"
#include "../core/Object3DContainer.h"
#include "../core/Camera3D.h"
#include "../core/View.h"
#include "hardware/alternativa/gfx/core/VertexBufferResource.h"
#include "hardware/alternativa/gfx/core/IndexBufferResource.h"
#include <QMap>
#include "../core/VG.h"
#include "../objects/Decal.h"
#include <QHash>
#include "MyMath.h"
#include "_global.h"
#include "flash/geom/Matrix3D.h"
#include "qhash_shared_ptr.h"


std::shared_ptr<KDContainer> new_KDContainer()
{
	return std::make_shared<KDContainer>();
}


KDContainer::KDContainer()
{
	ignoreChildrenInCollider = false;
	transparentLength = 0;
	root = nullptr;
	batched = true;
	numOccluders = 0;

	name_class = "KDContainer";

	boundVertexList = Vertex::createList(8);
}


void KDContainer::collectPlanes(const Vector3D *param1, const Vector3D *param2, const Vector3D *param3,
								const Vector3D *param4, const Vector3D *param5, Faces *param6, void *param7)
{
	//QVector<Face*> *param6, Dictionary param7 = nullptr
	QMap<Object3D*, void*> *_param7 = (QMap<Object3D*, void*>*)param7;
	if (_param7 != nullptr && _param7->contains(this))
	{
		return;
	}

	Vector3D _loc8_;
	calculateSphere(param1, param2, param3, param4, param5, &_loc8_);


	if (!this->ignoreChildrenInCollider)
	{
		if (!boundIntersectSphere(&_loc8_, bound.boundMinX, bound.boundMinY, bound.boundMinZ, bound.boundMaxX, bound.boundMaxY, bound.boundMaxZ))
		{
			return;
		}

		std::shared_ptr<Object3D> _loc9_ = childrenList;
		while (_loc9_ != nullptr)
		{


			_loc9_->composeAndAppend(this);
			_loc9_->collectPlanes(param1, param2, param3, param4, param5, param6, param7);
			_loc9_ = _loc9_->next;
		}
	}


	if (this->root != nullptr && boundIntersectSphere(&_loc8_,
													 this->root->boundMinX,
													 this->root->boundMinY,
													 this->root->boundMinZ,
													 this->root->boundMaxX,
													 this->root->boundMaxY,
													 this->root->boundMaxZ))
	{
		this->collectPlanesNode(this->root.get(), &_loc8_, param1, param2, param3, param4, param5, param6, param7);
	}
}


void KDContainer::collectPlanesNode(KDNode *param1, const Vector3D *param2, const Vector3D *param3, const Vector3D *param4,
									const Vector3D *param5, const Vector3D *param6, const Vector3D *param7, Faces *param8,  void *param9)
{
	//QVector<Face*> *param8
	//Dictionary *param9 = null

	int count = 0;
	std::shared_ptr<Object3D> _loc10_ = param1->objectList;
	while (_loc10_ != nullptr)
	{
		count++;
		_loc10_ = _loc10_->next;
	}


	if (param1->negative() != nullptr)
	{
		bool _loc12_ = param1->axis == 0;
		bool _loc13_ = param1->axis == 1;

		double _loc14_;

		if (_loc12_ == true)
		{
			_loc14_ = param2->x;
		} else if (_loc13_ == true)
		{
			_loc14_ = param2->y;
		} else {
			_loc14_ = param2->z;
		}

		_loc14_ -= param1->coord;

		//double _loc14_ = (!!_loc12_ ? param2->x : !!_loc13_ ? param2->y : param2->z) - param1->coord;

		if (_loc14_ >= param2->getW())
		{
			if (boundIntersectSphere(param2,
									 param1->positive()->boundMinX,
									 param1->positive()->boundMinY,
									 param1->positive()->boundMinZ,
									 param1->positive()->boundMaxX,
									 param1->positive()->boundMaxY,
									 param1->positive()->boundMaxZ))
			{
				this->collectPlanesNode(param1->positive(), param2, param3, param4, param5, param6, param7, param8, param9);
			}
		}
		else if (_loc14_ <= -param2->w)
		{
			if (boundIntersectSphere(param2,
									 param1->negative()->boundMinX,
									 param1->negative()->boundMinY,
									 param1->negative()->boundMinZ,
									 param1->negative()->boundMaxX,
									 param1->negative()->boundMaxY,
									 param1->negative()->boundMaxZ))
			{
				this->collectPlanesNode(param1->negative(), param2, param3, param4, param5, param6, param7, param8, param9);
			}
		}
		else
		{
			std::shared_ptr<Object3D> _loc10_ = param1->objectList;
			std::shared_ptr<Object3D> _loc11_ = param1->objectBoundList;

			while (_loc10_ != nullptr)
			{
				if (boundIntersectSphere(param2,
										_loc11_->bound.boundMinX,
										_loc11_->bound.boundMinY,
										_loc11_->bound.boundMinZ,
										_loc11_->bound.boundMaxX,
										_loc11_->bound.boundMaxY,
										_loc11_->bound.boundMaxZ))
				{
					_loc10_->composeAndAppend(this);
					_loc10_->collectPlanes(param3, param4, param5, param6, param7, param8, param9);
				}

				_loc10_ = _loc10_->next;
				_loc11_ = _loc11_->next;
			}

			if (boundIntersectSphere(param2,
									param1->positive()->boundMinX,
									param1->positive()->boundMinY,
									param1->positive()->boundMinZ,
									param1->positive()->boundMaxX,
									param1->positive()->boundMaxY,
									param1->positive()->boundMaxZ))
			{
				this->collectPlanesNode(param1->positive(), param2, param3, param4, param5, param6, param7, param8, param9);
			}

			if (boundIntersectSphere(param2,
									 param1->negative()->boundMinX,
									 param1->negative()->boundMinY,
									 param1->negative()->boundMinZ,
									 param1->negative()->boundMaxX,
									 param1->negative()->boundMaxY,
									 param1->negative()->boundMaxZ))
			{
				this->collectPlanesNode(param1->negative(), param2, param3, param4, param5, param6, param7, param8, param9);
			}
		}
	}
	else
	{
		std::shared_ptr<Object3D> o = param1->objectList;
		while (o != nullptr)
		{
			o->composeAndAppend(this);
			o->collectPlanes(param3, param4, param5, param6, param7, param8, param9);
			o = o->next;
		}
	}
}


void BoundBox_infinity(Bound *bound)
{
	bound->boundMinX = 1E308;
	bound->boundMinY = 1E308;
	bound->boundMinZ = 1E308;
	bound->boundMaxX = -1E308;
	bound->boundMaxY = -1E308;
	bound->boundMaxZ = -1E308;
}


static std::shared_ptr<Object3D> createObjectBounds(Object3D *src)
{
	std::shared_ptr<Object3D> result = new_Object3D();
	BoundBox_infinity(&result->bound);
	src->composeMatrix();
	src->updateBounds(result.get(), src);
	result->matrix4.copy(&src->matrix4);
	return result;
}


std::shared_ptr<KDNode> createNode(std::shared_ptr<Object3D> _arg_1,
								   std::shared_ptr<Object3D> _arg_2,
								   std::shared_ptr<Object3D> _arg_3,
								   std::shared_ptr<Object3D> _arg_4,
								   double _arg_5,
								   double _arg_6,
								   double _arg_7,
								   double _arg_8,
								   double _arg_9,
								   double _arg_10,
								   const double threshold)
{
	auto _local_11 = std::make_shared<KDNode>();

	_local_11->boundMinX = _arg_5;
	_local_11->boundMinY = _arg_6;
	_local_11->boundMinZ = _arg_7;
	_local_11->boundMaxX = _arg_8;
	_local_11->boundMaxY = _arg_9;
	_local_11->boundMaxZ = _arg_10;

	if (_arg_1 == nullptr)
	{
		if (_arg_3 != nullptr)
		{

		}


		return _local_11;
	}

	int _local_17 = 0;
	int _local_18 = 0;
	int _local_19 = 0;

	double _local_21 = NaN();
	//double _local_44 = 0;


	std::shared_ptr<Object3D> _local_29;
	std::shared_ptr<Object3D> _local_30;
	std::shared_ptr<Object3D> _local_31;
	std::shared_ptr<Object3D> _local_32;
	std::shared_ptr<Object3D> _local_33;
	std::shared_ptr<Object3D> _local_34;
	std::shared_ptr<Object3D> _local_35;
	std::shared_ptr<Object3D> _local_36;

	std::shared_ptr<Object3D> _local_15 = _arg_2;


	QVector<double> splitCoordsX;
	QVector<double> splitCoordsY;
	QVector<double> splitCoordsZ;


	while (_local_15 != nullptr)
	{
		if (_local_15->bound.boundMinX > (_arg_5 + threshold))
		{
			int _local_13 = 0;
			while (_local_13 < _local_17)
			{
				if (((_local_15->bound.boundMinX >= (splitCoordsX[_local_13] - threshold)) && (_local_15->bound.boundMinX <= (splitCoordsX[_local_13] + threshold)))) break;
				_local_13++;
			}
			if (_local_13 == _local_17)
			{
				_local_17++;
				splitCoordsX.append(_local_15->bound.boundMinX);
			}
		}

		if (_local_15->bound.boundMaxX < (_arg_8 - threshold))
		{
			int _local_13 = 0;
			while (_local_13 < _local_17)
			{
				if (((_local_15->bound.boundMaxX >= (splitCoordsX[_local_13] - threshold)) && (_local_15->bound.boundMaxX <= (splitCoordsX[_local_13] + threshold)))) break;
				_local_13++;
			}
			if (_local_13 == _local_17)
			{
				_local_17++;
				splitCoordsX.append(_local_15->bound.boundMaxX);
			}
		}

		if (_local_15->bound.boundMinY > (_arg_6 + threshold))
		{
			int _local_13 = 0;
			while (_local_13 < _local_18)
			{
				if (((_local_15->bound.boundMinY >= (splitCoordsY[_local_13] - threshold)) && (_local_15->bound.boundMinY <= (splitCoordsY[_local_13] + threshold)))) break;
				_local_13++;
			}
			if (_local_13 == _local_18)
			{
				_local_18++;
				splitCoordsY.append(_local_15->bound.boundMinY);
			}
		}

		if (_local_15->bound.boundMaxY < (_arg_9 - threshold))
		{
			int _local_13 = 0;
			while (_local_13 < _local_18)
			{
				if (((_local_15->bound.boundMaxY >= (splitCoordsY[_local_13] - threshold)) && (_local_15->bound.boundMaxY <= (splitCoordsY[_local_13] + threshold)))) break;
				_local_13++;
			}
			if (_local_13 == _local_18)
			{
				_local_18++;
				splitCoordsY.append(_local_15->bound.boundMaxY);
			}
		}

		if (_local_15->bound.boundMinZ > (_arg_7 + threshold))
		{
			int _local_13 = 0;
			while (_local_13 < _local_19)
			{
				if (((_local_15->bound.boundMinZ >= (splitCoordsZ[_local_13] - threshold)) && (_local_15->bound.boundMinZ <= (splitCoordsZ[_local_13] + threshold)))) break;
				_local_13++;
			}
			if (_local_13 == _local_19)
			{
				_local_19++;
				splitCoordsZ.append(_local_15->bound.boundMinZ);
			}
		}

		if (_local_15->bound.boundMaxZ < (_arg_10 - threshold))
		{
			int _local_13 = 0;
			while (_local_13 < _local_19)
			{
				if (((_local_15->bound.boundMaxZ >= (splitCoordsZ[_local_13] - threshold)) && (_local_15->bound.boundMaxZ <= (splitCoordsZ[_local_13] + threshold)))) break;
				_local_13++;
			}
			if (_local_13 == _local_19)
			{
				_local_19++;
				splitCoordsZ.append(_local_15->bound.boundMaxZ);
			}
		}
		_local_15 = _local_15->next;
	}

	int _local_20 = -1;
	double _local_22 = 1E22;
	double _local_25 = ((_arg_9 - _arg_6) * (_arg_10 - _arg_7));
	int _local_12 = 0;
	while (_local_12 < _local_17)
	{
		double _local_16 = splitCoordsX[_local_12];
		double _local_26 = (_local_25 * (_local_16 - _arg_5));
		double _local_27 = (_local_25 * (_arg_8 - _local_16));
		int _local_23 = 0;
		int _local_24 = 0;
		std::shared_ptr<Object3D> _local_15 = _arg_2;
		while (_local_15 != nullptr)
		{
			if (_local_15->bound.boundMaxX <= (_local_16 + threshold))
			{
				if (_local_15->bound.boundMinX < (_local_16 - threshold))
				{
					_local_23++;
				}
			}
			else
			{
				if (_local_15->bound.boundMinX >= (_local_16 - threshold))
				{
					_local_24++;
				}
				else
				{
					break;
				}
			}
			_local_15 = _local_15->next;
		}

		if (_local_15 == nullptr)
		{
			double _local_28 = ((_local_26 * _local_23) + (_local_27 * _local_24));
			if (_local_28 < _local_22)
			{
				_local_22 = _local_28;
				_local_20 = 0;
				_local_21 = _local_16;
			}
		}
		_local_12++;
	}

	_local_25 = ((_arg_8 - _arg_5) * (_arg_10 - _arg_7));
	_local_12 = 0;

	while (_local_12 < _local_18)
	{
		double _local_16 = splitCoordsY[_local_12];
		double _local_26 = (_local_25 * (_local_16 - _arg_6));
		double _local_27 = (_local_25 * (_arg_9 - _local_16));
		int _local_23 = 0;
		int _local_24 = 0;
		_local_15 = _arg_2;
		while (_local_15 != nullptr)
		{
			if (_local_15->bound.boundMaxY <= (_local_16 + threshold))
			{
				if (_local_15->bound.boundMinY < (_local_16 - threshold))
				{
					_local_23++;
				}
			}
			else
			{
				if (_local_15->bound.boundMinY >= (_local_16 - threshold))
				{
					_local_24++;
				}
				else
				{
					break;
				}
			}
			_local_15 = _local_15->next;
		}

		if (_local_15 == nullptr)
		{
			double _local_28 = ((_local_26 * _local_23) + (_local_27 * _local_24));
			if (_local_28 < _local_22)
			{
				_local_22 = _local_28;
				_local_20 = 1;
				_local_21 = _local_16;
			}
		}
		_local_12++;
	}

	_local_25 = ((_arg_8 - _arg_5) * (_arg_9 - _arg_6));
	_local_12 = 0;

	while (_local_12 < _local_19)
	{
		double _local_16 = splitCoordsZ[_local_12];
		double _local_26 = (_local_25 * (_local_16 - _arg_7));
		double _local_27 = (_local_25 * (_arg_10 - _local_16));
		int _local_23 = 0;
		int _local_24 = 0;
		std::shared_ptr<Object3D> _local_15 = _arg_2;
		while (_local_15 != nullptr)
		{
			if (_local_15->bound.boundMaxZ <= (_local_16 + threshold))
			{
				if (_local_15->bound.boundMinZ < (_local_16 - threshold))
				{
					_local_23++;
				}
			}
			else
			{
				if (_local_15->bound.boundMinZ >= (_local_16 - threshold))
				{
					_local_24++;
				}
				else
				{
					break;
				}
			}
			_local_15 = _local_15->next;
		}
		if (_local_15 == nullptr)
		{
			double _local_28 = ((_local_26 * _local_23) + (_local_27 * _local_24));
			if (_local_28 < _local_22)
			{
				_local_22 = _local_28;
				_local_20 = 2;
				_local_21 = _local_16;
			}
		}
		_local_12++;
	}

	if (_local_20 < 0)
	{
		_local_11->objectList = _arg_1;
		_local_11->objectBoundList = _arg_2;
		_local_11->occluderList = _arg_3;
		_local_11->occluderBoundList = _arg_4;
	}
	else
	{
		_local_11->axis = _local_20;
		_local_11->coord = _local_21;
		_local_11->minCoord = (_local_21 - threshold);
		_local_11->maxCoord = (_local_21 + threshold);
		std::shared_ptr<Object3D> _local_14 = _arg_1;
		_local_15 = _arg_2;
		while (_local_14 != nullptr)
		{
			std::shared_ptr<Object3D> _local_39 = _local_14->next;
			std::shared_ptr<Object3D> _local_40 = _local_15->next;
			_local_14->next = nullptr;
			_local_15->next = nullptr;
			double _local_37 = ((_local_20 == 0) ? _local_15->bound.boundMinX : ((_local_20 == 1) ? _local_15->bound.boundMinY : _local_15->bound.boundMinZ));
			double _local_38 = ((_local_20 == 0) ? _local_15->bound.boundMaxX : ((_local_20 == 1) ? _local_15->bound.boundMaxY : _local_15->bound.boundMaxZ));
			if (_local_38 <= (_local_21 + threshold))
			{
				if (_local_37 < (_local_21 - threshold))
				{
					_local_14->next = _local_29;
					_local_29 = _local_14;
					_local_15->next = _local_30;
					_local_30 = _local_15;
				}
				else
				{
					_local_14->next = _local_11->objectList;
					_local_11->objectList = _local_14;
					_local_15->next = _local_11->objectBoundList;
					_local_11->objectBoundList = _local_15;
				}
			}
			else
			{
				if (_local_37 >= (_local_21 - threshold))
				{
					_local_14->next = _local_33;
					_local_33 = _local_14;
					_local_15->next = _local_34;
					_local_34 = _local_15;
				}
			}
			_local_14 = _local_39;
			_local_15 = _local_40;
		}

		_local_14 = _arg_3;
		_local_15 = _arg_4;

		while (_local_14 != nullptr)
		{
			std::shared_ptr<Object3D> _local_39 = _local_14->next;
			std::shared_ptr<Object3D> _local_40 = _local_15->next;
			_local_14->next = nullptr;
			_local_15->next = nullptr;
			double _local_37 = ((_local_20 == 0) ? _local_15->bound.boundMinX : ((_local_20 == 1) ? _local_15->bound.boundMinY : _local_15->bound.boundMinZ));
			double _local_38 = ((_local_20 == 0) ? _local_15->bound.boundMaxX : ((_local_20 == 1) ? _local_15->bound.boundMaxY : _local_15->bound.boundMaxZ));
			if (_local_38 <= (_local_21 + threshold))
			{
				if (_local_37 < (_local_21 - threshold))
				{
					_local_14->next = _local_31;
					_local_31 = _local_14;
					_local_15->next = _local_32;
					_local_32 = _local_15;
				}
				else
				{
					_local_14->next = _local_11->occluderList;
					_local_11->occluderList = _local_14;
					_local_15->next = _local_11->occluderBoundList;
					_local_11->occluderBoundList = _local_15;
				}
			}
			else
			{
				if (_local_37 >= (_local_21 - threshold))
				{
					_local_14->next = _local_35;
					_local_35 = _local_14;
					_local_15->next = _local_36;
					_local_36 = _local_15;
				}
			}
			_local_14 = _local_39;
			_local_15 = _local_40;
		}

		double _local_41 = _local_11->boundMinX;
		double _local_42 = _local_11->boundMinY;
		double _local_43 = _local_11->boundMinZ;

		double _local_44 = _local_11->boundMaxX;
		double _local_45 = _local_11->boundMaxY;
		double _local_46 = _local_11->boundMaxZ;

		double _local_47 = _local_11->boundMinX;
		double _local_48 = _local_11->boundMinY;
		double _local_49 = _local_11->boundMinZ;

		double _local_50 = _local_11->boundMaxX;
		double _local_51 = _local_11->boundMaxY;
		double _local_52 = _local_11->boundMaxZ;

		if (_local_20 == 0)
		{
			_local_44 = _local_21;
			_local_47 = _local_21;
		}
		else
		{
			if (_local_20 == 1)
			{
				_local_45 = _local_21;
				_local_48 = _local_21;
			}
			else
			{
				_local_46 = _local_21;
				_local_49 = _local_21;
			}
		}

		_local_11->setNegative(createNode(_local_29, _local_30, _local_31, _local_32, _local_41, _local_42, _local_43, _local_44, _local_45, _local_46, threshold));
		_local_11->setPositive(createNode(_local_33, _local_34, _local_35, _local_36, _local_47, _local_48, _local_49, _local_50, _local_51, _local_52, threshold));
	}

	return _local_11;
}


void KDContainer::createTree(QVector<std::shared_ptr<Object3D>> &_arg_1,
							 QVector<std::shared_ptr<Occluder>> *_arg_2)
{
	std::shared_ptr<Object3D> _local_8;
	std::shared_ptr<Object3D> _local_9;
	std::shared_ptr<Object3D> _local_10;
	std::shared_ptr<Object3D> _local_11;

	Shared<Vertex> _local_28 = nullptr;

	this->destroyTree();

	int _local_6 = _arg_1.length();
	int _local_7 = ((_arg_2 != nullptr) ? _arg_2->length() : 0);
	int _local_3 = 0;


	QHash<std::shared_ptr<Material>, std::shared_ptr<Mesh>> _local_13;

	while (_local_3 < _local_6)
	{
		std::shared_ptr<Object3D> _local_4 = _arg_1[_local_3];


		std::shared_ptr<Material> _local_12;

		if (Object3D_isType<Mesh>(_local_4.get()))
		{
			std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(_local_4);
			_local_12 = mesh->faceList->material;
		}
		else if (Object3D_isType<BSP>(_local_4.get()))
		{
			std::shared_ptr<BSP> bsp = std::dynamic_pointer_cast<BSP>(_local_4);

			if (bsp->faces.isEmpty())
			{
				throw Error("KDContainer::createTree");
			}

			_local_12 = bsp->faces[0]->material;
		}
		else
		{
			_local_12 = nullptr;
		}

		if (_local_12 != nullptr)
		{
			this->materials[_local_12] = true;
			if (_local_12->transparent())
			{
				if (this->transparent.length() <= this->transparentLength)
				{
					this->transparent.append(_local_4);
				}
				else
				{
					this->transparent[this->transparentLength] = _local_4;
				}
				this->transparentLength++;
			}
			else
			{

				std::shared_ptr<Mesh> _local_29 = _local_13[_local_12];

				if (_local_29 == nullptr)
				{
					_local_29 = new_Mesh();
					_local_13[_local_12] = _local_29;
					_local_29->next = this->opaqueList;
					this->opaqueList = _local_29;
					_local_29->setParent(std::dynamic_pointer_cast<Object3DContainer>(this->shared_from_this()));
				}

				_local_4 = _local_4->clone();
				_local_4->composeMatrix();
				if (Object3D_isType<Mesh>(_local_4.get()))
				{
					std::shared_ptr<Mesh> _local_30 = std::dynamic_pointer_cast<Mesh>(_local_4);
					if (_local_30->faceList != nullptr)
					{
						Shared<Vertex> _local_26 = _local_30->vertexList;
						while (_local_26 != nullptr)
						{
							double _local_20 = _local_26->x;
							double _local_21 = _local_26->y;
							double _local_22 = _local_26->z;
							_local_26->x = ((((_local_4->matrix4.ma * _local_20) + (_local_4->matrix4.mb * _local_21)) + (_local_4->matrix4.mc * _local_22)) + _local_4->matrix4.md);
							_local_26->y = ((((_local_4->matrix4.me * _local_20) + (_local_4->matrix4.mf * _local_21)) + (_local_4->matrix4.mg * _local_22)) + _local_4->matrix4.mh);
							_local_26->z = ((((_local_4->matrix4.mi * _local_20) + (_local_4->matrix4.mj * _local_21)) + (_local_4->matrix4.mk * _local_22)) + _local_4->matrix4.ml);
							double _local_23 = _local_26->normalX;
							double _local_24 = _local_26->normalY;
							double _local_25 = _local_26->normalZ;
							_local_26->normalX = (((_local_4->matrix4.ma * _local_23) + (_local_4->matrix4.mb * _local_24)) + (_local_4->matrix4.mc * _local_25));
							_local_26->normalY = (((_local_4->matrix4.me * _local_23) + (_local_4->matrix4.mf * _local_24)) + (_local_4->matrix4.mg * _local_25));
							_local_26->normalZ = (((_local_4->matrix4.mi * _local_23) + (_local_4->matrix4.mj * _local_24)) + (_local_4->matrix4.mk * _local_25));
							_local_26->transformId = 0;
							if (_local_26->next == nullptr)
							{
								_local_28 = _local_26;
							}
							_local_26 = _local_26->next;
						}
						_local_28->next = _local_29->vertexList;
						_local_29->vertexList = _local_30->vertexList;
						_local_30->vertexList = nullptr;
						Shared<Face> _local_27 = _local_30->faceList;
						while (_local_27->next != nullptr)
						{
							_local_27 = _local_27->next;
						}
						_local_27->next = _local_29->faceList;
						_local_29->faceList = _local_30->faceList;
						_local_30->faceList = nullptr;
					}
				}
				else
				{
					if (Object3D_isType<BSP>(_local_4.get()))
					{
						std::shared_ptr<BSP> _local_31 = std::dynamic_pointer_cast<BSP>(_local_4);

						if (_local_31->root != nullptr)
						{
							Shared<Vertex> _local_26 = _local_31->vertexList;
							while (_local_26 != nullptr)
							{
								double _local_20 = _local_26->x;
								double _local_21 = _local_26->y;
								double _local_22 = _local_26->z;
								_local_26->x = ((((_local_4->matrix4.ma * _local_20) + (_local_4->matrix4.mb * _local_21)) + (_local_4->matrix4.mc * _local_22)) + _local_4->matrix4.md);
								_local_26->y = ((((_local_4->matrix4.me * _local_20) + (_local_4->matrix4.mf * _local_21)) + (_local_4->matrix4.mg * _local_22)) + _local_4->matrix4.mh);
								_local_26->z = ((((_local_4->matrix4.mi * _local_20) + (_local_4->matrix4.mj * _local_21)) + (_local_4->matrix4.mk * _local_22)) + _local_4->matrix4.ml);
								double _local_23 = _local_26->normalX;
								double _local_24 = _local_26->normalY;
								double _local_25 = _local_26->normalZ;
								_local_26->normalX = (((_local_4->matrix4.ma * _local_23) + (_local_4->matrix4.mb * _local_24)) + (_local_4->matrix4.mc * _local_25));
								_local_26->normalY = (((_local_4->matrix4.me * _local_23) + (_local_4->matrix4.mf * _local_24)) + (_local_4->matrix4.mg * _local_25));
								_local_26->normalZ = (((_local_4->matrix4.mi * _local_23) + (_local_4->matrix4.mj * _local_24)) + (_local_4->matrix4.mk * _local_25));
								_local_26->transformId = 0;
								if (_local_26->next == nullptr)
								{
									_local_28 = _local_26;
								}
								_local_26 = _local_26->next;
							}
							_local_28->next = _local_29->vertexList;
							_local_29->vertexList = _local_31->vertexList;
							_local_31->vertexList = nullptr;
							for (Shared<Face> _local_27 : _local_31->faces)
							{
								_local_27->next = _local_29->faceList;
								_local_29->faceList = _local_27;
							}
							//_local_31->faces->length = 0;
							_local_31->faces.clear();
							_local_31->root = nullptr;
						}
					}
				}
			}
		}

		_local_3++;
	}


	QHashIterator<std::shared_ptr<Material>, std::shared_ptr<Mesh>> i(_local_13);
	while (i.hasNext())
	{
		i.next();
		std::shared_ptr<Mesh> mesh = i.value();
		mesh->calculateFacesNormals(true);
		mesh->calculateBounds();
	}


	double _local_14 = 1E22;
	double _local_15 = 1E22;
	double _local_16 = 1E22;
	double _local_17 = -1E22;
	double _local_18 = -1E22;
	double _local_19 = -1E22;
	_local_3 = 0;

	while (_local_3 < _local_6)
	{
		std::shared_ptr<Object3D> _local_4 = _arg_1[_local_3];
		std::shared_ptr<Object3D> _local_5 = createObjectBounds(_local_4.get());
		if (_local_5->bound.boundMinX <= _local_5->bound.boundMaxX)
		{
			if (_local_4->_parent != nullptr)
			{
				_local_4->_parent->removeChild(_local_4);
			}
			_local_4->setParent(std::dynamic_pointer_cast<Object3DContainer>(this->shared_from_this()));
			_local_4->next = _local_8;
			_local_8 = _local_4;
			_local_5->next = _local_9;
			_local_9 = _local_5;
			if (_local_5->bound.boundMinX < _local_14)
			{
				_local_14 = _local_5->bound.boundMinX;
			}
			if (_local_5->bound.boundMaxX > _local_17)
			{
				_local_17 = _local_5->bound.boundMaxX;
			}
			if (_local_5->bound.boundMinY < _local_15)
			{
				_local_15 = _local_5->bound.boundMinY;
			}
			if (_local_5->bound.boundMaxY > _local_18)
			{
				_local_18 = _local_5->bound.boundMaxY;
			}
			if (_local_5->bound.boundMinZ < _local_16)
			{
				_local_16 = _local_5->bound.boundMinZ;
			}
			if (_local_5->bound.boundMaxZ > _local_19)
			{
				_local_19 = _local_5->bound.boundMaxZ;
			}
		}
		_local_3++;
	}

	_local_3 = 0;

	while (_local_3 < _local_7)
	{
		std::shared_ptr<Object3D> _local_4 = _arg_2->at(_local_3);
		std::shared_ptr<Object3D> _local_5 = createObjectBounds(_local_4.get());
		if (_local_5->bound.boundMinX <= _local_5->bound.boundMaxX)
		{
			if (!((((((_local_5->bound.boundMinX < _local_14) ||
					  (_local_5->bound.boundMaxX > _local_17)) ||
					 (_local_5->bound.boundMinY < _local_15)) ||
					(_local_5->bound.boundMaxY > _local_18)) ||
				   (_local_5->bound.boundMinZ < _local_16)) ||
				  (_local_5->bound.boundMaxZ > _local_19)))
			{
				if (_local_4->_parent != nullptr)
				{
					_local_4->_parent->removeChild(_local_4);
				}
				_local_4->setParent(std::dynamic_pointer_cast<Object3DContainer>(this->shared_from_this()));
				_local_4->next = _local_10;
				_local_10 = _local_4;
				_local_5->next = _local_11;
				_local_11 = _local_5;
			}
		}
		_local_3++;
	}

	if (_local_8 != nullptr)
	{

		this->root = createNode(_local_8,
								_local_9,
								_local_10,
								_local_11,
								_local_14,
								_local_15,
								_local_16,
								_local_17,
								_local_18,
								_local_19,
								threshold);

		QVector<QVector<float>*> _local_32;
		QVector<QVector<uint>*> _local_33;
		_local_32.append(new QVector<float>());
		_local_33.append(new QVector<uint>());

		this->root->createReceivers(_local_32, _local_33);

		int i = 0;

		while (i < _local_32.length())
		{
			this->receiversVertexBuffers.append(new VertexBufferResource(*_local_32[i], 3));
			this->receiversIndexBuffers.append(new IndexBufferResource(*_local_33[i]));
			i++;
		}
	}
}


void KDContainer::destroyTree()
{

}


static void updateBoundsNode(KDNode *_arg_1, Object3D *_arg_2, Object3D *_arg_3)
{
	Object3D *_local_4 = _arg_1->objectList.get();

	while (_local_4 != nullptr)
	{
		if (_arg_3 != nullptr)
		{
			_local_4->composeAndAppend(_arg_3);
		}
		else
		{
			_local_4->composeMatrix();
		}
		_local_4->updateBounds(_arg_2, _local_4);
		_local_4 = _local_4->next.get();
	}

	if (_arg_1->negative() != nullptr)
	{
		updateBoundsNode(_arg_1->negative(), _arg_2, _arg_3);
		updateBoundsNode(_arg_1->positive(), _arg_2, _arg_3);
	}
}


void KDContainer::updateBounds(Object3D *_arg_1, Object3D *_arg_2)
{
	ConflictContainer::updateBounds(_arg_1, _arg_2);

	if (this->root != nullptr)
	{
		if (_arg_2 != nullptr)
		{
			updateBoundsNode(this->root.get(), _arg_1, _arg_2);
		}
		else
		{
			if (this->root->boundMinX < _arg_1->bound.boundMinX)
			{
				_arg_1->bound.boundMinX = this->root->boundMinX;
			}
			if (this->root->boundMaxX > _arg_1->bound.boundMaxX)
			{
				_arg_1->bound.boundMaxX = this->root->boundMaxX;
			}
			if (this->root->boundMinY < _arg_1->bound.boundMinY)
			{
				_arg_1->bound.boundMinY = this->root->boundMinY;
			}
			if (this->root->boundMaxY > _arg_1->bound.boundMaxY)
			{
				_arg_1->bound.boundMaxY = this->root->boundMaxY;
			}
			if (this->root->boundMinZ < _arg_1->bound.boundMinZ)
			{
				_arg_1->bound.boundMinZ = this->root->boundMinZ;
			}
			if (this->root->boundMaxZ > _arg_1->bound.boundMaxZ)
			{
				_arg_1->bound.boundMaxZ = this->root->boundMaxZ;
			}
		}
	}
}


void KDContainer::draw(Camera3D *camera)
{
	//this->uploadResources(camera->device);

	calculateInverseMatrix();

	//int _local_6 = ((camera->debug) ? camera->checkInDebug(this) : 0);

	//if ((_local_6 & Debug.BOUNDS))
	//{
	//	Debug.drawBounds(camera, this, boundMinX, boundMinY, boundMinZ, boundMaxX, boundMaxY, boundMaxZ);
	//}

	//this->batched = false;

	if (this->batched)
	{
		//bool _local_7 = camera->debug;
		//if (((_local_6) && (_local_6 & Debug.NODES)))
		//{
		//	camera->debug = false;
		//}

		Object3D *_local_3 = this->opaqueList.get();

		while (_local_3 != nullptr)
		{
			_local_3->matrix4.ma = this->matrix4.ma;
			_local_3->matrix4.mb = this->matrix4.mb;
			_local_3->matrix4.mc = this->matrix4.mc;
			_local_3->matrix4.md = this->matrix4.md;
			_local_3->matrix4.me = this->matrix4.me;
			_local_3->matrix4.mf = this->matrix4.mf;
			_local_3->matrix4.mg = this->matrix4.mg;
			_local_3->matrix4.mh = this->matrix4.mh;
			_local_3->matrix4.mi = this->matrix4.mi;
			_local_3->matrix4.mj = this->matrix4.mj;
			_local_3->matrix4.mk = this->matrix4.mk;
			_local_3->matrix4.ml = this->matrix4.ml;
			_local_3->concat(this);
			_local_3->draw(camera);
#if 0
			if (((((!(camera->view->constrained)) && (!(camera->shadowMap == nullptr))) && (camera->shadowMapStrength > 0)) &&
				 (_local_3->concatenatedAlpha >= _local_3->shadowMapAlphaThreshold)))
			{
				camera->casterObjects[camera->casterCount] = _local_3;
				camera->casterCount++;
			}
#endif
			_local_3 = _local_3->next.get();
		}


		//camera->debug = _local_7;
		Shared<VG> _local_5 = ConflictContainer::getVG(camera);

		if ((((!(camera->view->constrained)) && (!(camera->shadowMap == nullptr))) && (camera->shadowMapStrength > 0)))
		{
			_local_3 = childrenList.get();
			while (_local_3 != nullptr)
			{
				if (_local_3->visible)
				{
#if 0
					if ((((_local_3 is Mesh) || (_local_3 is BSP)) || (_local_3 is Sprite3D)))
					{
						if (_local_3->concatenatedAlpha >= _local_3->shadowMapAlphaThreshold)
						{
							camera->casterObjects[camera->casterCount] = _local_3;
							camera->casterCount++;
						}
					}
					else
					{
						if ((_local_3 is Object3DContainer))
						{
							Object3D *_local_8 = Object3DContainer(_local_3).childrenList;
							while (_local_8 != null)
							{
								if (((((_local_8 is Mesh) || (_local_8 is BSP)) || (_local_8 is Sprite3D)) && (_local_8->concatenatedAlpha >= _local_8->shadowMapAlphaThreshold)))
								{
									camera->casterObjects[camera->casterCount] = _local_8;
									camera->casterCount++;
								}
								_local_8 = _local_8->next;
							}
						}
					}
#endif
				}
				_local_3 = _local_3->next.get();
			}
		}

		int _local_2 = 0;
		while (_local_2 < this->transparentLength)
		{
			_local_3 = this->transparent[_local_2].get();
			_local_3->composeAndAppend(this);
			if (_local_3->cullingInCamera(camera, culling) >= 0)
			{
				_local_3->concat(this);
				Shared<VG> _local_9 = _local_3->getVG(camera);
				if (_local_9 != nullptr)
				{
					_local_9->next = _local_5;
					_local_5 = _local_9;
				}
			}
#if 0
			if (((((!(camera->view->constrained)) && (!(camera->shadowMap == nullptr))) && (camera->shadowMapStrength > 0)) && (_local_3->concatenatedAlpha >= _local_3->shadowMapAlphaThreshold)))
			{
				camera->casterObjects[camera->casterCount] = _local_3;
				camera->casterCount++;
			}
#endif
			_local_2++;
		}

		if (_local_5 != nullptr)
		{
			if (_local_5->next != nullptr)
			{
				if (resolveByAABB)
				{
					Shared<VG> _local_4 = _local_5;
					while (_local_4 != nullptr)
					{
						_local_4->calculateAABB(this->im.ma,
												this->im.mb,
												this->im.mc,
												this->im.md,
												this->im.me,
												this->im.mf,
												this->im.mg,
												this->im.mh,
												this->im.mi,
												this->im.mj,
												this->im.mk,
												this->im.ml);

						_local_4 = _local_4->next;
					}

					drawAABBGeometry(camera, _local_5);
				}
				else
				{
					if (resolveByOOBB)
					{
						Weak<VG> _local_4 = _local_5;
						while (_local_4 != nullptr)
						{
							_local_4->calculateOOBB(this);
							_local_4 = _local_4->next;
						}
						drawOOBBGeometry(camera, _local_5);
					}
					else
					{
						drawConflictGeometry(camera, _local_5);
					}
				}
			}
			else
			{
				_local_5->draw(camera, threshold, this);
				_local_5->destroy();
			}
		}
	}
	else
	{
		if (this->root != nullptr)
		{
			this->calculateCameraPlanes(camera->nearClipping, camera->farClipping);

			int _local_10 = this->cullingInContainer(culling,
													 this->root->boundMinX,
													 this->root->boundMinY,
													 this->root->boundMinZ,
													 this->root->boundMaxX,
													 this->root->boundMaxY,
													 this->root->boundMaxZ);

			_local_10 = 0;
			if (_local_10 >= 0)
			{
				this->numOccluders = 0;

				if (camera->numOccluders > 0)
				{
					this->updateOccluders(camera);
				}

				Shared<VG> _local_5 = ConflictContainer::getVG(camera);

				Shared<VG> _local_4 = _local_5;

				while (_local_4 != nullptr)
				{
					_local_4->calculateAABB(this->im.ma,
											this->im.mb,
											this->im.mc,
											this->im.md,
											this->im.me,
											this->im.mf,
											this->im.mg,
											this->im.mh,
											this->im.mi,
											this->im.mj,
											this->im.mk,
											this->im.ml);

					_local_4 = _local_4->next;
				}

				this->drawNode(this->root.get(), _local_10, camera, _local_5);

				int _local_2 = 0;
				while (_local_2 < this->numOccluders)
				{
#if 0
					Vertex *_local_11 = this->occluders[_local_2];
					Vertex *_local_12 = _local_11;
					while (_local_12->next != nullptr)
					{
						_local_12 = _local_12->next;
					}
					_local_12->next = Vertex::collector;
					Vertex::collector = _local_11;
					this->occluders[_local_2] = nullptr;
#endif
					Shared<Vertex> _local_11 = this->occluders[_local_2];
					this->occluders[_local_2] = nullptr;

					_local_11->clear();


					_local_2++;
				}
				this->numOccluders = 0;
			}
			else
			{
				ConflictContainer::draw(camera);
			}
		}
		else
		{
			ConflictContainer::draw(camera);
		}
	}

	//if (((!(this->root == nullptr)) && (_local_6 & Debug.NODES)))
	//{
	//	this->debugNode(this->root, _local_10, camera, 1);
	//	Debug.drawBounds(camera, this, this->root.boundMinX, this->root.boundMinY, this->root.boundMinZ, this->root.boundMaxX, this->root.boundMaxY, this->root.boundMaxZ, 14496733);
	//}

	if (this->root != nullptr)
	{
		//camera->receiversVertexBuffers = this->receiversVertexBuffers;
		//camera->receiversIndexBuffers = this->receiversIndexBuffers;

#if 0
		for (ShadowAtlas _local_13 in camera->shadowAtlases)
		{
			int _local_2 = 0;
			while (_local_2 < _local_13->shadowsCount)
			{
				this->root.collectReceivers(_local_13->shadows[_local_2]);
				_local_2++;
			}
		}
#endif
	}
}


std::shared_ptr<Object3D> KDContainer::clone() const
{
	qDebug("used KDContainer::clone");
	return nullptr;
}


void KDContainer::uploadResources(Device *device)
{
	(void)device;
	qDebug("used KDContainer::uploadResources");

#if 0


	if (this->isCreated)
	{
		return;
	}

	this->isCreated = true;
	for (_local_2 in this->materials)
	{
		TextureMaterial *textureMaterial = (_local_2 as TextureMaterial);
		if (textureMaterial->texture != null)
		{
			device->uploadResource(textureMaterial->textureResource);
		}
		if (textureMaterial->_textureATF != null)
		{
			device->uploadResource(textureMaterial->textureATFResource);
		}
		if (textureMaterial->_textureATFAlpha != null)
		{
			device->uploadResource(textureMaterial->textureATFAlphaResource);
		}
	}

	_local_3 = this->opaqueList;
	while (_local_3 != null)
	{
		if ((_local_3 is Mesh))
		{
			Mesh *mesh = (_local_3 as Mesh);
			mesh->prepareResources();
			device->uploadResource(mesh->vertexBuffer);
			device->uploadResource(mesh->indexBuffer);
		}
		else
		{
			if ((_local_3 is BSP))
			{
				BSP *bsp = (_local_3 as BSP);
				bsp->prepareResources();
				device->uploadResource(bsp->vertexBuffer);
				device->uploadResource(bsp->indexBuffer);
			}
		}
		_local_3 = _local_3->next;
	}

	int _local_6 = 0;
	while (_local_6 < this->transparentLength)
	{
		_local_3 = this->transparent[_local_6];
		if ((_local_3 is Mesh))
		{
			Mesh *mesh = (_local_3 as Mesh);
			mesh->prepareResources();
			device->uploadResource(mesh->vertexBuffer);
			device->uploadResource(mesh->indexBuffer);
		}
		else
		{
			if ((_local_3 is BSP))
			{
				BSP *bsp = (_local_3 as BSP);
				bsp->prepareResources();
				device->uploadResource(bsp->vertexBuffer);
				device->uploadResource(bsp->indexBuffer);
			}
		}
		_local_6++;
	}

	_local_6 = 0;
	while (_local_6 < this->receiversVertexBuffers.length)
	{
		device->uploadResource(this->receiversVertexBuffers[_local_6]);
		device->uploadResource(this->receiversIndexBuffers[_local_6]);
		_local_6++;
	}
#endif
}


int KDContainer::cullingInContainer(int _arg_1,
									double _arg_2,
									double _arg_3,
									double _arg_4,
									double _arg_5,
									double _arg_6,
									double _arg_7)
{

	if (_arg_1 > 0)
	{
		if ((_arg_1 & 0x01))
		{
			if (this->nearPlaneX >= 0)
			{
				if (this->nearPlaneY >= 0)
				{
					if (this->nearPlaneZ >= 0)
					{
						if ((((_arg_5 * this->nearPlaneX) + (_arg_6 * this->nearPlaneY)) + (_arg_7 * this->nearPlaneZ)) <= this->nearPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->nearPlaneX) + (_arg_3 * this->nearPlaneY)) + (_arg_4 * this->nearPlaneZ)) > this->nearPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3E);
						}
					}
					else
					{
						if ((((_arg_5 * this->nearPlaneX) + (_arg_6 * this->nearPlaneY)) + (_arg_4 * this->nearPlaneZ)) <= this->nearPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->nearPlaneX) + (_arg_3 * this->nearPlaneY)) + (_arg_7 * this->nearPlaneZ)) > this->nearPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3E);
						}
					}
				}
				else
				{
					if (this->nearPlaneZ >= 0)
					{
						if ((((_arg_5 * this->nearPlaneX) + (_arg_3 * this->nearPlaneY)) + (_arg_7 * this->nearPlaneZ)) <= this->nearPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->nearPlaneX) + (_arg_6 * this->nearPlaneY)) + (_arg_4 * this->nearPlaneZ)) > this->nearPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3E);
						}
					}
					else
					{
						if ((((_arg_5 * this->nearPlaneX) + (_arg_3 * this->nearPlaneY)) + (_arg_4 * this->nearPlaneZ)) <= this->nearPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->nearPlaneX) + (_arg_6 * this->nearPlaneY)) + (_arg_7 * this->nearPlaneZ)) > this->nearPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3E);
						}
					}
				}
			}
			else
			{
				if (this->nearPlaneY >= 0)
				{
					if (this->nearPlaneZ >= 0)
					{
						if ((((_arg_2 * this->nearPlaneX) + (_arg_6 * this->nearPlaneY)) + (_arg_7 * this->nearPlaneZ)) <= this->nearPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->nearPlaneX) + (_arg_3 * this->nearPlaneY)) + (_arg_4 * this->nearPlaneZ)) > this->nearPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3E);
						}
					}
					else
					{
						if ((((_arg_2 * this->nearPlaneX) + (_arg_6 * this->nearPlaneY)) + (_arg_4 * this->nearPlaneZ)) <= this->nearPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->nearPlaneX) + (_arg_3 * this->nearPlaneY)) + (_arg_7 * this->nearPlaneZ)) > this->nearPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3E);
						}
					}
				}
				else
				{
					if (this->nearPlaneZ >= 0)
					{
						if ((((_arg_2 * this->nearPlaneX) + (_arg_3 * this->nearPlaneY)) + (_arg_7 * this->nearPlaneZ)) <= this->nearPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->nearPlaneX) + (_arg_6 * this->nearPlaneY)) + (_arg_4 * this->nearPlaneZ)) > this->nearPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3E);
						}
					}
					else
					{
						if ((((_arg_2 * this->nearPlaneX) + (_arg_3 * this->nearPlaneY)) + (_arg_4 * this->nearPlaneZ)) <= this->nearPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->nearPlaneX) + (_arg_6 * this->nearPlaneY)) + (_arg_7 * this->nearPlaneZ)) > this->nearPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3E);
						}
					}
				}
			}
		}
		if ((_arg_1 & 0x02))
		{
			if (this->farPlaneX >= 0)
			{
				if (this->farPlaneY >= 0)
				{
					if (this->farPlaneZ >= 0)
					{
						if ((((_arg_5 * this->farPlaneX) + (_arg_6 * this->farPlaneY)) + (_arg_7 * this->farPlaneZ)) <= this->farPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->farPlaneX) + (_arg_3 * this->farPlaneY)) + (_arg_4 * this->farPlaneZ)) > this->farPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3D);
						}
					}
					else
					{
						if ((((_arg_5 * this->farPlaneX) + (_arg_6 * this->farPlaneY)) + (_arg_4 * this->farPlaneZ)) <= this->farPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->farPlaneX) + (_arg_3 * this->farPlaneY)) + (_arg_7 * this->farPlaneZ)) > this->farPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3D);
						}
					}
				}
				else
				{
					if (this->farPlaneZ >= 0)
					{
						if ((((_arg_5 * this->farPlaneX) + (_arg_3 * this->farPlaneY)) + (_arg_7 * this->farPlaneZ)) <= this->farPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->farPlaneX) + (_arg_6 * this->farPlaneY)) + (_arg_4 * this->farPlaneZ)) > this->farPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3D);
						}
					}
					else
					{
						if ((((_arg_5 * this->farPlaneX) + (_arg_3 * this->farPlaneY)) + (_arg_4 * this->farPlaneZ)) <= this->farPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->farPlaneX) + (_arg_6 * this->farPlaneY)) + (_arg_7 * this->farPlaneZ)) > this->farPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3D);
						}
					}
				}
			}
			else
			{
				if (this->farPlaneY >= 0)
				{
					if (this->farPlaneZ >= 0)
					{
						if ((((_arg_2 * this->farPlaneX) + (_arg_6 * this->farPlaneY)) + (_arg_7 * this->farPlaneZ)) <= this->farPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->farPlaneX) + (_arg_3 * this->farPlaneY)) + (_arg_4 * this->farPlaneZ)) > this->farPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3D);
						}
					}
					else
					{
						if ((((_arg_2 * this->farPlaneX) + (_arg_6 * this->farPlaneY)) + (_arg_4 * this->farPlaneZ)) <= this->farPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->farPlaneX) + (_arg_3 * this->farPlaneY)) + (_arg_7 * this->farPlaneZ)) > this->farPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3D);
						}
					}
				}
				else
				{
					if (this->farPlaneZ >= 0)
					{
						if ((((_arg_2 * this->farPlaneX) + (_arg_3 * this->farPlaneY)) + (_arg_7 * this->farPlaneZ)) <= this->farPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->farPlaneX) + (_arg_6 * this->farPlaneY)) + (_arg_4 * this->farPlaneZ)) > this->farPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3D);
						}
					}
					else
					{
						if ((((_arg_2 * this->farPlaneX) + (_arg_3 * this->farPlaneY)) + (_arg_4 * this->farPlaneZ)) <= this->farPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->farPlaneX) + (_arg_6 * this->farPlaneY)) + (_arg_7 * this->farPlaneZ)) > this->farPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3D);
						}
					}
				}
			}
		}
		if ((_arg_1 & 0x04))
		{
			if (this->leftPlaneX >= 0)
			{
				if (this->leftPlaneY >= 0)
				{
					if (this->leftPlaneZ >= 0)
					{
						if ((((_arg_5 * this->leftPlaneX) + (_arg_6 * this->leftPlaneY)) + (_arg_7 * this->leftPlaneZ)) <= this->leftPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->leftPlaneX) + (_arg_3 * this->leftPlaneY)) + (_arg_4 * this->leftPlaneZ)) > this->leftPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3B);
						}
					}
					else
					{
						if ((((_arg_5 * this->leftPlaneX) + (_arg_6 * this->leftPlaneY)) + (_arg_4 * this->leftPlaneZ)) <= this->leftPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->leftPlaneX) + (_arg_3 * this->leftPlaneY)) + (_arg_7 * this->leftPlaneZ)) > this->leftPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3B);
						}
					}
				}
				else
				{
					if (this->leftPlaneZ >= 0)
					{
						if ((((_arg_5 * this->leftPlaneX) + (_arg_3 * this->leftPlaneY)) + (_arg_7 * this->leftPlaneZ)) <= this->leftPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->leftPlaneX) + (_arg_6 * this->leftPlaneY)) + (_arg_4 * this->leftPlaneZ)) > this->leftPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3B);
						}
					}
					else
					{
						if ((((_arg_5 * this->leftPlaneX) + (_arg_3 * this->leftPlaneY)) + (_arg_4 * this->leftPlaneZ)) <= this->leftPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->leftPlaneX) + (_arg_6 * this->leftPlaneY)) + (_arg_7 * this->leftPlaneZ)) > this->leftPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3B);
						}
					}
				}
			}
			else
			{
				if (this->leftPlaneY >= 0)
				{
					if (this->leftPlaneZ >= 0)
					{
						if ((((_arg_2 * this->leftPlaneX) + (_arg_6 * this->leftPlaneY)) + (_arg_7 * this->leftPlaneZ)) <= this->leftPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->leftPlaneX) + (_arg_3 * this->leftPlaneY)) + (_arg_4 * this->leftPlaneZ)) > this->leftPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3B);
						}
					}
					else
					{
						if ((((_arg_2 * this->leftPlaneX) + (_arg_6 * this->leftPlaneY)) + (_arg_4 * this->leftPlaneZ)) <= this->leftPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->leftPlaneX) + (_arg_3 * this->leftPlaneY)) + (_arg_7 * this->leftPlaneZ)) > this->leftPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3B);
						}
					}
				}
				else
				{
					if (this->leftPlaneZ >= 0)
					{
						if ((((_arg_2 * this->leftPlaneX) + (_arg_3 * this->leftPlaneY)) + (_arg_7 * this->leftPlaneZ)) <= this->leftPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->leftPlaneX) + (_arg_6 * this->leftPlaneY)) + (_arg_4 * this->leftPlaneZ)) > this->leftPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3B);
						}
					}
					else
					{
						if ((((_arg_2 * this->leftPlaneX) + (_arg_3 * this->leftPlaneY)) + (_arg_4 * this->leftPlaneZ)) <= this->leftPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->leftPlaneX) + (_arg_6 * this->leftPlaneY)) + (_arg_7 * this->leftPlaneZ)) > this->leftPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x3B);
						}
					}
				}
			}
		}
		if ((_arg_1 & 0x08))
		{
			if (this->rightPlaneX >= 0)
			{
				if (this->rightPlaneY >= 0)
				{
					if (this->rightPlaneZ >= 0)
					{
						if ((((_arg_5 * this->rightPlaneX) + (_arg_6 * this->rightPlaneY)) + (_arg_7 * this->rightPlaneZ)) <= this->rightPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->rightPlaneX) + (_arg_3 * this->rightPlaneY)) + (_arg_4 * this->rightPlaneZ)) > this->rightPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x37);
						}
					}
					else
					{
						if ((((_arg_5 * this->rightPlaneX) + (_arg_6 * this->rightPlaneY)) + (_arg_4 * this->rightPlaneZ)) <= this->rightPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->rightPlaneX) + (_arg_3 * this->rightPlaneY)) + (_arg_7 * this->rightPlaneZ)) > this->rightPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x37);
						}
					}
				}
				else
				{
					if (this->rightPlaneZ >= 0)
					{
						if ((((_arg_5 * this->rightPlaneX) + (_arg_3 * this->rightPlaneY)) + (_arg_7 * this->rightPlaneZ)) <= this->rightPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->rightPlaneX) + (_arg_6 * this->rightPlaneY)) + (_arg_4 * this->rightPlaneZ)) > this->rightPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x37);
						}
					}
					else
					{
						if ((((_arg_5 * this->rightPlaneX) + (_arg_3 * this->rightPlaneY)) + (_arg_4 * this->rightPlaneZ)) <= this->rightPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->rightPlaneX) + (_arg_6 * this->rightPlaneY)) + (_arg_7 * this->rightPlaneZ)) > this->rightPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x37);
						}
					}
				}
			}
			else
			{
				if (this->rightPlaneY >= 0)
				{
					if (this->rightPlaneZ >= 0)
					{
						if ((((_arg_2 * this->rightPlaneX) + (_arg_6 * this->rightPlaneY)) + (_arg_7 * this->rightPlaneZ)) <= this->rightPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->rightPlaneX) + (_arg_3 * this->rightPlaneY)) + (_arg_4 * this->rightPlaneZ)) > this->rightPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x37);
						}
					}
					else
					{
						if ((((_arg_2 * this->rightPlaneX) + (_arg_6 * this->rightPlaneY)) + (_arg_4 * this->rightPlaneZ)) <= this->rightPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->rightPlaneX) + (_arg_3 * this->rightPlaneY)) + (_arg_7 * this->rightPlaneZ)) > this->rightPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x37);
						}
					}
				}
				else
				{
					if (this->rightPlaneZ >= 0)
					{
						if ((((_arg_2 * this->rightPlaneX) + (_arg_3 * this->rightPlaneY)) + (_arg_7 * this->rightPlaneZ)) <= this->rightPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->rightPlaneX) + (_arg_6 * this->rightPlaneY)) + (_arg_4 * this->rightPlaneZ)) > this->rightPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x37);
						}
					}
					else
					{
						if ((((_arg_2 * this->rightPlaneX) + (_arg_3 * this->rightPlaneY)) + (_arg_4 * this->rightPlaneZ)) <= this->rightPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->rightPlaneX) + (_arg_6 * this->rightPlaneY)) + (_arg_7 * this->rightPlaneZ)) > this->rightPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x37);
						}
					}
				}
			}
		}
		if ((_arg_1 & 0x10))
		{
			if (this->topPlaneX >= 0)
			{
				if (this->topPlaneY >= 0)
				{
					if (this->topPlaneZ >= 0)
					{
						if ((((_arg_5 * this->topPlaneX) + (_arg_6 * this->topPlaneY)) + (_arg_7 * this->topPlaneZ)) <= this->topPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->topPlaneX) + (_arg_3 * this->topPlaneY)) + (_arg_4 * this->topPlaneZ)) > this->topPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x2F);
						}
					}
					else
					{
						if ((((_arg_5 * this->topPlaneX) + (_arg_6 * this->topPlaneY)) + (_arg_4 * this->topPlaneZ)) <= this->topPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->topPlaneX) + (_arg_3 * this->topPlaneY)) + (_arg_7 * this->topPlaneZ)) > this->topPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x2F);
						}
					}
				}
				else
				{
					if (this->topPlaneZ >= 0)
					{
						if ((((_arg_5 * this->topPlaneX) + (_arg_3 * this->topPlaneY)) + (_arg_7 * this->topPlaneZ)) <= this->topPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->topPlaneX) + (_arg_6 * this->topPlaneY)) + (_arg_4 * this->topPlaneZ)) > this->topPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x2F);
						}
					}
					else
					{
						if ((((_arg_5 * this->topPlaneX) + (_arg_3 * this->topPlaneY)) + (_arg_4 * this->topPlaneZ)) <= this->topPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->topPlaneX) + (_arg_6 * this->topPlaneY)) + (_arg_7 * this->topPlaneZ)) > this->topPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x2F);
						}
					}
				}
			}
			else
			{
				if (this->topPlaneY >= 0)
				{
					if (this->topPlaneZ >= 0)
					{
						if ((((_arg_2 * this->topPlaneX) + (_arg_6 * this->topPlaneY)) + (_arg_7 * this->topPlaneZ)) <= this->topPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->topPlaneX) + (_arg_3 * this->topPlaneY)) + (_arg_4 * this->topPlaneZ)) > this->topPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x2F);
						}
					}
					else
					{
						if ((((_arg_2 * this->topPlaneX) + (_arg_6 * this->topPlaneY)) + (_arg_4 * this->topPlaneZ)) <= this->topPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->topPlaneX) + (_arg_3 * this->topPlaneY)) + (_arg_7 * this->topPlaneZ)) > this->topPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x2F);
						}
					}
				}
				else
				{
					if (this->topPlaneZ >= 0)
					{
						if ((((_arg_2 * this->topPlaneX) + (_arg_3 * this->topPlaneY)) + (_arg_7 * this->topPlaneZ)) <= this->topPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->topPlaneX) + (_arg_6 * this->topPlaneY)) + (_arg_4 * this->topPlaneZ)) > this->topPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x2F);
						}
					}
					else
					{
						if ((((_arg_2 * this->topPlaneX) + (_arg_3 * this->topPlaneY)) + (_arg_4 * this->topPlaneZ)) <= this->topPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->topPlaneX) + (_arg_6 * this->topPlaneY)) + (_arg_7 * this->topPlaneZ)) > this->topPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x2F);
						}
					}
				}
			}
		}
		if ((_arg_1 & 0x20))
		{
			if (this->bottomPlaneX >= 0)
			{
				if (this->bottomPlaneY >= 0)
				{
					if (this->bottomPlaneZ >= 0)
					{
						if ((((_arg_5 * this->bottomPlaneX) + (_arg_6 * this->bottomPlaneY)) + (_arg_7 * this->bottomPlaneZ)) <= this->bottomPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->bottomPlaneX) + (_arg_3 * this->bottomPlaneY)) + (_arg_4 * this->bottomPlaneZ)) > this->bottomPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x1F);
						}
					}
					else
					{
						if ((((_arg_5 * this->bottomPlaneX) + (_arg_6 * this->bottomPlaneY)) + (_arg_4 * this->bottomPlaneZ)) <= this->bottomPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->bottomPlaneX) + (_arg_3 * this->bottomPlaneY)) + (_arg_7 * this->bottomPlaneZ)) > this->bottomPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x1F);
						}
					}
				}
				else
				{
					if (this->bottomPlaneZ >= 0)
					{
						if ((((_arg_5 * this->bottomPlaneX) + (_arg_3 * this->bottomPlaneY)) + (_arg_7 * this->bottomPlaneZ)) <= this->bottomPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->bottomPlaneX) + (_arg_6 * this->bottomPlaneY)) + (_arg_4 * this->bottomPlaneZ)) > this->bottomPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x1F);
						}
					}
					else
					{
						if ((((_arg_5 * this->bottomPlaneX) + (_arg_3 * this->bottomPlaneY)) + (_arg_4 * this->bottomPlaneZ)) <= this->bottomPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_2 * this->bottomPlaneX) + (_arg_6 * this->bottomPlaneY)) + (_arg_7 * this->bottomPlaneZ)) > this->bottomPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x1F);
						}
					}
				}
			}
			else
			{
				if (this->bottomPlaneY >= 0)
				{
					if (this->bottomPlaneZ >= 0)
					{
						if ((((_arg_2 * this->bottomPlaneX) + (_arg_6 * this->bottomPlaneY)) + (_arg_7 * this->bottomPlaneZ)) <= this->bottomPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->bottomPlaneX) + (_arg_3 * this->bottomPlaneY)) + (_arg_4 * this->bottomPlaneZ)) > this->bottomPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x1F);
						}
					}
					else
					{
						if ((((_arg_2 * this->bottomPlaneX) + (_arg_6 * this->bottomPlaneY)) + (_arg_4 * this->bottomPlaneZ)) <= this->bottomPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->bottomPlaneX) + (_arg_3 * this->bottomPlaneY)) + (_arg_7 * this->bottomPlaneZ)) > this->bottomPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x1F);
						}
					}
				}
				else
				{
					if (this->bottomPlaneZ >= 0)
					{
						if ((((_arg_2 * this->bottomPlaneX) + (_arg_3 * this->bottomPlaneY)) + (_arg_7 * this->bottomPlaneZ)) <= this->bottomPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->bottomPlaneX) + (_arg_6 * this->bottomPlaneY)) + (_arg_4 * this->bottomPlaneZ)) > this->bottomPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x1F);
						}
					}
					else
					{
						if ((((_arg_2 * this->bottomPlaneX) + (_arg_3 * this->bottomPlaneY)) + (_arg_4 * this->bottomPlaneZ)) <= this->bottomPlaneOffset)
						{
							return (-1);
						}
						if ((((_arg_5 * this->bottomPlaneX) + (_arg_6 * this->bottomPlaneY)) + (_arg_7 * this->bottomPlaneZ)) > this->bottomPlaneOffset)
						{
							_arg_1 = (_arg_1 & 0x1F);
						}
					}
				}
			}
		}
	}

	int _local_8 = 0;
	while (_local_8 < this->numOccluders)
	{
		Vertex *_local_9 = this->occluders[_local_8].get();
		while (_local_9 != nullptr)
		{
			if (_local_9->x >= 0)
			{
				if (_local_9->y >= 0)
				{
					if (_local_9->z >= 0)
					{
						if ((((_arg_5 * _local_9->x) + (_arg_6 * _local_9->y)) + (_arg_7 * _local_9->z)) > _local_9->offset) break;
					}
					else
					{
						if ((((_arg_5 * _local_9->x) + (_arg_6 * _local_9->y)) + (_arg_4 * _local_9->z)) > _local_9->offset) break;
					}
				}
				else
				{
					if (_local_9->z >= 0)
					{
						if ((((_arg_5 * _local_9->x) + (_arg_3 * _local_9->y)) + (_arg_7 * _local_9->z)) > _local_9->offset) break;
					}
					else
					{
						if ((((_arg_5 * _local_9->x) + (_arg_3 * _local_9->y)) + (_arg_4 * _local_9->z)) > _local_9->offset) break;
					}
				}
			}
			else
			{
				if (_local_9->y >= 0)
				{
					if (_local_9->z >= 0)
					{
						if ((((_arg_2 * _local_9->x) + (_arg_6 * _local_9->y)) + (_arg_7 * _local_9->z)) > _local_9->offset) break;
					}
					else
					{
						if ((((_arg_2 * _local_9->x) + (_arg_6 * _local_9->y)) + (_arg_4 * _local_9->z)) > _local_9->offset) break;
					}
				}
				else
				{
					if (_local_9->z >= 0)
					{
						if ((((_arg_2 * _local_9->x) + (_arg_3 * _local_9->y)) + (_arg_7 * _local_9->z)) > _local_9->offset) break;
					}
					else
					{
						if ((((_arg_2 * _local_9->x) + (_arg_3 * _local_9->y)) + (_arg_4 * _local_9->z)) > _local_9->offset) break;
					}
				}
			}
			_local_9 = _local_9->next.get();
		}
		if (_local_9 == nullptr)
		{
			return (-1);
		}
		_local_8++;
	}

	return _arg_1;
}


void KDContainer::calculateCameraPlanes(double _arg_1, double _arg_2)
{
	this->nearPlaneX = this->im.mc;
	this->nearPlaneY = this->im.mg;
	this->nearPlaneZ = this->im.mk;
	this->nearPlaneOffset = (((((this->im.mc * _arg_1) + this->im.md) * this->nearPlaneX) + (((this->im.mg * _arg_1) + this->im.mh) * this->nearPlaneY)) + (((this->im.mk * _arg_1) + this->im.ml) * this->nearPlaneZ));
	this->farPlaneX = -(this->im.mc);
	this->farPlaneY = -(this->im.mg);
	this->farPlaneZ = -(this->im.mk);
	this->farPlaneOffset = (((((this->im.mc * _arg_2) + this->im.md) * this->farPlaneX) + (((this->im.mg * _arg_2) + this->im.mh) * this->farPlaneY)) + (((this->im.mk * _arg_2) + this->im.ml) * this->farPlaneZ));
	double _local_3 = ((-(this->im.ma) - this->im.mb) + this->im.mc);
	double _local_4 = ((-(this->im.me) - this->im.mf) + this->im.mg);
	double _local_5 = ((-(this->im.mi) - this->im.mj) + this->im.mk);
	double _local_6 = ((this->im.ma - this->im.mb) + this->im.mc);
	double _local_7 = ((this->im.me - this->im.mf) + this->im.mg);
	double _local_8 = ((this->im.mi - this->im.mj) + this->im.mk);
	this->topPlaneX = ((_local_8 * _local_4) - (_local_7 * _local_5));
	this->topPlaneY = ((_local_6 * _local_5) - (_local_8 * _local_3));
	this->topPlaneZ = ((_local_7 * _local_3) - (_local_6 * _local_4));
	this->topPlaneOffset = (((this->im.md * this->topPlaneX) + (this->im.mh * this->topPlaneY)) + (this->im.ml * this->topPlaneZ));
	_local_3 = _local_6;
	_local_4 = _local_7;
	_local_5 = _local_8;
	_local_6 = ((this->im.ma + this->im.mb) + this->im.mc);
	_local_7 = ((this->im.me + this->im.mf) + this->im.mg);
	_local_8 = ((this->im.mi + this->im.mj) + this->im.mk);
	this->rightPlaneX = ((_local_8 * _local_4) - (_local_7 * _local_5));
	this->rightPlaneY = ((_local_6 * _local_5) - (_local_8 * _local_3));
	this->rightPlaneZ = ((_local_7 * _local_3) - (_local_6 * _local_4));
	this->rightPlaneOffset = (((this->im.md * this->rightPlaneX) + (this->im.mh * this->rightPlaneY)) + (this->im.ml * this->rightPlaneZ));
	_local_3 = _local_6;
	_local_4 = _local_7;
	_local_5 = _local_8;
	_local_6 = ((-(this->im.ma) + this->im.mb) + this->im.mc);
	_local_7 = ((-(this->im.me) + this->im.mf) + this->im.mg);
	_local_8 = ((-(this->im.mi) + this->im.mj) + this->im.mk);
	this->bottomPlaneX = ((_local_8 * _local_4) - (_local_7 * _local_5));
	this->bottomPlaneY = ((_local_6 * _local_5) - (_local_8 * _local_3));
	this->bottomPlaneZ = ((_local_7 * _local_3) - (_local_6 * _local_4));
	this->bottomPlaneOffset = (((this->im.md * this->bottomPlaneX) + (this->im.mh * this->bottomPlaneY)) + (this->im.ml * this->bottomPlaneZ));
	_local_3 = _local_6;
	_local_4 = _local_7;
	_local_5 = _local_8;
	_local_6 = ((-(this->im.ma) - this->im.mb) + this->im.mc);
	_local_7 = ((-(this->im.me) - this->im.mf) + this->im.mg);
	_local_8 = ((-(this->im.mi) - this->im.mj) + this->im.mk);
	this->leftPlaneX = ((_local_8 * _local_4) - (_local_7 * _local_5));
	this->leftPlaneY = ((_local_6 * _local_5) - (_local_8 * _local_3));
	this->leftPlaneZ = ((_local_7 * _local_3) - (_local_6 * _local_4));
	this->leftPlaneOffset = (((this->im.md * this->leftPlaneX) + (this->im.mh * this->leftPlaneY)) + (this->im.ml * this->leftPlaneZ));
}


void KDContainer::updateOccluders(Camera3D *camera)
{
	int _local_2 = this->numOccluders;

	while (_local_2 < camera->numOccluders)
	{
		Shared<Vertex> _local_3 = nullptr;
		Shared<Vertex> _local_4 = camera->occluders[_local_2];
		while (_local_4 != nullptr)
		{
			Shared<Vertex> _local_5 = _local_4->create();
			_local_5->next = _local_3;
			_local_3 = _local_5;
			double _local_6 = (((this->im.ma * _local_4->x) + (this->im.mb * _local_4->y)) + (this->im.mc * _local_4->z));
			double _local_7 = (((this->im.me * _local_4->x) + (this->im.mf * _local_4->y)) + (this->im.mg * _local_4->z));
			double _local_8 = (((this->im.mi * _local_4->x) + (this->im.mj * _local_4->y)) + (this->im.mk * _local_4->z));
			double _local_9 = (((this->im.ma * _local_4->u) + (this->im.mb * _local_4->v)) + (this->im.mc * _local_4->offset));
			double _local_10 = (((this->im.me * _local_4->u) + (this->im.mf * _local_4->v)) + (this->im.mg * _local_4->offset));
			double _local_11 = (((this->im.mi * _local_4->u) + (this->im.mj * _local_4->v)) + (this->im.mk * _local_4->offset));
			_local_3->x = ((_local_11 * _local_7) - (_local_10 * _local_8));
			_local_3->y = ((_local_9 * _local_8) - (_local_11 * _local_6));
			_local_3->z = ((_local_10 * _local_6) - (_local_9 * _local_7));
			_local_3->offset = (((this->im.md * _local_3->x) + (this->im.mh * _local_3->y)) + (this->im.ml * _local_3->z));
			_local_4 = _local_4->next;
		}
		this->occluders[this->numOccluders] = _local_3;
		this->numOccluders++;
		_local_2++;
	}
}


void KDContainer::drawNode(KDNode *_arg_1, int _arg_2, Camera3D *_arg_3, Shared<VG> _arg_4)
{
	Shared<VG> _local_7 = nullptr;
	Shared<VG> _local_8 = nullptr;
	Shared<VG> _local_9 = nullptr;



	if (_arg_3->occludedAll)
	{
		while (_arg_4 != nullptr)
		{
			Shared<VG> _local_6 = _arg_4->next;
			_arg_4->destroy();
			_arg_4 = _local_6;
		}
		return;
	}
	if (_arg_1->negative() != nullptr)
	{
		int _local_12 = (((_arg_2 > 0) || (this->numOccluders > 0)) ? this->cullingInContainer(_arg_2, _arg_1->negative()->boundMinX, _arg_1->negative()->boundMinY, _arg_1->negative()->boundMinZ, _arg_1->negative()->boundMaxX, _arg_1->negative()->boundMaxY, _arg_1->negative()->boundMaxZ) : 0);
		int _local_13 = (((_arg_2 > 0) || (this->numOccluders > 0)) ? this->cullingInContainer(_arg_2, _arg_1->positive()->boundMinX, _arg_1->positive()->boundMinY, _arg_1->positive()->boundMinZ, _arg_1->positive()->boundMaxX, _arg_1->positive()->boundMaxY, _arg_1->positive()->boundMaxZ) : 0);
		bool _local_14 = (_arg_1->axis == 0);
		bool _local_15 = (_arg_1->axis == 1);
		if (((_local_12 >= 0) && (_local_13 >= 0)))
		{
			while (_arg_4 != nullptr)
			{
				Shared<VG> _local_6 = _arg_4->next;
				if (((_arg_4->numOccluders < this->numOccluders) && (this->occludeGeometry(_arg_3, _arg_4))))
				{
					_arg_4->destroy();
				}
				else
				{
					double _local_16 = ((_local_14) ? _arg_4->boundMinX : ((_local_15) ? _arg_4->boundMinY : _arg_4->boundMinZ));
					double _local_17 = ((_local_14) ? _arg_4->boundMaxX : ((_local_15) ? _arg_4->boundMaxY : _arg_4->boundMaxZ));
					if (_local_17 <= _arg_1->maxCoord)
					{
						if (_local_16 < _arg_1->minCoord)
						{
							_arg_4->next = _local_7;
							_local_7 = _arg_4;
						}
						else
						{
							_arg_4->next = _local_8;
							_local_8 = _arg_4;
						}
					}
					else
					{
						if (_local_16 >= _arg_1->minCoord)
						{
							_arg_4->next = _local_9;
							_local_9 = _arg_4;
						}
						else
						{
							_arg_4->split(_arg_3, ((_arg_1->axis == 0) ? 1 : 0), ((_arg_1->axis == 1) ? 1 : 0), ((_arg_1->axis == 2) ? 1 : 0), _arg_1->coord, threshold);
							if (_arg_4->next != nullptr)
							{
								_arg_4->next->next = _local_7;
								_local_7 = _arg_4->next;
							}
							if (_arg_4->faceStruct != nullptr)
							{
								_arg_4->next = _local_9;
								_local_9 = _arg_4;
							}
							else
							{
								_arg_4->destroy();
							}
						}
					}
				}
				_arg_4 = _local_6;
			}
			if (((((_local_14) && (this->im.md > _arg_1->coord)) || ((_local_15) && (this->im.mh > _arg_1->coord))) || (((!(_local_14)) && (!(_local_15))) && (this->im.ml > _arg_1->coord))))
			{
				this->drawNode(_arg_1->positive(), _local_13, _arg_3, _local_9);
				while (_local_8 != nullptr)
				{
					Shared<VG> _local_6 = _local_8->next;
					if (((_local_8->numOccluders >= this->numOccluders) || (!(this->occludeGeometry(_arg_3, _local_8)))))
					{
						_local_8->draw(_arg_3, threshold, this);
					}
					_local_8->destroy();
					_local_8 = _local_6;
				}
				Object3D *_local_10 = _arg_1->objectList.get();
				Object3D *_local_11 = _arg_1->objectBoundList.get();
				while (_local_10 != nullptr)
				{
					if (((_local_10->visible) && ((((_local_10->culling = _arg_2) == 0) && (this->numOccluders == 0)) || ((_local_10->culling = this->cullingInContainer(_arg_2,
																																										 _local_11->bound.boundMinX,
																																										 _local_11->bound.boundMinY,
																																										 _local_11->bound.boundMinZ,
																																										 _local_11->bound.boundMaxX,
																																										 _local_11->bound.boundMaxY,
																																										 _local_11->bound.boundMaxZ)) >= 0))))
					{
						_local_10->copyAndAppend(_local_11, this);
						_local_10->concat(this);
						_local_10->draw(_arg_3);
					}
					_local_10 = _local_10->next.get();
					_local_11 = _local_11->next.get();
				}
				_local_10 = _arg_1->occluderList.get();
				_local_11 = _arg_1->occluderBoundList.get();
				while (_local_10 != nullptr)
				{
					if (((_local_10->visible) && ((((_local_10->culling = _arg_2) == 0) && (this->numOccluders == 0)) || ((_local_10->culling = this->cullingInContainer(_arg_2,
																																										 _local_11->bound.boundMinX,
																																										 _local_11->bound.boundMinY,
																																										 _local_11->bound.boundMinZ,
																																										 _local_11->bound.boundMaxX,
																																										 _local_11->bound.boundMaxY,
																																										 _local_11->bound.boundMaxZ)) >= 0))))
					{
						_local_10->copyAndAppend(_local_11, this);
						_local_10->concat(this);
						_local_10->draw(_arg_3);
					}
					_local_10 = _local_10->next.get();
					_local_11 = _local_11->next.get();
				}
				if (_arg_1->occluderList != nullptr)
				{
					this->updateOccluders(_arg_3);
				}
				this->drawNode(_arg_1->negative(), _local_12, _arg_3, _local_7);
			}
			else
			{
				this->drawNode(_arg_1->negative(), _local_12, _arg_3, _local_7);
				while (_local_8 != nullptr)
				{
					Shared<VG> _local_6 = _local_8->next;
					if (((_local_8->numOccluders >= this->numOccluders) || (!(this->occludeGeometry(_arg_3, _local_8)))))
					{
						_local_8->draw(_arg_3, threshold, this);
					}
					_local_8->destroy();
					_local_8 = _local_6;
				}
				Object3D *_local_10 = _arg_1->objectList.get();
				Object3D *_local_11 = _arg_1->objectBoundList.get();
				while (_local_10 != nullptr)
				{
					if (((_local_10->visible) && ((((_local_10->culling = _arg_2) == 0) && (this->numOccluders == 0)) || ((_local_10->culling = this->cullingInContainer(_arg_2,
																																										 _local_11->bound.boundMinX,
																																										 _local_11->bound.boundMinY,
																																										 _local_11->bound.boundMinZ,
																																										 _local_11->bound.boundMaxX,
																																										 _local_11->bound.boundMaxY,
																																										 _local_11->bound.boundMaxZ)) >= 0))))
					{
						_local_10->copyAndAppend(_local_11, this);
						_local_10->concat(this);
						_local_10->draw(_arg_3);
					}
					_local_10 = _local_10->next.get();
					_local_11 = _local_11->next.get();
				}
				_local_10 = _arg_1->occluderList.get();
				_local_11 = _arg_1->occluderBoundList.get();
				while (_local_10 != nullptr)
				{
					if (((_local_10->visible) && ((((_local_10->culling = _arg_2) == 0) && (this->numOccluders == 0)) || ((_local_10->culling = this->cullingInContainer(_arg_2,
																																										 _local_11->bound.boundMinX,
																																										 _local_11->bound.boundMinY,
																																										 _local_11->bound.boundMinZ,
																																										 _local_11->bound.boundMaxX,
																																										 _local_11->bound.boundMaxY,
																																										 _local_11->bound.boundMaxZ)) >= 0))))
					{
						_local_10->copyAndAppend(_local_11, this);
						_local_10->concat(this);
						_local_10->draw(_arg_3);
					}
					_local_10 = _local_10->next.get();
					_local_11 = _local_11->next.get();
				}
				if (_arg_1->occluderList != nullptr)
				{
					this->updateOccluders(_arg_3);
				}
				this->drawNode(_arg_1->positive(), _local_13, _arg_3, _local_9);
			}
		}
		else
		{
			if (_local_12 >= 0)
			{
				while (_arg_4 != nullptr)
				{
					Shared<VG> _local_6 = _arg_4->next;
					if (((_arg_4->numOccluders < this->numOccluders) && (this->occludeGeometry(_arg_3, _arg_4))))
					{
						_arg_4->destroy();
					}
					else
					{
						double _local_16 = ((_local_14) ? _arg_4->boundMinX : ((_local_15) ? _arg_4->boundMinY : _arg_4->boundMinZ));
						double _local_17 = ((_local_14) ? _arg_4->boundMaxX : ((_local_15) ? _arg_4->boundMaxY : _arg_4->boundMaxZ));
						if (_local_17 <= _arg_1->maxCoord)
						{
							_arg_4->next = _local_7;
							_local_7 = _arg_4;
						}
						else
						{
							if (_local_16 >= _arg_1->minCoord)
							{
								_arg_4->destroy();
							}
							else
							{
								_arg_4->crop(_arg_3, ((_arg_1->axis == 0) ? -1 : 0), ((_arg_1->axis == 1) ? -1 : 0), ((_arg_1->axis == 2) ? -1 : 0), -(_arg_1->coord), threshold);
								if (_arg_4->faceStruct != nullptr)
								{
									_arg_4->next = _local_7;
									_local_7 = _arg_4;
								}
								else
								{
									_arg_4->destroy();
								}
							}
						}
					}
					_arg_4 = _local_6;
				}
				this->drawNode(_arg_1->negative(), _local_12, _arg_3, _local_7);
			}
			else
			{
				if (_local_13 >= 0)
				{
					while (_arg_4 != nullptr)
					{
						Shared<VG> _local_6 = _arg_4->next;
						if (((_arg_4->numOccluders < this->numOccluders) && (this->occludeGeometry(_arg_3, _arg_4))))
						{
							_arg_4->destroy();
						}
						else
						{
							double _local_16 = ((_local_14) ? _arg_4->boundMinX : ((_local_15) ? _arg_4->boundMinY : _arg_4->boundMinZ));
							double _local_17 = ((_local_14) ? _arg_4->boundMaxX : ((_local_15) ? _arg_4->boundMaxY : _arg_4->boundMaxZ));
							if (_local_17 <= _arg_1->maxCoord)
							{
								_arg_4->destroy();
							}
							else
							{
								if (_local_16 >= _arg_1->minCoord)
								{
									_arg_4->next = _local_9;
									_local_9 = _arg_4;
								}
								else
								{
									_arg_4->crop(_arg_3, ((_arg_1->axis == 0) ? 1 : 0), ((_arg_1->axis == 1) ? 1 : 0), ((_arg_1->axis == 2) ? 1 : 0), _arg_1->coord, threshold);
									if (_arg_4->faceStruct != nullptr)
									{
										_arg_4->next = _local_9;
										_local_9 = _arg_4;
									}
									else
									{
										_arg_4->destroy();
									}
								}
							}
						}
						_arg_4 = _local_6;
					}
					this->drawNode(_arg_1->positive(), _local_13, _arg_3, _local_9);
				}
				else
				{
					while (_arg_4 != nullptr)
					{
						Shared<VG> _local_6 = _arg_4->next;
						_arg_4->destroy();
						_arg_4 = _local_6;
					}
				}
			}
		}
	}
	else
	{
		if (_arg_1->objectList != nullptr)
		{
			if (((!(_arg_1->objectList->next == nullptr)) || (!(_arg_4 == nullptr))))
			{
				while (_arg_4 != nullptr)
				{
					Shared<VG> _local_6 = _arg_4->next;
					if (((_arg_4->numOccluders < this->numOccluders) && (this->occludeGeometry(_arg_3, _arg_4))))
					{
						_arg_4->destroy();
					}
					else
					{
						_arg_4->next = _local_8;
						_local_8 = _arg_4;
					}
					_arg_4 = _local_6;
				}
				Object3D *_local_10 = _arg_1->objectList.get();
				Object3D *_local_11 = _arg_1->objectBoundList.get();
				while (_local_10 != nullptr)
				{
					if (((_local_10->visible) && ((((_local_10->culling = _arg_2) == 0) && (this->numOccluders == 0)) || ((_local_10->culling = this->cullingInContainer(_arg_2,
																																										 _local_11->bound.boundMinX,
																																										 _local_11->bound.boundMinY,
																																										 _local_11->bound.boundMinZ,
																																										 _local_11->bound.boundMaxX,
																																										 _local_11->bound.boundMaxY,
																																										 _local_11->bound.boundMaxZ)) >= 0))))
					{
						_local_10->copyAndAppend(_local_11, this);
						_local_10->concat(this);
						_arg_4 = _local_10->getVG(_arg_3);
						while (_arg_4 != nullptr)
						{
							Shared<VG> _local_6 = _arg_4->next;
							_arg_4->next = _local_8;
							_local_8 = _arg_4;
							_arg_4 = _local_6;
						}
					}
					_local_10 = _local_10->next.get();
					_local_11 = _local_11->next.get();
				}
				if (_local_8 != nullptr)
				{
					if (_local_8->next != nullptr)
					{
						drawConflictGeometry(_arg_3, _local_8);
					}
					else
					{
						_local_8->draw(_arg_3, threshold, this);
						_local_8->destroy();
					}
				}
			}
			else
			{
				Object3D *_local_10 = _arg_1->objectList.get();
				if (_local_10->visible)
				{
					_local_10->copyAndAppend(_arg_1->objectBoundList.get(), this);
					_local_10->culling = _arg_2;
					_local_10->concat(this);
					_local_10->draw(_arg_3);
				}
			}
		}
		else
		{
			if (_arg_4 != nullptr)
			{
				if (_arg_4->next != nullptr)
				{
					if (this->numOccluders > 0)
					{
						while (_arg_4 != nullptr)
						{
							Shared<VG> _local_6 = _arg_4->next;
							if (((_arg_4->numOccluders < this->numOccluders) && (this->occludeGeometry(_arg_3, _arg_4))))
							{
								_arg_4->destroy();
							}
							else
							{
								_arg_4->next = _local_8;
								_local_8 = _arg_4;
							}
							_arg_4 = _local_6;
						}
						if (_local_8 != nullptr)
						{
							if (_local_8->next != nullptr)
							{
								if (resolveByAABB)
								{
									drawAABBGeometry(_arg_3, _local_8);
								}
								else
								{
									if (resolveByOOBB)
									{
										_arg_4 = _local_8;
										while (_arg_4 != nullptr)
										{
											_arg_4->calculateOOBB(this);
											_arg_4 = _arg_4->next;
										}
										drawOOBBGeometry(_arg_3, _local_8);
									}
									else
									{
										drawConflictGeometry(_arg_3, _local_8);
									}
								}
							}
							else
							{
								_local_8->draw(_arg_3, threshold, this);
								_local_8->destroy();
							}
						}
					}
					else
					{
						_local_8 = _arg_4;
						if (resolveByAABB)
						{
							drawAABBGeometry(_arg_3, _local_8);
						}
						else
						{
							if (resolveByOOBB)
							{
								_arg_4 = _local_8;
								while (_arg_4 != nullptr)
								{
									_arg_4->calculateOOBB(this);
									_arg_4 = _arg_4->next;
								}
								drawOOBBGeometry(_arg_3, _local_8);
							}
							else
							{
								drawConflictGeometry(_arg_3, _local_8);
							}
						}
					}
				}
				else
				{
					if (((_arg_4->numOccluders >= this->numOccluders) || (!(this->occludeGeometry(_arg_3, _arg_4)))))
					{
						_arg_4->draw(_arg_3, threshold, this);
					}
					_arg_4->destroy();
				}
			}
		}

		Object3D *_local_10 = _arg_1->occluderList.get();
		Object3D *_local_11 = _arg_1->occluderBoundList.get();
		while (_local_10 != nullptr)
		{
			if (((_local_10->visible) && ((((_local_10->culling = _arg_2) == 0) && (this->numOccluders == 0)) || ((_local_10->culling = this->cullingInContainer(_arg_2,
																																								 _local_11->bound.boundMinX,
																																								 _local_11->bound.boundMinY,
																																								 _local_11->bound.boundMinZ,
																																								 _local_11->bound.boundMaxX,
																																								 _local_11->bound.boundMaxY,
																																								 _local_11->bound.boundMaxZ)) >= 0))))
			{
				_local_10->copyAndAppend(_local_11, this);
				_local_10->concat(this);
				_local_10->draw(_arg_3);
			}
			_local_10 = _local_10->next.get();
			_local_11 = _local_11->next.get();
		}
		if (_arg_1->occluderList != nullptr)
		{
			this->updateOccluders(_arg_3);
		}
	}
}


bool KDContainer::occludeGeometry(Camera3D *camera, Shared<VG> _arg_2)
{
	(void)camera;

	int _local_3 = _arg_2->numOccluders;
	while (_local_3 < this->numOccluders)
	{
		Vertex *_local_4 = this->occluders[_local_3].get();
		while (_local_4 != nullptr)
		{
			if (_local_4->x >= 0)
			{
				if (_local_4->y >= 0)
				{
					if (_local_4->z >= 0)
					{
						if ((((_arg_2->boundMaxX * _local_4->x) + (_arg_2->boundMaxY * _local_4->y)) + (_arg_2->boundMaxZ * _local_4->z)) > _local_4->offset) break;
					}
					else
					{
						if ((((_arg_2->boundMaxX * _local_4->x) + (_arg_2->boundMaxY * _local_4->y)) + (_arg_2->boundMinZ * _local_4->z)) > _local_4->offset) break;
					}
				}
				else
				{
					if (_local_4->z >= 0)
					{
						if ((((_arg_2->boundMaxX * _local_4->x) + (_arg_2->boundMinY * _local_4->y)) + (_arg_2->boundMaxZ * _local_4->z)) > _local_4->offset) break;
					}
					else
					{
						if ((((_arg_2->boundMaxX * _local_4->x) + (_arg_2->boundMinY * _local_4->y)) + (_arg_2->boundMinZ * _local_4->z)) > _local_4->offset) break;
					}
				}
			}
			else
			{
				if (_local_4->y >= 0)
				{
					if (_local_4->z >= 0)
					{
						if ((((_arg_2->boundMinX * _local_4->x) + (_arg_2->boundMaxY * _local_4->y)) + (_arg_2->boundMaxZ * _local_4->z)) > _local_4->offset) break;
					}
					else
					{
						if ((((_arg_2->boundMinX * _local_4->x) + (_arg_2->boundMaxY * _local_4->y)) + (_arg_2->boundMinZ * _local_4->z)) > _local_4->offset) break;
					}
				}
				else
				{
					if (_local_4->z >= 0)
					{
						if ((((_arg_2->boundMinX * _local_4->x) + (_arg_2->boundMinY * _local_4->y)) + (_arg_2->boundMaxZ * _local_4->z)) > _local_4->offset) break;
					}
					else
					{
						if ((((_arg_2->boundMinX * _local_4->x) + (_arg_2->boundMinY * _local_4->y)) + (_arg_2->boundMinZ * _local_4->z)) > _local_4->offset) break;
					}
				}
			}
			_local_4 = _local_4->next.get();
		}
		if (_local_4 == nullptr)
		{
			return true;
		}
		_local_3++;
	}

	_arg_2->numOccluders = this->numOccluders;
	return false;
}



namespace
{
	struct LocalVertex
	{
		double x, y, z;
		double cameraX, cameraY, cameraZ;
	};
}


std::shared_ptr<Decal> KDContainer::createDecal(const Vector3D *_arg_1,
												const Vector3D *_arg_2,
												double _arg_3,
												double _arg_4,
												double _arg_5,
												double _arg_6,
												std::shared_ptr<Material> _arg_7)
{

	std::shared_ptr<Decal> result = new_Decal();

	Decal *_local_8 = result.get();

	_local_8->attenuation = _arg_6;

	Matrix3D _local_9;

	_local_9.identity();

	_local_9.appendRotation(((_arg_4 * 180) / MyMath::PI), Vector3D::Z_AXIS);

	_local_9.appendRotation((((MyMath::atan2(-(_arg_2->z),
											 MyMath::sqrt(((_arg_2->x * _arg_2->x) + (_arg_2->y * _arg_2->y)))) * 180) / MyMath::PI) - 90),
							Vector3D::X_AXIS);


	_local_9.appendRotation(((-(MyMath::atan2(-(_arg_2->x), -(_arg_2->y))) * 180) / MyMath::PI), Vector3D::Z_AXIS);

	_local_9.appendTranslation(_arg_1->x, _arg_1->y, _arg_1->z);


	_local_8->setMatrix(&_local_9);

	_local_8->composeMatrix();
	_local_8->bound.boundMinX = -(_arg_3);
	_local_8->bound.boundMaxX = _arg_3;
	_local_8->bound.boundMinY = -(_arg_3);
	_local_8->bound.boundMaxY = _arg_3;
	_local_8->bound.boundMinZ = -(_arg_6);
	_local_8->bound.boundMaxZ = _arg_6;

	double _local_10 = 1E22;
	double _local_11 = 1E22;
	double _local_12 = 1E22;
	double _local_13 = -1E22;
	double _local_14 = -1E22;
	double _local_15 = -1E22;



	LocalVertex boundVertexList[8];

	LocalVertex *_local_16;

	_local_16 = &boundVertexList[0];
	_local_16->x = _local_8->bound.MinX;
	_local_16->y = _local_8->bound.MinY;
	_local_16->z = _local_8->bound.MinZ;
	_local_16 = &boundVertexList[1];
	_local_16->x = _local_8->bound.MaxX;
	_local_16->y = _local_8->bound.MinY;
	_local_16->z = _local_8->bound.MinZ;
	_local_16 = &boundVertexList[2];
	_local_16->x = _local_8->bound.MinX;
	_local_16->y = _local_8->bound.MaxY;
	_local_16->z = _local_8->bound.MinZ;
	_local_16 = &boundVertexList[3];
	_local_16->x = _local_8->bound.MaxX;
	_local_16->y = _local_8->bound.MaxY;
	_local_16->z = _local_8->bound.MinZ;
	_local_16 = &boundVertexList[4];
	_local_16->x = _local_8->bound.MinX;
	_local_16->y = _local_8->bound.MinY;
	_local_16->z = _local_8->bound.MaxZ;
	_local_16 = &boundVertexList[5];
	_local_16->x = _local_8->bound.MaxX;
	_local_16->y = _local_8->bound.MinY;
	_local_16->z = _local_8->bound.MaxZ;
	_local_16 = &boundVertexList[6];
	_local_16->x = _local_8->bound.MinX;
	_local_16->y = _local_8->bound.MaxY;
	_local_16->z = _local_8->bound.MaxZ;
	_local_16 = &boundVertexList[7];
	_local_16->x = _local_8->bound.MaxX;
	_local_16->y = _local_8->bound.MaxY;
	_local_16->z = _local_8->bound.MaxZ;



	for (int i = 0; i < 8; i++)
	{
		_local_16 = &boundVertexList[i];

		_local_16->cameraX = ((((_local_8->matrix4.ma * _local_16->x) + (_local_8->matrix4.mb * _local_16->y)) + (_local_8->matrix4.mc * _local_16->z)) + _local_8->matrix4.md);
		_local_16->cameraY = ((((_local_8->matrix4.me * _local_16->x) + (_local_8->matrix4.mf * _local_16->y)) + (_local_8->matrix4.mg * _local_16->z)) + _local_8->matrix4.mh);
		_local_16->cameraZ = ((((_local_8->matrix4.mi * _local_16->x) + (_local_8->matrix4.mj * _local_16->y)) + (_local_8->matrix4.mk * _local_16->z)) + _local_8->matrix4.ml);
		if (_local_16->cameraX < _local_10)
		{
			_local_10 = _local_16->cameraX;
		}
		if (_local_16->cameraX > _local_13)
		{
			_local_13 = _local_16->cameraX;
		}
		if (_local_16->cameraY < _local_11)
		{
			_local_11 = _local_16->cameraY;
		}
		if (_local_16->cameraY > _local_14)
		{
			_local_14 = _local_16->cameraY;
		}
		if (_local_16->cameraZ < _local_12)
		{
			_local_12 = _local_16->cameraZ;
		}
		if (_local_16->cameraZ > _local_15)
		{
			_local_15 = _local_16->cameraZ;
		}
		//_local_16 = _local_16->next;
	}

	_local_8->invertMatrix();

	if (_arg_5 > (MyMath::PI / 2))
	{
		_arg_5 = (MyMath::PI / 2);
	}

	if (this->root != nullptr)
	{
		this->root->collectPolygons(_local_8, MyMath::sqrt((((_arg_3 * _arg_3) + (_arg_3 * _arg_3)) + (_arg_6 * _arg_6))), (MyMath::cos(_arg_5) - 0.001), _local_10, _local_13, _local_11, _local_14, _local_12, _local_15);
	}

	if (_local_8->faceList != nullptr)
	{
		_local_8->calculateBounds();
	}
	else
	{
		_local_8->bound.MinX = -1;
		_local_8->bound.MinY = -1;
		_local_8->bound.MinZ = -1;
		_local_8->bound.MaxX = 1;
		_local_8->bound.MaxY = 1;
		_local_8->bound.MaxZ = 1;
	}

	_local_8->setMaterialToAllFaces(_arg_7);

	return result;
}
