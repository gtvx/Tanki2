#include "Mesh.h"
#include "MyMath.h"
#include "hardware/alternativa/engine3d/core/Wrapper.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include "flash/geom/Vector3D.h"
#include "flash/geom/Vector3D.h"
#include "_global.h"
#include "Error.h"
#include "../core/Sorting.h"
#include "../core/Clipping.h"
#include "../core/Object3DContainer.h"
#include "../materials/Material.h"
#include "../core/Camera3D.h"
#include "hardware/alternativa/gfx/core/VertexBufferResource.h"
#include "hardware/alternativa/gfx/core/IndexBufferResource.h"
#include "../core/VG.h"
#include "../core/Faces.h"
#include "qhash_shared_ptr.h"
#include <QMap>
#include <QVector>
#include <QDebug>


std::shared_ptr<Mesh> new_Mesh()
{
	return std::make_shared<Mesh>();
}


Mesh::Mesh()
{
	faceList = nullptr;
	vertexList = nullptr;
	sorting = Sorting::AVERAGE_Z;
	threshold = 0.01;
	clipping = Clipping::FACE_CLIPPING;
	transformId = 0;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	opaqueLength = 0;
	transparentList = nullptr;
	numTriangles = 0;
	numOpaqueTriangles = 0;
}


static void set(QVector<Shared<Vertex>> *vector, int index, Shared<Vertex> &value)
{
	if (vector->count() <= index)
		vector->resize(index + 1);
	vector->replace(index, value);
}


static void set(QVector<int> *vector, int index, int value)
{
	if (vector->count() <= index)
		vector->resize(index+1);
	(*vector)[index] = value;
}


static void group(QVector<Shared<Vertex>> *_arg_1, int _arg_2, int _arg_3, int _arg_4, double _arg_5, double _arg_6, QVector<int> *_arg_7)
{
	int _local_8 = 0;
	double _local_11 = NaN();

	switch (_arg_4)
	{
	case 0:
		_local_8 = _arg_2;
		while (_local_8 < _arg_3)
		{
			Vertex *_local_10 = _arg_1->at(_local_8).get();
			_local_10->offset = _local_10->x;
			_local_8++;
		}
		_local_11 = _arg_5;
		break;
	case 1:
		_local_8 = _arg_2;
		while (_local_8 < _arg_3)
		{
			Vertex *_local_10 = _arg_1->at(_local_8).get();
			_local_10->offset = _local_10->y;
			_local_8++;
		}
		_local_11 = _arg_5;
		break;
	case 2:
		_local_8 = _arg_2;
		while (_local_8 < _arg_3)
		{
			Vertex *_local_10 = _arg_1->at(_local_8).get();
			_local_10->offset = _local_10->z;
			_local_8++;
		}
		_local_11 = _arg_5;
		break;
	case 3:
		_local_8 = _arg_2;
		while (_local_8 < _arg_3)
		{
			Vertex *_local_10 = _arg_1->at(_local_8).get();
			_local_10->offset = _local_10->u;
			_local_8++;
		}
		_local_11 = _arg_6;
		break;
	case 4:
		_local_8 = _arg_2;
		while (_local_8 < _arg_3)
		{
			Vertex *_local_10 = _arg_1->at(_local_8).get();
			_local_10->offset = _local_10->v;
			_local_8++;
		}
		_local_11 = _arg_6;
		break;
	}

	set(_arg_7, 0, _arg_2);
	set(_arg_7, 1, _arg_3 - 1);

	int _local_12 = 2;
	while (_local_12 > 0)
	{
		_local_12--;
		int _local_14 = (*_arg_7)[_local_12];
		int _local_9 = _local_14;
		_local_12--;
		int _local_15 = (*_arg_7)[_local_12];
		_local_8 = _local_15;
		Vertex *_local_10 = _arg_1->at((_local_14 + _local_15) >> 1).get();
		double _local_16 = _local_10->offset;
		while (_local_8 <= _local_9)
		{
			Shared<Vertex> _local_17 = _arg_1->at(_local_8);
			while (_local_17->offset > _local_16)
			{
				_local_8++;
				_local_17 = _arg_1->at(_local_8);
			}
			Shared<Vertex> _local_18 = _arg_1->at(_local_9);
			while (_local_18->offset < _local_16)
			{
				_local_9--;
				_local_18 = _arg_1->at(_local_9);
			}
			if (_local_8 <= _local_9)
			{
				set(_arg_1, _local_8, _local_18);
				set(_arg_1, _local_9, _local_17);
				_local_8++;
				_local_9--;
			}
		}
		if (_local_15 < _local_9)
		{
			set(_arg_7, _local_12, _local_15);
			_local_12++;
			set(_arg_7, _local_12, _local_9);
			_local_12++;
		}
		if (_local_8 < _local_14)
		{
			set(_arg_7, _local_12, _local_8);
			_local_12++;
			set(_arg_7, _local_12, _local_14);
			_local_12++;
		}
	}

	Vertex *_local_13 = nullptr;

	_local_8 = _arg_2;
	Shared<Vertex> _local_10 = _arg_1->value(_local_8);
	int _local_9 = (_local_8 + 1);
	while (_local_9 <= _arg_3)
	{
		if (_local_9 < _arg_3)
		{
			_local_13 = _arg_1->at(_local_9).get();
		}
		if (((_local_9 == _arg_3) || ((_local_10->offset - _local_13->offset) > _local_11)))
		{
			if (((_arg_4 < 4) && ((_local_9 - _local_8) > 1)))
			{
				group(_arg_1, _local_8, _local_9, (_arg_4 + 1), _arg_5, _arg_6, _arg_7);
			}
			if (_local_9 < _arg_3)
			{
				_local_8 = _local_9;
				_local_10 = _arg_1->value(_local_8);
			}
		}
		else
		{
			if (_arg_4 == 4)
			{
				_local_13->value = _local_10;
			}
		}
		_local_9++;
	}
}


void Mesh::calculateFacesNormals(bool param1)
{
	Shared<Face> face = this->faceList;
	while (face != nullptr)
	{
		Shared<Wrapper> wrapper = face->wrapper;
		Shared<Vertex> vertex1 = wrapper->vertex;
		wrapper = wrapper->next;
		Shared<Vertex> vertex2 = wrapper->vertex;
		wrapper = wrapper->next;
		Shared<Vertex> vertex3 = wrapper->vertex;
		double _loc7_ = vertex2->x - vertex1->x;
		double _loc8_ = vertex2->y - vertex1->y;
		double _loc9_ = vertex2->z - vertex1->z;
		double _loc10_ = vertex3->x - vertex1->x;
		double _loc11_ = vertex3->y - vertex1->y;
		double _loc12_ = vertex3->z - vertex1->z;
		double _loc13_ = _loc12_ * _loc8_ - _loc11_ * _loc9_;
		double _loc14_ = _loc10_ * _loc9_ - _loc12_ * _loc7_;
		double _loc15_ = _loc11_ * _loc7_ - _loc10_ * _loc8_;
		if (param1 == true) {
			double _loc16_ = _loc13_ * _loc13_ + _loc14_ * _loc14_ + _loc15_ * _loc15_;
			if (_loc16_ > 0.001) {
				_loc16_ = 1 / MyMath::sqrt(_loc16_);
				_loc13_ = _loc13_ * _loc16_;
				_loc14_ = _loc14_ * _loc16_;
				_loc15_ = _loc15_ * _loc16_;
			}
		}
		face->normalX = _loc13_;
		face->normalY = _loc14_;
		face->normalZ = _loc15_;
		face->offset = vertex1->x * _loc13_ + vertex1->y * _loc14_ + vertex1->z * _loc15_;
		face = face->next;
	}
}


void Mesh::updateBounds(Object3D *param1, Object3D *param2)
{
	Vertex *vertex = this->vertexList.get();
	while (vertex != nullptr)
	{
		if (param2 != nullptr)
		{
			vertex->cameraX = param2->matrix4.ma * vertex->x + param2->matrix4.mb * vertex->y + param2->matrix4.mc * vertex->z + param2->matrix4.md;
			vertex->cameraY = param2->matrix4.me * vertex->x + param2->matrix4.mf * vertex->y + param2->matrix4.mg * vertex->z + param2->matrix4.mh;
			vertex->cameraZ = param2->matrix4.mi * vertex->x + param2->matrix4.mj * vertex->y + param2->matrix4.mk * vertex->z + param2->matrix4.ml;
		}
		else
		{
			vertex->cameraX = vertex->x;
			vertex->cameraY = vertex->y;
			vertex->cameraZ = vertex->z;
		}
		if (vertex->cameraX < param1->bound.boundMinX)
		{
			param1->bound.boundMinX = vertex->cameraX;
		}
		if (vertex->cameraX > param1->bound.boundMaxX)
		{
			param1->bound.boundMaxX = vertex->cameraX;
		}
		if (vertex->cameraY < param1->bound.boundMinY)
		{
			param1->bound.boundMinY = vertex->cameraY;
		}
		if (vertex->cameraY > param1->bound.boundMaxY)
		{
			param1->bound.boundMaxY = vertex->cameraY;
		}
		if (vertex->cameraZ < param1->bound.boundMinZ)
		{
			param1->bound.boundMinZ = vertex->cameraZ;
		}
		if (vertex->cameraZ > param1->bound.boundMaxZ)
		{
			param1->bound.boundMaxZ = vertex->cameraZ;
		}
		vertex = vertex->next.get();
	}
}


void Mesh::optimizeForDynamicBSP(int param1)
{
	Shared<Face> _loc3_ = nullptr;
	Shared<Face> _loc2_ = this->faceList;

	int _loc4_ = 0;
	while (_loc4_ < param1)
	{
		Shared<Face> _loc5_ = nullptr;
		Shared<Face> _loc6_ = _loc2_;
		while (_loc6_ != nullptr) //Face
		{
			double _loc7_ = _loc6_->normalX;
			double _loc8_ = _loc6_->normalY;
			double _loc9_ = _loc6_->normalZ;
			double _loc10_ = _loc6_->offset;
			double _loc11_ = _loc10_ - this->threshold;
			double _loc12_ = _loc10_ + this->threshold;
			int _loc13_ = 0;
			Shared<Face> _loc14_ = _loc2_;

			while (_loc14_ != nullptr) //Face
			{
				if (_loc14_ != _loc6_)
				{
					Shared<Wrapper> _loc15_ = _loc14_->wrapper;
					Shared<Vertex> _loc16_ = _loc15_->vertex;
					_loc15_ = _loc15_->next;
					Shared<Vertex> _loc17_ = _loc15_->vertex;
					_loc15_ = _loc15_->next;
					Shared<Vertex> _loc18_ = _loc15_->vertex;
					_loc15_ = _loc15_->next;
					double _loc19_ = _loc16_->x * _loc7_ + _loc16_->y * _loc8_ + _loc16_->z * _loc9_;
					double _loc20_ = _loc17_->x * _loc7_ + _loc17_->y * _loc8_ + _loc17_->z * _loc9_;
					double _loc21_ = _loc18_->x * _loc7_ + _loc18_->y * _loc8_ + _loc18_->z * _loc9_;
					bool _loc22_ = _loc19_ < _loc11_ || _loc20_ < _loc11_ || _loc21_ < _loc11_;
					bool _loc23_ = _loc19_ > _loc12_ || _loc20_ > _loc12_ || _loc21_ > _loc12_;
					while (_loc15_ != nullptr) //Wrapper
					{
						Shared<Vertex> _loc24_ = _loc15_->vertex;
						double _loc25_ = _loc24_->x * _loc7_ + _loc24_->y * _loc8_ + _loc24_->z * _loc9_;
						if (_loc25_ < _loc11_)
						{
							_loc22_ = true;
							if (_loc23_)
							{
								break;
							}
						}
						else if (_loc25_ > _loc12_)
						{
							_loc23_ = true;
							if (_loc22_)
							{
								break;
							}
						}
						_loc15_ = _loc15_->next;
					}
					if (_loc23_ && _loc22_)
					{
						_loc13_++;
						if (_loc13_ > _loc4_)
						{
							break;
						}
					}
				}
				_loc14_ = _loc14_->next;
			}

			if (_loc14_ == nullptr)
			{

				if (_loc5_ != nullptr)
				{
					_loc5_->next = _loc6_->next;
				}
				else
				{
					_loc2_ = _loc6_->next;
				}

				if (_loc3_ != nullptr)
				{
					_loc3_->next = _loc6_;
				}
				else
				{
					this->faceList = _loc6_;
				}

				_loc3_ = _loc6_;
			}
			else
			{
				_loc5_ = _loc6_;
			}

			_loc6_ = _loc6_->next;
		}

		if (_loc2_ == nullptr)
		{
			break;
		}

		_loc4_++;
	}

	if (_loc3_ != nullptr)
	{
		_loc3_->next = _loc2_;
	}
}


void Mesh::weldFaces(double _arg_1, double _arg_2, double _arg_3, bool _arg_4)
{
	Shared<Face> _local_8 = nullptr;
	Shared<Face> _local_10 = nullptr;
	Shared<Wrapper> _local_11 = nullptr;
	Shared<Wrapper> _local_12 = nullptr;
	Shared<Wrapper> _local_13 = nullptr;
	Shared<Wrapper> _local_14 = nullptr;
	Shared<Wrapper> _local_15 = nullptr;
	Shared<Wrapper> _local_16 = nullptr;
	Shared<Wrapper> _local_17 = nullptr;
	Shared<Wrapper> _local_18 = nullptr;
	Shared<Vertex> _local_19 = nullptr;
	Shared<Vertex> _local_20 = nullptr;
	Shared<Vertex> _local_21 = nullptr;
	Shared<Vertex> _local_22 = nullptr;
	double _local_23;
	double _local_24;
	double _local_25;
	double _local_26;
	double _local_27;
	double _local_28;
	double _local_29;
	double _local_30;
	double _local_31;
	double _local_32;
	double _local_33;
	double _local_34;
	double _local_35;
	double _local_36;
	double _local_45;
	double _local_46;
	double _local_47;
	double _local_48;
	double _local_49;
	double _local_50;
	double _local_51;
	double _local_52;
	double _local_53;
	double _local_54;
	double _local_55;
	double _local_56;
	double _local_57;
	double _local_58;
	double _local_59;
	double _local_60;
	double _local_61;
	double _local_62 = 0;
	double _local_63 = 0;
	bool _local_64;

	Shared<Face> _local_65 = nullptr;

	this->deleteResources();

	double _local_38 = 0.001;

	_arg_1 = MyMath::cos(_arg_1) - _local_38;

	_arg_2 = (_arg_2 + _local_38);

	_arg_3 = MyMath::cos((MyMath::PI - _arg_3)) - _local_38;

	QMap<Shared<Face>, bool> _local_39;

	QMap<Shared<Vertex>, std::shared_ptr<QMap<Shared<Face>, bool>>> _local_40;

	Shared<Face> _local_9 = this->faceList;

	while (_local_9 != nullptr)
	{
		_local_10 = _local_9->next;
		_local_9->next = nullptr;
		_local_20 = _local_9->wrapper->vertex;
		_local_21 = _local_9->wrapper->next->vertex;
		_local_22 = _local_9->wrapper->next->next->vertex;
		_local_23 = (_local_21->x - _local_20->x);
		_local_24 = (_local_21->y - _local_20->y);
		_local_25 = (_local_21->z - _local_20->z);
		_local_28 = (_local_22->x - _local_20->x);
		_local_29 = (_local_22->y - _local_20->y);
		_local_30 = (_local_22->z - _local_20->z);
		_local_33 = ((_local_30 * _local_24) - (_local_29 * _local_25));
		_local_34 = ((_local_28 * _local_25) - (_local_30 * _local_23));
		_local_35 = ((_local_29 * _local_23) - (_local_28 * _local_24));
		_local_36 = (((_local_33 * _local_33) + (_local_34 * _local_34)) + (_local_35 * _local_35));
		if (_local_36 > _local_38)
		{
			_local_36 = (1 / MyMath::sqrt(_local_36));
			_local_33 = (_local_33 * _local_36);
			_local_34 = (_local_34 * _local_36);
			_local_35 = (_local_35 * _local_36);
			_local_9->normalX = _local_33;
			_local_9->normalY = _local_34;
			_local_9->normalZ = _local_35;
			_local_9->offset = (((_local_20->x * _local_33) + (_local_20->y * _local_34)) + (_local_20->z * _local_35));
			_local_39[_local_9] = true;
			_local_15 = _local_9->wrapper;
			while (_local_15 != nullptr)
			{
				_local_19 = _local_15->vertex;
				auto _local_37 = _local_40.value(_local_19);
				if (_local_37 == nullptr)
				{
					_local_37 = std::make_shared<QMap<Shared<Face>, bool>>();
					_local_40[_local_19] = _local_37;
				}
				_local_37->insert(_local_9, true);
				_local_15 = _local_15->next;
			}
		}
		_local_9 = _local_10;
	}

	this->faceList = nullptr;

	QVector<Shared<Face>> _local_41;
	QMap<Shared<Face>, bool> _local_42;
	QMap<Shared<Face>, bool> _local_43;

	while (true)
	{
		_local_9 = nullptr;

		{
			QMapIterator<Shared<Face>, bool> iter(_local_39);
			while (iter.hasNext())
			{
				iter.next();
				Shared<Face> face = iter.key();

				if (iter.value() == false)
				{
					continue;
				}

				_local_9 = face;
				_local_39.remove(_local_9);
				break;
			}
		}

		if (_local_9 == nullptr)
			break;


		int _local_44 = 0;

		if (_local_41.length() <= _local_44)
			_local_41.resize(_local_44 + 1);

		_local_41[_local_44] = _local_9;
		_local_44++;
		_local_20 = _local_9->wrapper->vertex;
		_local_21 = _local_9->wrapper->next->vertex;
		_local_22 = _local_9->wrapper->next->next->vertex;
		_local_23 = (_local_21->x - _local_20->x);
		_local_24 = (_local_21->y - _local_20->y);
		_local_25 = (_local_21->z - _local_20->z);
		_local_26 = (_local_21->u - _local_20->u);
		_local_27 = (_local_21->v - _local_20->v);
		_local_28 = (_local_22->x - _local_20->x);
		_local_29 = (_local_22->y - _local_20->y);
		_local_30 = (_local_22->z - _local_20->z);
		_local_31 = (_local_22->u - _local_20->u);
		_local_32 = (_local_22->v - _local_20->v);
		_local_33 = _local_9->normalX;
		_local_34 = _local_9->normalY;
		_local_35 = _local_9->normalZ;
		_local_45 = (((((((-(_local_33) * _local_29) * _local_25) + ((_local_28 * _local_34) * _local_25)) + ((_local_33 * _local_24) * _local_30)) - ((_local_23 * _local_34) * _local_30)) - ((_local_28 * _local_24) * _local_35)) + ((_local_23 * _local_29) * _local_35));
		_local_46 = (((-(_local_34) * _local_30) + (_local_29 * _local_35)) / _local_45);
		_local_47 = (((_local_33 * _local_30) - (_local_28 * _local_35)) / _local_45);
		_local_48 = (((-(_local_33) * _local_29) + (_local_28 * _local_34)) / _local_45);
		_local_49 = ((((((((_local_20->x * _local_34) * _local_30) - ((_local_33 * _local_20->y) * _local_30)) - ((_local_20->x * _local_29) * _local_35)) + ((_local_28 * _local_20->y) * _local_35)) + ((_local_33 * _local_29) * _local_20->z)) - ((_local_28 * _local_34) * _local_20->z)) / _local_45);
		_local_50 = (((_local_34 * _local_25) - (_local_24 * _local_35)) / _local_45);
		_local_51 = (((-(_local_33) * _local_25) + (_local_23 * _local_35)) / _local_45);
		_local_52 = (((_local_33 * _local_24) - (_local_23 * _local_34)) / _local_45);
		_local_53 = ((((((((_local_33 * _local_20->y) * _local_25) - ((_local_20->x * _local_34) * _local_25)) + ((_local_20->x * _local_24) * _local_35)) - ((_local_23 * _local_20->y) * _local_35)) - ((_local_33 * _local_24) * _local_20->z)) + ((_local_23 * _local_34) * _local_20->z)) / _local_45);
		_local_54 = ((_local_26 * _local_46) + (_local_31 * _local_50));
		_local_55 = ((_local_26 * _local_47) + (_local_31 * _local_51));
		_local_56 = ((_local_26 * _local_48) + (_local_31 * _local_52));
		_local_57 = (((_local_26 * _local_49) + (_local_31 * _local_53)) + _local_20->u);
		_local_58 = ((_local_27 * _local_46) + (_local_32 * _local_50));
		_local_59 = ((_local_27 * _local_47) + (_local_32 * _local_51));
		_local_60 = ((_local_27 * _local_48) + (_local_32 * _local_52));
		_local_61 = (((_local_27 * _local_49) + (_local_32 * _local_53)) + _local_20->v);

		_local_43.clear();


		int _local_5 = 0;

		while (_local_5 < _local_44)
		{
			_local_9 = _local_41[_local_5];

			_local_42.clear();


			_local_13 = _local_9->wrapper;

			while (_local_13 != nullptr)
			{
				QMapIterator<Shared<Face>, bool> iter(*_local_40[_local_13->vertex]);
				while (iter.hasNext())
				{
					iter.next();
					Shared<Face> _local_7 = iter.key();
					if (((_local_39[_local_7]) && (!(_local_43[_local_7]))))
					{
						_local_42[_local_7] = true;
					}
				}
				_local_13 = _local_13->next;
			}

			QMapIterator<Shared<Face>, bool> iter(_local_42);
			while (iter.hasNext())
			{
				iter.next();
				Shared<Face> _local_7 = iter.key();

				_local_8 = _local_7;
				if ((((_local_33 * _local_8->normalX) + (_local_34 * _local_8->normalY)) + (_local_35 * _local_8->normalZ)) >= _arg_1)
				{
					_local_14 = _local_8->wrapper;
					while (_local_14 != nullptr)
					{
						_local_19 = _local_14->vertex;
						_local_62 = (((((_local_54 * _local_19->x) + (_local_55 * _local_19->y)) + (_local_56 * _local_19->z)) + _local_57) - _local_19->u);
						_local_63 = (((((_local_58 * _local_19->x) + (_local_59 * _local_19->y)) + (_local_60 * _local_19->z)) + _local_61) - _local_19->v);
						if (((((_local_62 > _arg_2) || (_local_62 < -(_arg_2))) || (_local_63 > _arg_2)) || (_local_63 < -(_arg_2)))) break;
						_local_14 = _local_14->next;
					}
					if (_local_14 == nullptr)
					{
						_local_13 = _local_9->wrapper;
						while (_local_13 != nullptr)
						{
							_local_15 = ((_local_13->next != nullptr) ? _local_13->next : _local_9->wrapper);
							_local_14 = _local_8->wrapper;
							while (_local_14 != nullptr)
							{
								_local_16 = ((_local_14->next != nullptr) ? _local_14->next : _local_8->wrapper);
								if (((_local_13->vertex == _local_16->vertex) && (_local_15->vertex == _local_14->vertex))) break;
								_local_14 = _local_14->next;
							}
							if (_local_14 != nullptr) break;
							_local_13 = _local_13->next;
						}
						if (_local_13 != nullptr)
						{
							if (_local_41.length() <= _local_44)
								_local_41.resize(_local_44 + 1);

							_local_41[_local_44] = _local_8;


							_local_44++;

							_local_39.remove(_local_8); //delete _local_39[_local_8];
						}
					}
					else
					{
						_local_43[_local_8] = true;
					}
				}
				else
				{
					_local_43[_local_8] = true;
				}
			}
			_local_5++;
		}

		if (_local_44 == 1)
		{
			_local_9 = _local_41[0];
			_local_9->next = this->faceList;
			this->faceList = _local_9;
		}
		else
		{
			while (true)
			{
				_local_64 = false;
				_local_5 = 0;
				while (_local_5 < (_local_44 - 1))
				{
					_local_9 = _local_41[_local_5];
					if (_local_9 != nullptr)
					{
						int _local_6 = 1;
						for (;_local_6 < _local_44;_local_6++)
						{
							_local_8 = _local_41[_local_6];
							if (_local_8 != nullptr)
							{
								_local_13 = _local_9->wrapper;
								while (_local_13 != nullptr)
								{
									_local_15 = ((_local_13->next != nullptr) ? _local_13->next : _local_9->wrapper);
									_local_14 = _local_8->wrapper;
									while (_local_14 != nullptr)
									{
										_local_16 = ((_local_14->next != nullptr) ? _local_14->next : _local_8->wrapper);
										if (((_local_13->vertex == _local_16->vertex) && (_local_15->vertex == _local_14->vertex))) break;
										_local_14 = _local_14->next;
									}
									if (_local_14 != nullptr) break;
									_local_13 = _local_13->next;
								}
								if (_local_13 != nullptr)
								{
									while (true)
									{
										_local_17 = ((_local_15->next != nullptr) ? _local_15->next : _local_9->wrapper);
										_local_12 = _local_8->wrapper;
										while (((!(_local_12->next == _local_14)) && (!(_local_12->next == nullptr))))
										{
											_local_12 = _local_12->next;
										}
										if (_local_17->vertex == _local_12->vertex)
										{
											_local_15 = _local_17;
											_local_14 = _local_12;
										}
										else
										{
											break;
										}
									}
									while (true)
									{
										_local_11 = _local_9->wrapper;
										while (((!(_local_11->next == _local_13)) && (!(_local_11->next == nullptr))))
										{
											_local_11 = _local_11->next;
										}
										_local_18 = ((_local_16->next != nullptr) ? _local_16->next : _local_8->wrapper);
										if (_local_11->vertex == _local_18->vertex)
										{
											_local_13 = _local_11;
											_local_16 = _local_18;
										}
										else
										{
											break;
										}
									}
									_local_20 = _local_13->vertex;
									_local_21 = _local_18->vertex;
									_local_22 = _local_11->vertex;
									_local_23 = (_local_21->x - _local_20->x);
									_local_24 = (_local_21->y - _local_20->y);
									_local_25 = (_local_21->z - _local_20->z);
									_local_28 = (_local_22->x - _local_20->x);
									_local_29 = (_local_22->y - _local_20->y);
									_local_30 = (_local_22->z - _local_20->z);
									_local_33 = ((_local_30 * _local_24) - (_local_29 * _local_25));
									_local_34 = ((_local_28 * _local_25) - (_local_30 * _local_23));
									_local_35 = ((_local_29 * _local_23) - (_local_28 * _local_24));

									if (((((((_local_33 < _local_38) && (_local_33 > -(_local_38))) && (_local_34 < _local_38)) && (_local_34 > -(_local_38))) && (_local_35 < _local_38)) && (_local_35 > -(_local_38))))
									{
										if ((((_local_23 * _local_28) + (_local_24 * _local_29)) + (_local_25 * _local_30)) > 0) continue;
									}
									else
									{
										if ((((_local_9->normalX * _local_33) + (_local_9->normalY * _local_34)) + (_local_9->normalZ * _local_35)) < 0) continue;
									}

									_local_36 = (1 / MyMath::sqrt((((_local_23 * _local_23) + (_local_24 * _local_24)) + (_local_25 * _local_25))));
									_local_23 = (_local_23 * _local_36);
									_local_24 = (_local_24 * _local_36);
									_local_25 = (_local_25 * _local_36);
									_local_36 = (1 / MyMath::sqrt((((_local_28 * _local_28) + (_local_29 * _local_29)) + (_local_30 * _local_30))));
									_local_28 = (_local_28 * _local_36);
									_local_29 = (_local_29 * _local_36);
									_local_30 = (_local_30 * _local_36);
									if ((((_local_23 * _local_28) + (_local_24 * _local_29)) + (_local_25 * _local_30)) >= _arg_3)
									{
										_local_20 = _local_14->vertex;
										_local_21 = _local_17->vertex;
										_local_22 = _local_12->vertex;
										_local_23 = (_local_21->x - _local_20->x);
										_local_24 = (_local_21->y - _local_20->y);
										_local_25 = (_local_21->z - _local_20->z);
										_local_28 = (_local_22->x - _local_20->x);
										_local_29 = (_local_22->y - _local_20->y);
										_local_30 = (_local_22->z - _local_20->z);
										_local_33 = ((_local_30 * _local_24) - (_local_29 * _local_25));
										_local_34 = ((_local_28 * _local_25) - (_local_30 * _local_23));
										_local_35 = ((_local_29 * _local_23) - (_local_28 * _local_24));
										if (((((((_local_33 < _local_38) && (_local_33 > -(_local_38))) && (_local_34 < _local_38)) && (_local_34 > -(_local_38))) && (_local_35 < _local_38)) && (_local_35 > -(_local_38))))
										{
											if ((((_local_23 * _local_28) + (_local_24 * _local_29)) + (_local_25 * _local_30)) > 0) continue;
										}
										else
										{
											if ((((_local_9->normalX * _local_33) + (_local_9->normalY * _local_34)) + (_local_9->normalZ * _local_35)) < 0) continue;
										}
										_local_36 = (1 / MyMath::sqrt((((_local_23 * _local_23) + (_local_24 * _local_24)) + (_local_25 * _local_25))));
										_local_23 = (_local_23 * _local_36);
										_local_24 = (_local_24 * _local_36);
										_local_25 = (_local_25 * _local_36);
										_local_36 = (1 / MyMath::sqrt((((_local_28 * _local_28) + (_local_29 * _local_29)) + (_local_30 * _local_30))));
										_local_28 = (_local_28 * _local_36);
										_local_29 = (_local_29 * _local_36);
										_local_30 = (_local_30 * _local_36);
										if ((((_local_23 * _local_28) + (_local_24 * _local_29)) + (_local_25 * _local_30)) >= _arg_3)
										{
											_local_64 = true;
											_local_65 = Face::create(22);
											_local_65->material = _local_9->material;
											_local_65->smoothingGroups = _local_9->smoothingGroups;
											_local_65->normalX = _local_9->normalX;
											_local_65->normalY = _local_9->normalY;
											_local_65->normalZ = _local_9->normalZ;
											_local_65->offset = _local_9->offset;
											//_local_65->id = _local_9->id;
											_local_17 = nullptr;
											while (_local_15 != _local_13)
											{
												_local_18 = Wrapper::create();
												_local_18->vertex = _local_15->vertex;
												if (_local_17 != nullptr)
												{
													_local_17->next = _local_18;
												}
												else
												{
													_local_65->wrapper = _local_18;
												}
												_local_17 = _local_18;
												_local_15 = ((_local_15->next != nullptr) ? _local_15->next : _local_9->wrapper);
											}
											while (_local_16 != _local_14)
											{
												_local_18 = Wrapper::create();
												_local_18->vertex = _local_16->vertex;
												if (_local_17 != nullptr)
												{
													_local_17->next = _local_18;
												}
												else
												{
													_local_65->wrapper = _local_18;
												}
												_local_17 = _local_18;
												_local_16 = ((_local_16->next != nullptr) ? _local_16->next : _local_8->wrapper);
											}

											_local_41[_local_5] = _local_65;
											_local_41[_local_6] = nullptr;
											_local_9 = _local_65;
											if (_arg_4) break;
										}
									}
								}
							}
						}
					}
					_local_5++;
				}
				if (!_local_64)
					break;
			}

			_local_5 = 0;
			while (_local_5 < _local_44)
			{
				_local_9 = _local_41[_local_5];
				if (_local_9 != nullptr)
				{
					_local_9->calculateBestSequenceAndNormal();
					_local_9->next = this->faceList;
					this->faceList = _local_9;
				}
				_local_5++;
			}
		}
	}
}


void Mesh::weldVertices(double param1, double param2)
{
	QVector<Shared<Vertex>> _loc5_;
	int  _loc6_ = 0;

	Shared<Vertex> _loc3_ = this->vertexList;

	while (_loc3_ != nullptr)
	{
		Shared<Vertex> _loc4_ = _loc3_->next;
		_loc3_->next = nullptr;
		_loc5_.append(_loc3_);
		_loc6_++;
		_loc3_ = _loc4_;
	}

	this->vertexList = nullptr;

	QVector<int> v;

	group(&_loc5_, 0, _loc6_, 0, param1, param2, &v);

	Shared<Face> _loc7_ = this->faceList;

	while (_loc7_ != nullptr)
	{
		Weak<Wrapper> _loc9_ = _loc7_->wrapper;
		while (_loc9_ != nullptr)
		{
			if (_loc9_->vertex->value != nullptr)
			{
				_loc9_->vertex = _loc9_->vertex->value;
			}
			_loc9_ = _loc9_->next;
		}

		_loc7_ = _loc7_->next;
	}

	int _loc8_ = 0;
	while (_loc8_ < _loc6_)
	{
		_loc3_ = _loc5_.at(_loc8_);
		if (_loc3_->value == nullptr)
		{
			_loc3_->next = this->vertexList;
			this->vertexList = _loc3_;
		}
		_loc8_++;
	}
}


void Mesh::calculateNormalsAndRemoveDegenerateFaces()
{
	Shared<Face> _local_1 = this->faceList;
	this->faceList = nullptr;
	while (_local_1 != nullptr)
	{
		Shared<Face> _local_2 = _local_1->next;
		Shared<Wrapper> _local_3 = _local_1->wrapper;
		Shared<Vertex> _local_4 = _local_3->vertex;
		_local_3 = _local_3->next;
		Shared<Vertex> _local_5 = _local_3->vertex;
		_local_3 = _local_3->next;
		Shared<Vertex> _local_6 = _local_3->vertex;
		double _local_7 = (_local_5->x - _local_4->x);
		double _local_8 = (_local_5->y - _local_4->y);
		double _local_9 = (_local_5->z - _local_4->z);
		double _local_10 = (_local_6->x - _local_4->x);
		double _local_11 = (_local_6->y - _local_4->y);
		double _local_12 = (_local_6->z - _local_4->z);
		_local_1->normalX = ((_local_12 * _local_8) - (_local_11 * _local_9));
		_local_1->normalY = ((_local_10 * _local_9) - (_local_12 * _local_7));
		_local_1->normalZ = ((_local_11 * _local_7) - (_local_10 * _local_8));
		double _local_13 = (((_local_1->normalX * _local_1->normalX) + (_local_1->normalY * _local_1->normalY)) + (_local_1->normalZ * _local_1->normalZ));
		if (_local_13 > 0.001)
		{
			_local_13 = (1 / MyMath::sqrt(_local_13));
			_local_1->normalX = (_local_1->normalX * _local_13);
			_local_1->normalY = (_local_1->normalY * _local_13);
			_local_1->normalZ = (_local_1->normalZ * _local_13);
			_local_1->offset = (((_local_4->x * _local_1->normalX) + (_local_4->y * _local_1->normalY)) + (_local_4->z * _local_1->normalZ));
			_local_1->next = this->faceList;
			this->faceList = _local_1;
		}
		else
		{
			_local_1->next = nullptr;
		}
		_local_1 = _local_2;
	}
}


static void shareFaces(QVector<Shared<Vertex>> &_arg_1,
					   int _arg_2,
					   int _arg_3,
					   int _arg_4,
					   double _arg_5,
					   QVector<int> &_arg_6,
					   QMap<Shared<Vertex>, std::shared_ptr<QMap<Shared<Face>, bool>>> &_arg_7)
{
	int _local_8 = 0;
	Shared<Vertex> _local_11 = nullptr;
	Shared<Vertex> _local_13 = nullptr;

	switch (_arg_4)
	{
		case 0:
			_local_8 = _arg_2;
			while (_local_8 < _arg_3)
			{
				_local_11 = _arg_1[_local_8];
				_local_11->offset = _local_11->x;
				_local_8++;
			}
			break;
		case 1:
			_local_8 = _arg_2;
			while (_local_8 < _arg_3)
			{
				_local_11 = _arg_1[_local_8];
				_local_11->offset = _local_11->y;
				_local_8++;
			}
			break;
		case 2:
			_local_8 = _arg_2;
			while (_local_8 < _arg_3)
			{
				_local_11 = _arg_1[_local_8];
				_local_11->offset = _local_11->z;
				_local_8++;
			}
			break;
	}

	if (_arg_6.length() < 2)
		_arg_6.resize(2);

	_arg_6[0] = _arg_2;
	_arg_6[1] = (_arg_3 - 1);
	int _local_12 = 2;
	while (_local_12 > 0)
	{
		_local_12--;
		int _local_14 = _arg_6[_local_12];
		int _local_9 = _local_14;
		_local_12--;
		int _local_15 = _arg_6[_local_12];
		_local_8 = _local_15;
		_local_11 = _arg_1[((_local_14 + _local_15) >> 1)];
		double _local_16 = _local_11->offset;
		while (_local_8 <= _local_9)
		{
			Shared<Vertex> _local_17 = _arg_1[_local_8];
			while (_local_17->offset > _local_16)
			{
				_local_8++;
				_local_17 = _arg_1[_local_8];
			}
			Shared<Vertex> _local_18 = _arg_1[_local_9];
			while (_local_18->offset < _local_16)
			{
				_local_9--;
				_local_18 = _arg_1[_local_9];
			}
			if (_local_8 <= _local_9)
			{
				_arg_1[_local_8] = _local_18;
				_arg_1[_local_9] = _local_17;
				_local_8++;
				_local_9--;
			}
		}
		if (_local_15 < _local_9)
		{
			if (_arg_6.length() <= _local_12)
				_arg_6.resize(_local_12 + 1);

			_arg_6[_local_12] = _local_15;
			_local_12++;

			if (_arg_6.length() <= _local_12)
				_arg_6.resize(_local_12 + 1);

			_arg_6[_local_12] = _local_9;
			_local_12++;
		}
		if (_local_8 < _local_14)
		{
			if (_arg_6.length() <= _local_12)
				_arg_6.resize(_local_12 + 1);

			_arg_6[_local_12] = _local_8;
			_local_12++;

			if (_arg_6.length() <= _local_12)
				_arg_6.resize(_local_12 + 1);

			_arg_6[_local_12] = _local_14;
			_local_12++;
		}
	}

	_local_8 = _arg_2;
	_local_11 = _arg_1[_local_8];
	int _local_9 = (_local_8 + 1);
	while (_local_9 <= _arg_3)
	{
		if (_local_9 < _arg_3)
		{
			_local_13 = _arg_1[_local_9];
		}
		if (((_local_9 == _arg_3) || ((_local_11->offset - _local_13->offset) > _arg_5)))
		{
			if ((_local_9 - _local_8) > 1)
			{
				if (_arg_4 < 2)
				{
					shareFaces(_arg_1, _local_8, _local_9, (_arg_4 + 1), _arg_5, _arg_6, _arg_7);
				}
				else
				{
					int _local_10 = (_local_8 + 1);
					while (_local_10 < _local_9)
					{
						_local_13 = _arg_1[_local_10];

						QMapIterator<Shared<Face>, bool> i(*_arg_7[_local_13]);
						while (i.hasNext())
						{
							i.next();
							Shared<Face> _local_19 = i.key();
							_arg_7[_local_11]->insert(_local_19, true);
						}
						_local_10++;
					}
					_local_10 = (_local_8 + 1);
					while (_local_10 < _local_9)
					{
						_local_13 = _arg_1[_local_10];


						QMapIterator<Shared<Face>, bool> i(*_arg_7[_local_11]);
						while (i.hasNext())
						{
							i.next();
							Shared<Face> _local_19 = i.key();
							_arg_7[_local_13]->insert(_local_19, true);
						}

						_local_10++;
					}
				}
			}
			if (_local_9 < _arg_3)
			{
				_local_8 = _local_9;
				_local_11 = _arg_1[_local_8];
			}
		}
		_local_9++;
	}
}


void Mesh::calculateVerticesNormalsByAngle(double _arg_1, double _arg_2)
{
	this->deleteResources();

	this->calculateNormalsAndRemoveDegenerateFaces();


	QMap<Shared<Vertex>, std::shared_ptr<QMap<Shared<Face>, bool>>> _local_6;

	Shared<Vertex> _local_5 = this->vertexList;
	while (_local_5 != nullptr)
	{
		_local_6[_local_5] = std::make_shared<QMap<Shared<Face>, bool>>();
		_local_5 = _local_5->next;
	}

	int i = 0;
	int j = 0;

	Shared<Face> _local_3 = this->faceList;
	while (_local_3 != nullptr)
	{
		i++;
		Weak<Wrapper> _local_4 = _local_3->wrapper;
		while (_local_4 != nullptr)
		{
			j++;
			Shared<Vertex> _local_5 = _local_4->vertex;
			_local_6[_local_5]->insert(_local_3, true);
			_local_4 = _local_4->next;
		}
		_local_3 = _local_3->next;
	}

	QVector<Shared<Vertex>> _local_7 = this->getVertices();

	QVector<int> v;

	shareFaces(_local_7, 0, _local_7.length(), 0, _arg_2, v, _local_6);
	this->vertexList = nullptr;
	_arg_1 = MyMath::cos(_arg_1);
	_local_3 = this->faceList;
	while (_local_3 != nullptr)
	{
		Weak<Wrapper> _local_4 = _local_3->wrapper;
		while (_local_4 != nullptr)
		{
			Shared<Vertex>  _local_5 = _local_4->vertex;
			Shared<Vertex> _local_8 = Vertex::create();
			_local_8->x = _local_5->x;
			_local_8->y = _local_5->y;
			_local_8->z = _local_5->z;
			_local_8->u = _local_5->u;
			_local_8->v = _local_5->v;
			//_local_8->id = _local_5.id;
			_local_8->normalX = _local_3->normalX;
			_local_8->normalY = _local_3->normalY;
			_local_8->normalZ = _local_3->normalZ;

			QMapIterator<Shared<Face>, bool> i(*_local_6[_local_5]);
			while (i.hasNext())
			{
				i.next();
				Shared<Face> _local_9 = i.key();

				Shared<Face> _local_11 = _local_9;
				if (((!(_local_3 == _local_11)) && ((((_local_3->normalX * _local_11->normalX) + (_local_3->normalY * _local_11->normalY)) + (_local_3->normalZ * _local_11->normalZ)) >= _arg_1)))
				{
					_local_8->normalX = (_local_8->normalX + _local_11->normalX);
					_local_8->normalY = (_local_8->normalY + _local_11->normalY);
					_local_8->normalZ = (_local_8->normalZ + _local_11->normalZ);
				}
			}

			double _local_10 = (((_local_8->normalX * _local_8->normalX) + (_local_8->normalY * _local_8->normalY)) + (_local_8->normalZ * _local_8->normalZ));
			if (_local_10 > 0.001)
			{
				_local_10 = (1 / MyMath::sqrt(_local_10));
				_local_8->normalX = (_local_8->normalX * _local_10);
				_local_8->normalY = (_local_8->normalY * _local_10);
				_local_8->normalZ = (_local_8->normalZ * _local_10);
			}
			_local_4->vertex = _local_8;
			_local_8->next = this->vertexList;
			this->vertexList = _local_8;
			_local_4 = _local_4->next;
		}
		_local_3 = _local_3->next;
	}

}


std::shared_ptr<Object3D> Mesh::clone() const
{
	std::shared_ptr<Mesh> mesh = new_Mesh();
	mesh->clonePropertiesFrom(this);
	return mesh;
}


void Mesh::clonePropertiesFrom(const Object3D *in)
{
	Mesh *src = (Mesh*)in;

	Shared<Vertex> _loc4_ = nullptr;
	Shared<Face> _loc5_ = nullptr;
	Shared<Wrapper> _loc9_ = nullptr;
	Object3D::clonePropertiesFrom(src);


	this->clipping = src->clipping;
	this->sorting = src->sorting;
	this->threshold = src->threshold;
	Shared<Vertex> _loc3_ = src->vertexList;
	while (_loc3_ != nullptr)
	{
		Shared<Vertex> _loc7_ = Vertex::create();
		_loc7_->x = _loc3_->x;
		_loc7_->y = _loc3_->y;
		_loc7_->z = _loc3_->z;
		_loc7_->u = _loc3_->u;
		_loc7_->v = _loc3_->v;
		_loc7_->normalX = _loc3_->normalX;
		_loc7_->normalY = _loc3_->normalY;
		_loc7_->normalZ = _loc3_->normalZ;
		_loc7_->offset = _loc3_->offset;
		//_loc7_->id = _loc3_->id;
		_loc3_->value = _loc7_;
		if (_loc4_ != nullptr)
		{
			_loc4_->next = _loc7_;
		}
		else
		{
			this->vertexList = _loc7_;
		}
		_loc4_ = _loc7_;
		_loc3_ = _loc3_->next;
	}



	Shared<Face> _loc6_ = src->faceList;
	while (_loc6_ != nullptr)
	{
		Shared<Face> _loc8_ = Face::create(23);
		_loc8_->material = _loc6_->material;
		_loc8_->smoothingGroups = _loc6_->smoothingGroups;
		//_loc8_->id = _loc6_->id;
		_loc8_->normalX = _loc6_->normalX;
		_loc8_->normalY = _loc6_->normalY;
		_loc8_->normalZ = _loc6_->normalZ;
		_loc8_->offset = _loc6_->offset;
		_loc9_ = nullptr;
		Weak<Wrapper> _loc10_ = _loc6_->wrapper;
		while (_loc10_ != nullptr)
		{
			Shared<Wrapper> _loc11_ = Wrapper::create();
			_loc11_->vertex = _loc10_->vertex->value;
			if (_loc9_ != nullptr)
			{
				_loc9_->next = _loc11_;
			}
			else
			{
				_loc8_->wrapper = _loc11_;
			}
			_loc9_ = _loc11_;
			_loc10_ = _loc10_->next;
		}


		if (_loc5_ != nullptr)
		{
			_loc5_->next = _loc8_;
		}
		else
		{
			this->faceList = _loc8_;
		}

		_loc5_ = _loc8_;
		_loc6_ = _loc6_->next;
	}
	_loc3_ = src->vertexList;
	while (_loc3_ != nullptr)
	{
		_loc3_->value = nullptr;
		_loc3_ = _loc3_->next;
	}
}


void Mesh::setMaterialToAllFaces(std::shared_ptr<Material> material)
{
	this->deleteResources();
	Weak<Face> face = this->faceList;
	while (face != nullptr)
	{
		face->material = material;
		face = face->next;
	}
}


void Mesh::collectPlanes(const Vector3D *_arg_1, const Vector3D *_arg_2, const Vector3D *_arg_3,
						 const Vector3D *_arg_4, const Vector3D *_arg_5, Faces *_arg_6, void *_arg_7)
{


	QMap<Object3D*, void*> *_param7 = (QMap<Object3D*, void*>*)_arg_7;

	if (_param7 != nullptr && _param7->contains(this))
	{
		return;
	}


	QVector<Shared<Face>>* __arg_6 = _arg_6->faces;


	Vector3D _local_8;

	calculateSphere(_arg_1, _arg_2, _arg_3, _arg_4, _arg_5, &_local_8);

	if (!boundIntersectSphere(&_local_8, bound.boundMinX, bound.boundMinY, bound.boundMinZ, bound.boundMaxX, bound.boundMaxY, bound.boundMaxZ))
	{
		return;
	}

	if (transformId > 0x1DCD6500)
	{
		transformId = 0;
		Weak<Vertex> vertex = this->vertexList;
		while (vertex != nullptr)
		{
			vertex->transformId = 0;
			vertex = vertex->next;
		}
	}

	transformId++;

	Shared<Face> _local_10 = this->faceList;
	while (_local_10 != nullptr)
	{
		double _local_11 = ((((_local_8.x * _local_10->normalX) + (_local_8.y * _local_10->normalY)) + (_local_8.z * _local_10->normalZ)) - _local_10->offset);

		if (((_local_11 < _local_8.w) && (_local_11 > -(_local_8.w))))
		{
			Weak<Wrapper> _local_12 = _local_10->wrapper;
			while (_local_12 != nullptr)
			{
				Weak<Vertex> _local_9 = _local_12->vertex;
				if (_local_9->transformId != transformId)
				{
					_local_9->cameraX = ((((matrix4.ma * _local_9->x) + (matrix4.mb * _local_9->y)) + (matrix4.mc * _local_9->z)) + matrix4.md);
					_local_9->cameraY = ((((matrix4.me * _local_9->x) + (matrix4.mf * _local_9->y)) + (matrix4.mg * _local_9->z)) + matrix4.mh);
					_local_9->cameraZ = ((((matrix4.mi * _local_9->x) + (matrix4.mj * _local_9->y)) + (matrix4.mk * _local_9->z)) + matrix4.ml);
					_local_9->transformId = transformId;
				}
				_local_12 = _local_12->next;
			}
			__arg_6->append(_local_10);
		}
		_local_10 = _local_10->next;
	}
}


QVector<Shared<Face>> Mesh::getFaces()
{
	QVector<Shared<Face>> result;

	Shared<Face> face = this->faceList;
	while (face != nullptr)
	{
		result.append(face);
		face = face->next;
	}

	return result;
}


QVector<Shared<Vertex>> Mesh::getVertices()
{
	QVector<Shared<Vertex>> result;

	Shared<Vertex> vertex = this->vertexList;
	while (vertex != nullptr)
	{
		result.append(vertex);
		vertex = vertex->next;
	}

	return result;
}


Shared<Vertex> Mesh::removeVertex(Shared<Vertex> _arg_1)
{
	Shared<Vertex> _local_3 = nullptr;
	Shared<Face> _local_5 = nullptr;

	this->deleteResources();

	if (_arg_1 == nullptr)
	{
		throw TypeError("Parameter vertex must be non-null.");
	}

	Shared<Vertex> _local_2 = this->vertexList;
	while (_local_2 != nullptr)
	{
		if (_local_2 == _arg_1)
		{
			if (_local_3 != nullptr)
			{
				_local_3->next = _local_2->next;
			}
			else
			{
				this->vertexList = _local_2->next;
			}
			_local_2->next = nullptr;
			break;
		}
		_local_3 = _local_2;
		_local_2 = _local_2->next;
	}

	if (_local_2 == nullptr)
	{
		throw ArgumentError("Vertex not found.");
	}

	Shared<Face> _local_4 = this->faceList;
	while (_local_4 != nullptr)
	{
		Shared<Face> _local_6 = _local_4->next;
		Weak<Wrapper> wrapper = _local_4->wrapper;
		while (wrapper != nullptr)
		{
			if (wrapper->vertex == _local_2)
				break;
			wrapper = wrapper->next;
		}
		if (wrapper != nullptr)
		{
			if (_local_5 != nullptr)
			{
				_local_5->next = _local_6;
			}
			else
			{
				this->faceList = _local_6;
			}
			_local_4->next = nullptr;
		}
		else
		{
			_local_5 = _local_4;
		}
		_local_4 = _local_6;
	}

	return _local_2;
}


Shared<Vertex> Mesh::addVertex(double x,
							   double y,
							   double z,
							   double u,
							   double v,
							   void *id)
{
	(void)id;
	this->deleteResources();

	Shared<Vertex> _local_7 = Vertex::create();

	_local_7->x = x;
	_local_7->y = y;
	_local_7->z = z;
	_local_7->u = u;
	_local_7->v = v;
	//_local_7->id = id;

	if (this->vertexList != nullptr)
	{
		Weak<Vertex> _local_8 = this->vertexList;
		while (_local_8->next != nullptr)
		{
			_local_8 = _local_8->next;
		}
		_local_8->next = _local_7;
	}
	else
	{
		this->vertexList = _local_7;
	}

	return _local_7;
}


void Mesh::deleteResources()
{
	if (this->vertexBuffer != nullptr)
	{
		//this->vertexBuffer.dispose();
		this->vertexBuffer = nullptr;
		//this->indexBuffer.dispose();
		this->indexBuffer = nullptr;
		this->numTriangles = 0;
		this->numOpaqueTriangles = 0;
		this->opaqueMaterials.clear();
		this->opaqueBegins.clear();
		this->opaqueNums.clear();
		this->opaqueLength = 0;
		this->transparentList = nullptr;
	}
}


void Mesh::calculateVerticesNormalsBySmoothingGroupsForMeshList(QVector<std::shared_ptr<Object3D>> &_arg_1, double _arg_2)
{
	QMap<Shared<Vertex>, std::shared_ptr<QMap<Shared<Face>, bool>>> _local_12;

	int _local_13 = _arg_1.length();

	int _local_3 = 0;
	while (_local_3 < _local_13)
	{
		std::shared_ptr<Mesh> _local_8 = std::dynamic_pointer_cast<Mesh>(_arg_1[_local_3]);
		if (_local_8 != nullptr)
		{
			_local_8->deleteResources();
			_local_8->composeMatrix();
			std::shared_ptr<Object3D> _local_16 = _local_8;
			while (_local_16->_parent != nullptr)
			{
				_local_16 = _local_16->_parent;
				_local_16->composeMatrix();
				_local_8->appendMatrix(_local_16.get());
			}
			Vertex *_local_10 = _local_8->vertexList.get();
			while (_local_10 != nullptr)
			{
				double _local_4 = _local_10->x;
				double _local_5 = _local_10->y;
				double _local_6 = _local_10->z;
				_local_10->x = ((((_local_8->matrix4.ma * _local_4) + (_local_8->matrix4.mb * _local_5)) + (_local_8->matrix4.mc * _local_6)) + _local_8->matrix4.md);
				_local_10->y = ((((_local_8->matrix4.me * _local_4) + (_local_8->matrix4.mf * _local_5)) + (_local_8->matrix4.mg * _local_6)) + _local_8->matrix4.mh);
				_local_10->z = ((((_local_8->matrix4.mi * _local_4) + (_local_8->matrix4.mj * _local_5)) + (_local_8->matrix4.mk * _local_6)) + _local_8->matrix4.ml);
				_local_10 = _local_10->next.get();
			}
			_local_8->calculateNormalsAndRemoveDegenerateFaces();
			Shared<Face> _local_9 = _local_8->faceList;
			while (_local_9 != nullptr)
			{
				if (_local_9->smoothingGroups > 0)
				{
					Weak<Wrapper> _local_11 = _local_9->wrapper;
					while (_local_11 != nullptr)
					{
						Shared<Vertex> _local_10 = _local_11->vertex;
						if ((!(_local_12[_local_10])))
						{
							_local_12[_local_10] = std::make_shared<QMap<Shared<Face>, bool>>();
						}
						_local_12[_local_10]->insert(_local_9, true);
						_local_11 = _local_11->next;
					}
				}
				_local_9 = _local_9->next;
			}
		}
		_local_3++;
	}

	QVector<Shared<Vertex>> _local_14;
	_local_14.resize(_local_12.count());

	int _local_15 = 0;

	QMapIterator<Shared<Vertex>, std::shared_ptr<QMap<Shared<Face>, bool>>> iter(_local_12);
	while (iter.hasNext())
	{
		iter.next();
		_local_14[_local_15] = iter.key();
		_local_15++;
	}

	if (_local_15 > 0)
	{
		QVector<int> v;
		shareFaces(_local_14, 0, _local_15, 0, _arg_2, v, _local_12);
	}

	_local_3 = 0;
	while (_local_3 < _local_13)
	{
		std::shared_ptr<Mesh> _local_8 = std::dynamic_pointer_cast<Mesh>(_arg_1[_local_3]);
		if (_local_8 != nullptr)
		{
			_local_8->vertexList = nullptr;
			Shared<Face> _local_9 = _local_8->faceList;
			while (_local_9 != nullptr)
			{
				Weak<Wrapper> _local_11 = _local_9->wrapper;
				while (_local_11 != nullptr)
				{
					Shared<Vertex> _local_10 = _local_11->vertex;
					Shared<Vertex> _local_17 = Vertex::create();
					_local_17->x = _local_10->x;
					_local_17->y = _local_10->y;
					_local_17->z = _local_10->z;
					_local_17->u = _local_10->u;
					_local_17->v = _local_10->v;
					//_local_17->id = _local_10.id;
					_local_17->normalX = _local_9->normalX;
					_local_17->normalY = _local_9->normalY;
					_local_17->normalZ = _local_9->normalZ;
					if (_local_9->smoothingGroups > 0)
					{

						QMapIterator<Shared<Face>, bool> iter(*_local_12[_local_10]);
						while (iter.hasNext())
						{
							iter.next();
							Shared<Face> _local_7 = iter.key();
							Shared<Face> _local_19 = _local_7;
							if (((!(_local_9 == _local_19)) && ((_local_9->smoothingGroups & _local_19->smoothingGroups) > 0)))
							{
								_local_17->normalX = (_local_17->normalX + _local_19->normalX);
								_local_17->normalY = (_local_17->normalY + _local_19->normalY);
								_local_17->normalZ = (_local_17->normalZ + _local_19->normalZ);
							}
						}
						double _local_18 = (((_local_17->normalX * _local_17->normalX) + (_local_17->normalY * _local_17->normalY)) + (_local_17->normalZ * _local_17->normalZ));
						if (_local_18 > 0.001)
						{
							_local_18 = (1 / MyMath::sqrt(_local_18));
							_local_17->normalX = (_local_17->normalX * _local_18);
							_local_17->normalY = (_local_17->normalY * _local_18);
							_local_17->normalZ = (_local_17->normalZ * _local_18);
						}
					}
					_local_11->vertex = _local_17;
					_local_17->next = _local_8->vertexList;
					_local_8->vertexList = _local_17;
					_local_11 = _local_11->next;
				}
				_local_9 = _local_9->next;
			}
		}
		_local_3++;
	}
	_local_3 = 0;
	while (_local_3 < _local_13)
	{
		std::shared_ptr<Mesh> _local_8 = std::dynamic_pointer_cast<Mesh>(_arg_1[_local_3]);
		if (_local_8 != nullptr)
		{
			_local_8->invertMatrix();
			Vertex *_local_10 = _local_8->vertexList.get();
			while (_local_10 != nullptr)
			{
				double _local_4 = _local_10->x;
				double _local_5 = _local_10->y;
				double _local_6 = _local_10->z;
				_local_10->x = ((((_local_8->matrix4.ma * _local_4) + (_local_8->matrix4.mb * _local_5)) + (_local_8->matrix4.mc * _local_6)) + _local_8->matrix4.md);
				_local_10->y = ((((_local_8->matrix4.me * _local_4) + (_local_8->matrix4.mf * _local_5)) + (_local_8->matrix4.mg * _local_6)) + _local_8->matrix4.mh);
				_local_10->z = ((((_local_8->matrix4.mi * _local_4) + (_local_8->matrix4.mj * _local_5)) + (_local_8->matrix4.mk * _local_6)) + _local_8->matrix4.ml);
				_local_4 = _local_10->normalX;
				_local_5 = _local_10->normalY;
				_local_6 = _local_10->normalZ;
				_local_10->normalX = (((_local_8->matrix4.ma * _local_4) + (_local_8->matrix4.mb * _local_5)) + (_local_8->matrix4.mc * _local_6));
				_local_10->normalY = (((_local_8->matrix4.me * _local_4) + (_local_8->matrix4.mf * _local_5)) + (_local_8->matrix4.mg * _local_6));
				_local_10->normalZ = (((_local_8->matrix4.mi * _local_4) + (_local_8->matrix4.mj * _local_5)) + (_local_8->matrix4.mk * _local_6));
				_local_10 = _local_10->next.get();
			}
			Shared<Face> _local_9 = _local_8->faceList;
			while (_local_9 != nullptr)
			{
				double _local_4 = _local_9->normalX;
				double _local_5 = _local_9->normalY;
				double _local_6 = _local_9->normalZ;
				_local_9->normalX = (((_local_8->matrix4.ma * _local_4) + (_local_8->matrix4.mb * _local_5)) + (_local_8->matrix4.mc * _local_6));
				_local_9->normalY = (((_local_8->matrix4.me * _local_4) + (_local_8->matrix4.mf * _local_5)) + (_local_8->matrix4.mg * _local_6));
				_local_9->normalZ = (((_local_8->matrix4.mi * _local_4) + (_local_8->matrix4.mj * _local_5)) + (_local_8->matrix4.mk * _local_6));
				_local_9->offset = (((_local_9->wrapper->vertex->x * _local_9->normalX) + (_local_9->wrapper->vertex->y * _local_9->normalY)) + (_local_9->wrapper->vertex->z * _local_9->normalZ));
				_local_9 = _local_9->next;
			}
		}
		_local_3++;
	}
}


void Mesh::draw(Camera3D *camera)
{
	if (this->faceList == nullptr)
	{
		return;
	}

	if (this->clipping == 0)
	{
		if ((culling & 0x01))
		{
			return;
		}
		culling = 0;
	}


	this->prepareResources();

	bool camera_view_constrained = true;

	if ((useDepth = (((!(camera_view_constrained)) && ((((camera->softTransparency) &&
														  (camera->softTransparencyStrength > 0)) ||
														 ((camera->ssao) && (camera->ssaoStrength > 0))) ||
														((camera->deferredLighting) && (camera->deferredLightingStrength > 0)))) &&
					 (concatenatedAlpha >= depthMapAlphaThreshold))))
	{
		if (camera->depthObjects.length() <= camera->depthCount)
			camera->depthObjects.resize(camera->depthCount);

		camera->depthObjects[camera->depthCount] = this;
		camera->depthCount++;
	}


	Shared<Face> _local_2;

	if (1 || ((concatenatedAlpha >= 1) && (concatenatedBlendMode == "normal")))
	{
		this->addOpaque(camera);
		_local_2 = this->transparentList;
	}
	else
	{
		_local_2 = this->faceList;
	}

	transformConst[0] = this->matrix4.ma;
	transformConst[1] = this->matrix4.mb;
	transformConst[2] = this->matrix4.mc;
	transformConst[3] = this->matrix4.md;
	transformConst[4] = this->matrix4.me;
	transformConst[5] = this->matrix4.mf;
	transformConst[6] = this->matrix4.mg;
	transformConst[7] = this->matrix4.mh;
	transformConst[8] = this->matrix4.mi;
	transformConst[9] = this->matrix4.mj;
	transformConst[10] = this->matrix4.mk;
	transformConst[11] = this->matrix4.ml;

	//int _local_3 = ((_arg_1.debug) ? _arg_1.checkInDebug(this) : 0);

	//if ((_local_3 & Debug.BOUNDS))
	//{
	//	Debug.drawBounds(_arg_1, this, boundMinX, boundMinY, boundMinZ, boundMaxX, boundMaxY, boundMaxZ);
	//}

	if (_local_2 == nullptr)
	{
		return;
	}

	if (transformId > 0x1DCD6500)
	{
		transformId = 0;
		Vertex *_local_4 = this->vertexList.get();
		while (_local_4 != nullptr)
		{
			_local_4->transformId = 0;
			_local_4 = _local_4->next.get();
		}
	}

	transformId++;

	calculateInverseMatrix();

	_local_2 = this->prepareFaces(camera, _local_2);

	if (_local_2 == nullptr)
	{
		return;
	}

	if (culling > 0)
	{
		if (this->clipping == 1)
		{
			_local_2 = camera->cull(_local_2, culling);
		}
		else
		{
			_local_2 = camera->clip(_local_2, culling);
		}

		if (_local_2 == nullptr)
		{
			return;
		}
	}

	if (_local_2->processNext != nullptr)
	{
		if (this->sorting == 1)
		{
			_local_2 = camera->sortByAverageZ(_local_2);
		}
		else
		{
			if (this->sorting == 2)
			{
				_local_2 = camera->sortByDynamicBSP(_local_2, this->threshold);
			}
		}
	}

	//if ((_local_3 & Debug.EDGES))
	//{
	//	Debug.drawEdges(camera, _local_2, 0xFFFFFF);
	//}

	this->drawFaces(camera, _local_2);
}


void Mesh::prepareResources()
{
	Shared<Face> _local_10 = nullptr;

	if (this->vertexBuffer == nullptr)
	{
		QVector<float> _local_1;

		int _local_2 = 0;
		int _local_3 = 0;

		Weak<Vertex> _local_4 = this->vertexList;

		while (_local_4 != nullptr)
		{
			_local_1.append(_local_4->x);
			_local_2++;
			_local_1.append(_local_4->y);
			_local_2++;
			_local_1.append(_local_4->z);
			_local_2++;
			_local_1.append(_local_4->u);
			_local_2++;
			_local_1.append(_local_4->v);
			_local_2++;
			_local_1.append(_local_4->normalX);
			_local_2++;
			_local_1.append(_local_4->normalY);
			_local_2++;
			_local_1.append(_local_4->normalZ);
			_local_2++;
			_local_4->index = _local_3;
			_local_3++;
			_local_4 = _local_4->next;
		}

		if (_local_3 > 0)
		{
			this->vertexBuffer = new VertexBufferResource(_local_1, 8);
		}

		QHash<std::shared_ptr<Material>, Shared<Face>> _local_12;

		Shared<Face> _local_8 = this->faceList;
		while (_local_8 != nullptr)
		{
			Shared<Face> _local_9 = _local_8->next;
			_local_8->next = nullptr;
			if (((!(_local_8->material == nullptr)) && ((!(_local_8->material->transparent())) || (_local_8->material->alphaTestThreshold > 0))))
			{
				_local_8->next = _local_12[_local_8->material];
				_local_12[_local_8->material] = _local_8;
			}
			else
			{
				if (_local_10 != nullptr)
				{
					_local_10->next = _local_8;
				}
				else
				{
					this->transparentList = _local_8;
				}
				_local_10 = _local_8;
			}
			_local_8 = _local_9;
		}

		this->faceList = this->transparentList;
		QVector<uint> _local_13;
		int _local_14 = 0;


		QHashIterator<std::shared_ptr<Material>, Shared<Face>> iter(_local_12);
		while (iter.hasNext())
		{
			iter.next();

			Shared<Face> _local_16 = iter.value();

			if (this->opaqueMaterials.length() <= this->opaqueLength)
				this->opaqueMaterials.resize(this->opaqueLength + 10);


			if (this->opaqueBegins.length() <= this->opaqueLength)
				this->opaqueBegins.resize(this->opaqueLength + 10);

			this->opaqueMaterials[this->opaqueLength] = _local_16->material;
			this->opaqueBegins[this->opaqueLength] = (this->numTriangles * 3);

			Shared<Face> _local_8 = _local_16;
			while (_local_8 != nullptr)
			{
				Weak<Wrapper> _local_11 = _local_8->wrapper;
				int _local_5 = _local_11->vertex->index;
				_local_11 = _local_11->next;
				int _local_6 = _local_11->vertex->index;
				_local_11 = _local_11->next;
				while (_local_11 != nullptr)
				{
					int _local_7 = _local_11->vertex->index;
					_local_13.append(_local_5);
					_local_14++;
					_local_13.append(_local_6);
					_local_14++;
					_local_13.append(_local_7);
					_local_14++;
					_local_6 = _local_7;
					this->numTriangles++;
					_local_11 = _local_11->next;
				}
				if (_local_8->next == nullptr)
				{
					_local_10 = _local_8;
				}
				_local_8 = _local_8->next;
			}

			if (this->opaqueNums.length() <= this->opaqueLength)
				this->opaqueNums.resize(this->opaqueLength + 10);

			this->opaqueNums[this->opaqueLength] = (this->numTriangles - (this->opaqueBegins[this->opaqueLength] / 3));
			this->opaqueLength++;
			_local_10->next = this->faceList;
			this->faceList = _local_16;
		}

		this->numOpaqueTriangles = this->numTriangles;

		_local_8 = this->transparentList;

		while (_local_8 != nullptr)
		{
			Weak<Wrapper> _local_11 = _local_8->wrapper;
			int _local_5 = _local_11->vertex->index;
			_local_11 = _local_11->next;
			int _local_6 = _local_11->vertex->index;
			_local_11 = _local_11->next;
			while (_local_11 != nullptr)
			{
				int _local_7 = _local_11->vertex->index;
				_local_13.append(_local_5);
				_local_14++;
				_local_13.append(_local_6);
				_local_14++;
				_local_13.append(_local_7);
				_local_14++;
				_local_6 = _local_7;
				this->numTriangles++;
				_local_11 = _local_11->next;
			}
			_local_8 = _local_8->next;
		}
		if (_local_14 > 0)
		{
			this->indexBuffer = new IndexBufferResource(_local_13);
		}
	}
}


Shared<Face> Mesh::prepareFaces(Camera3D *camera, Shared<Face> face)
{
	(void)camera;

	Shared<Face> _local_3 = nullptr;
	Shared<Face> _local_4 = nullptr;
	Shared<Face> _local_5 = face;

	while (_local_5 != nullptr)
	{
		if ((((_local_5->normalX * this->im.md) + (_local_5->normalY * this->im.mh)) + (_local_5->normalZ * this->im.ml)) > _local_5->offset)
		{
			Weak<Wrapper> _local_6 = _local_5->wrapper;
			while (_local_6 != nullptr)
			{
				Vertex *_local_7 = _local_6->vertex.get();
				if (_local_7->transformId != transformId)
				{
					double _local_8 = _local_7->x;
					double _local_9 = _local_7->y;
					double _local_10 = _local_7->z;
					_local_7->cameraX = ((((this->matrix4.ma * _local_8) + (this->matrix4.mb * _local_9)) + (this->matrix4.mc * _local_10)) + this->matrix4.md);
					_local_7->cameraY = ((((this->matrix4.me * _local_8) + (this->matrix4.mf * _local_9)) + (this->matrix4.mg * _local_10)) + this->matrix4.mh);
					_local_7->cameraZ = ((((this->matrix4.mi * _local_8) + (this->matrix4.mj * _local_9)) + (this->matrix4.mk * _local_10)) + this->matrix4.ml);
					_local_7->transformId = transformId;
					_local_7->drawId = 0;
				}
				_local_6 = _local_6->next;
			}

			if (_local_3 != nullptr)
			{
				_local_4->processNext = _local_5;
			}
			else
			{
				_local_3 = _local_5;
			}
			_local_4 = _local_5;
		}
		_local_5 = _local_5->next;
	}

	if (_local_4 != nullptr)
	{
		_local_4->processNext = nullptr;
	}

	return _local_3;
}


void Mesh::drawFaces(Camera3D *_arg_1, Shared<Face> _arg_2)
{
	Shared<Face> _local_4 = nullptr;
	Shared<Face> _local_5 = _arg_2;

	while (_local_5 != nullptr)
	{
		Shared<Face> _local_3 = _local_5->processNext;
		if (((_local_3 == nullptr) || (!(_local_3->material == _arg_2->material))))
		{
			_local_5->processNext = nullptr;
			if (_arg_2->material != nullptr)
			{
				_arg_2->processNegative = _local_4;
				_local_4 = _arg_2;
			}
			else
			{
				while (_arg_2 != nullptr)
				{
					_local_5 = _arg_2->processNext;
					_arg_2->processNext = nullptr;
					_arg_2 = _local_5;
				}
			}
			_arg_2 = _local_3;
		}

		_local_5 = _local_3;
	}

	_arg_2 = _local_4;
	while (_arg_2 != nullptr)
	{
		Shared<Face> _local_3 = _arg_2->processNegative;
		_arg_2->processNegative = nullptr;
		_arg_1->addTransparent(_arg_2, this);
		_arg_2 = _local_3;
	}
}


void Mesh::addOpaque(Camera3D *camera)
{
	for (int i = 0; i < this->opaqueLength; i++)
	{
		camera->addOpaque(this->opaqueMaterials[i].get(),
						  this->vertexBuffer,
						  this->indexBuffer,
						  this->opaqueBegins[i],
						  this->opaqueNums[i],
						  this);
	}
}


void Mesh::addVerticesAndFaces(const QVector<double> &_arg_1,
							   const QVector<double> &_arg_2,
							   const QVector<int> _arg_3,
							   bool _arg_4,
							   std::shared_ptr<Material> _arg_5)
{
	Shared<Vertex> _local_11 = nullptr;

	Shared<Face> _local_13 = nullptr;
	Shared<Face> face = nullptr;

	Shared<Wrapper> _local_15 = nullptr;

	this->deleteResources();

	int _local_9 = int((_arg_1.length() / 3));
	if (_local_9 != (_arg_2.length() / 2))
	{
		throw ArgumentError("Vertices count and uvs count doesn't match.");
	}

	int _local_10 = _arg_3.length();
	if (((!(_arg_4)) && (_local_10 % 3)))
	{
		throw ArgumentError("Incorrect indices.");
	}

	int _local_6 = 0;
	int _local_8 = 0;
	while (_local_6 < _local_10)
	{
		if (_local_6 == _local_8)
		{
			int _local_17 = ((_arg_4) ? _arg_3[_local_6] : 3);
			if (_local_17 < 3)
			{
				throw ArgumentError((_local_17 + " vertices not enough."));
			}
			_local_8 = ((_arg_4) ? (_local_17 + ++_local_6) : (_local_6 + _local_17));
			if (_local_8 > _local_10)
			{
				throw ArgumentError("Incorrect indices.");
			}
		}
		int _local_16 = _arg_3[_local_6];
		if (((_local_16 < 0) || (_local_16 >= _local_9)))
		{
			throw RangeError("Index is out of bounds.");
		}
		_local_6++;
	}

	if (this->vertexList != nullptr)
	{
		_local_11 = this->vertexList;
		while (_local_11->next != nullptr)
		{
			_local_11 = _local_11->next;
		}
	}

	QVector<Shared<Vertex>> _local_12;
	_local_12.resize(_local_9);

	_local_6 = 0;

	int _local_7 = 0;
	_local_8 = 0;
	while (_local_6 < _local_9)
	{
		Shared<Vertex> vertex = Vertex::create();
		vertex->x = _arg_1[_local_7];
		_local_7++;
		vertex->y = _arg_1[_local_7];
		_local_7++;
		vertex->z = _arg_1[_local_7];
		_local_7++;
		vertex->u = _arg_2[_local_8];
		_local_8++;
		vertex->v = _arg_2[_local_8];
		_local_8++;
		_local_12[_local_6] = vertex;

		if (_local_11 != nullptr)
		{
			_local_11->next = vertex;
		}
		else
		{
			this->vertexList = vertex;
		}

		_local_11 = vertex;
		_local_6++;
	}

	if (this->faceList != nullptr)
	{
		_local_13 = this->faceList;
		while (_local_13->next != nullptr)
		{
			_local_13 = _local_13->next;
		}
	}

	_local_6 = 0;
	_local_8 = 0;
	while (_local_6 < _local_10)
	{
		if (_local_6 == _local_8)
		{

			if (_arg_4 == true)
			{
				int v1 = _local_6;
				_local_6++;
				int v2 = _local_6;

				_local_8 = _arg_3[v1] + v2;
			}
			else
			{
				_local_8 = _local_6 + 3;
			}

			_local_15 = nullptr;
			face = Face::create(24);
			face->material = _arg_5;
			if (_local_13 != nullptr)
			{
				_local_13->next = face;
			}
			else
			{
				this->faceList = face;
			}
			_local_13 = face;
		}

		Shared<Wrapper> wrapper = Wrapper::create();
		wrapper->vertex = _local_12[_arg_3[_local_6]];

		if (_local_15 != nullptr)
		{
			_local_15->next = wrapper;
		}
		else
		{
			face->wrapper = wrapper;
		}

		_local_15 = wrapper;
		_local_6++;
	}
}


Shared<VG> Mesh::getVG(Camera3D *camera)
{
	if (this->faceList == nullptr)
	{
		return nullptr;
	}

	if (this->clipping == 0)
	{
		if ((culling & 0x01))
		{
			return nullptr;
		}
		culling = 0;
	}

	this->prepareResources();

	//if ((useDepth = (((!(_arg_1.view.constrained)) && ((((_arg_1.softTransparency) && (_arg_1.softTransparencyStrength > 0)) || ((_arg_1.ssao) && (_arg_1.ssaoStrength > 0))) || ((_arg_1.deferredLighting) && (_arg_1.deferredLightingStrength > 0)))) && (concatenatedAlpha >= depthMapAlphaThreshold))))
	//{
	//	_arg_1.depthObjects[_arg_1.depthCount] = this;
	//	_arg_1.depthCount++;
	//}

	Shared<Face> _local_2;

	if (concatenatedAlpha >= 1 && concatenatedBlendMode == "normal")
	{
		this->addOpaque(camera);
		_local_2 = this->transparentList;
	}
	else
	{
		_local_2 = this->faceList;
	}

	transformConst[0] = matrix4.ma;
	transformConst[1] = matrix4.mb;
	transformConst[2] = matrix4.mc;
	transformConst[3] = matrix4.md;
	transformConst[4] = matrix4.me;
	transformConst[5] = matrix4.mf;
	transformConst[6] = matrix4.mg;
	transformConst[7] = matrix4.mh;
	transformConst[8] = matrix4.mi;
	transformConst[9] = matrix4.mj;
	transformConst[10] = matrix4.mk;
	transformConst[11] = matrix4.ml;


	/*
	int _local_3 = ((_arg_1.debug) ? _arg_1.checkInDebug(this) : 0);
	if ((_local_3 & Debug.BOUNDS))
	{
		Debug.drawBounds(_arg_1, this, boundMinX, boundMinY, boundMinZ, boundMaxX, boundMaxY, boundMaxZ);
	}
	*/

	if (_local_2 == nullptr)
	{
		return nullptr;
	}

	if (transformId > 0x1DCD6500)
	{
		transformId = 0;
		Vertex *_local_4 = this->vertexList.get();
		while (_local_4 != nullptr)
		{
			_local_4->transformId = 0;
			_local_4 = _local_4->next.get();
		}
	}

	transformId++;
	calculateInverseMatrix();
	_local_2 = this->prepareFaces(camera, _local_2);

	if (_local_2 == nullptr)
	{
		return nullptr;
	}

	if (culling > 0)
	{
		if (this->clipping == 1)
		{
			_local_2 = camera->cull(_local_2, culling);
		}
		else
		{
			_local_2 = camera->clip(_local_2, culling);
		}

		if (_local_2 == nullptr)
		{
			return nullptr;
		}
	}

	bool debug = false;

	return VG::create(this->shared_from_this(), _local_2, this->sorting, debug, false, 2);
}
