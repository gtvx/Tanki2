#include "VG.h"
#include "Object3D.h"
#include "Face.h"
#include "Wrapper.h"
#include "Vertex.h"
#include "Camera3D.h"
#include "MyMath.h"
#include "../materials/Material.h"
//#include <QHash>

namespace
{
	class Collector
	{
	public:
		Shared<VG> collector;

		Collector()
		{

		}

		~Collector()
		{
			Shared<VG> temp = collector;

			while (temp != nullptr)
			{
				Shared<VG> next = temp->next;
				delete temp.get();
				temp.reset();
				temp = next;
			}

			collector.reset();
		}


		int count()
		{
			int count = 0;

			Weak<VG> temp = collector;

			while (temp != nullptr)
			{
				count++;
				temp = temp->next;
			}

			return count;
		}

	} c;

	//QHash<int, Collector*> _hash;
}


Shared<VG> VG::create(int id)
{
	Shared<VG> result;

#if 0
	Collector *collector = _hash[id];
	if (collector == nullptr)
	{
		collector = new Collector;
		_hash[id] = collector;
	}
#endif

	Collector *collector = &c;


	if (collector->collector != nullptr)
	{
		result = collector->collector;
		collector->collector = result->next;
		result->next = nullptr;
	}
	else
	{
		result.reset(new VG);
	}

	result->viewAligned = false;
	result->sorting = 0;
	result->numOccluders = 0;
	result->faceStruct = nullptr;
	result->object = nullptr;
	result->space = 0;
	result->boundVertexList = nullptr;
	result->boundPlaneList = nullptr;
	result->id = id;

	return result;
}


Shared<VG> VG::create(std::shared_ptr<Object3D> object, Shared<Face> faceStruct, int sorting, int debug, bool viewAligned, int id)
{
	(void)debug;

	Shared<VG> result;

#if 0
	Collector *collector = _hash[id];
	if (collector == nullptr)
	{
		collector = new Collector;
		_hash[id] = collector;
	}
#endif

	Collector *collector = &c;

	if (collector->collector != nullptr)
	{
		result = collector->collector;
		collector->collector = result->next;
		result->next = nullptr;
	}
	else
	{
		result.reset(new VG);
	}

	result->viewAligned = viewAligned;
	result->sorting = sorting;
	result->numOccluders = 0;
	result->faceStruct = faceStruct;
	result->object = object;
	result->space = 0;
	result->boundVertexList = nullptr;
	result->boundPlaneList = nullptr;
	result->id = id;

	return result;
}


void VG::remove()
{
	auto temp = this;

	while (temp->next != nullptr)
	{
		if (temp->next->count() > 1)
		{
			break;
		}

		temp = temp->next.get();
	}


	this->numOccluders = 0;
	this->faceStruct = nullptr;
	this->object = nullptr;
	this->boundVertexList = nullptr;
	this->boundPlaneList = nullptr;

	Collector *collector = &c;
	//Collector *collector = _hash[id];

	temp->next = collector->collector;
	collector->collector.set(this);
}


VG::VG()
{
	space = 0;
	next = nullptr;
	faceStruct = nullptr;
	viewAligned = false;
	sorting = 0;

	boundVertexList = Vertex::createList(8);
	boundPlaneList = Vertex::createList(6);

	numOccluders = 0;
}

static Shared<Face> collectNode(Shared<Face> _arg_1, Shared<Face> _arg_2 = nullptr)
{

	Shared<Face> _local_4;
	Shared<Face> _local_5;

	if (_arg_1->offset < 0)
	{
		_local_4 = _arg_1->processNegative;
		_local_5 = _arg_1->processPositive;
	}
	else
	{
		_local_4 = _arg_1->processPositive;
		_local_5 = _arg_1->processNegative;
	}

	_arg_1->processNegative = nullptr;
	_arg_1->processPositive = nullptr;

	if (_local_5 != nullptr)
	{
		_arg_2 = collectNode(_local_5, _arg_2);
	}

	if (_arg_1->wrapper != nullptr)
	{
		Weak<Face> _local_3 = _arg_1;
		while (_local_3->processNext != nullptr)
		{
			_local_3 = _local_3->processNext;
		}
		_local_3->processNext = _arg_2;
		_arg_2 = _arg_1;
	}

	if (_local_4 != nullptr)
	{
		_arg_2 = collectNode(_local_4, _arg_2);
	}

	return _arg_2;
}


void VG::calculateOOBBStruct(Shared<Face> _arg_1,
							 int _arg_2,
							 double _arg_3,
							 double _arg_4,
							 double _arg_5,
							 double _arg_6,
							 double _arg_7,
							 double _arg_8,
							 double _arg_9,
							 double _arg_10,
							 double _arg_11,
							 double _arg_12,
							 double _arg_13,
							 double _arg_14)
{
	Shared<Face> _local_15 = _arg_1;
	while (_local_15 != nullptr)
	{
		Weak<Wrapper> _local_16 = _local_15->wrapper;
		while (_local_16 != nullptr)
		{
			Vertex *_local_17 = _local_16->vertex.get();
			if (_local_17->transformId != _arg_2)
			{
				if (_local_17->x < this->boundMinX)
				{
					this->boundMinX = _local_17->x;
				}
				if (_local_17->x > this->boundMaxX)
				{
					this->boundMaxX = _local_17->x;
				}
				if (_local_17->y < this->boundMinY)
				{
					this->boundMinY = _local_17->y;
				}
				if (_local_17->y > this->boundMaxY)
				{
					this->boundMaxY = _local_17->y;
				}
				if (_local_17->z < this->boundMinZ)
				{
					this->boundMinZ = _local_17->z;
				}
				if (_local_17->z > this->boundMaxZ)
				{
					this->boundMaxZ = _local_17->z;
				}
				_local_17->transformId = _arg_2;
			}
			_local_16 = _local_16->next;
		}
		_local_15 = _local_15->processNext;
	}
	if (_arg_1->processNegative != nullptr)
	{
		this->calculateOOBBStruct(_arg_1->processNegative, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9, _arg_10, _arg_11, _arg_12, _arg_13, _arg_14);
	}
	if (_arg_1->processPositive != nullptr)
	{
		this->calculateOOBBStruct(_arg_1->processPositive, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9, _arg_10, _arg_11, _arg_12, _arg_13, _arg_14);
	}
}


void VG::calculateAABB(double _arg_1,
				  double _arg_2,
				  double _arg_3,
				  double _arg_4,
				  double _arg_5,
				  double _arg_6,
				  double _arg_7,
				  double _arg_8,
				  double _arg_9,
				  double _arg_10,
				  double _arg_11,
				  double _arg_12)
{
	this->boundMinX = 1E22;
	this->boundMinY = 1E22;
	this->boundMinZ = 1E22;
	this->boundMaxX = -1E22;
	this->boundMaxY = -1E22;
	this->boundMaxZ = -1E22;

	this->calculateAABBStruct(this->faceStruct,
							 ++this->object->transformId,
							 _arg_1,
							 _arg_2,
							 _arg_3,
							 _arg_4,
							 _arg_5,
							 _arg_6,
							 _arg_7,
							 _arg_8,
							 _arg_9,
							 _arg_10,
							 _arg_11,
							 _arg_12);
	this->space = 1;
}


void VG::calculateAABBcalculateAABBStruct(Shared<Face> _arg_1,
										  int _arg_2,
										  double _arg_3,
										  double _arg_4,
										  double _arg_5,
										  double _arg_6,
										  double _arg_7,
										  double _arg_8,
										  double _arg_9,
										  double _arg_10,
										  double _arg_11,
										  double _arg_12,
										  double _arg_13,
										  double _arg_14)
{
	Shared<Face> _local_15 = _arg_1;

	while (_local_15 != nullptr)
	{
		Weak<Wrapper> _local_16 = _local_15->wrapper;
		while (_local_16 != nullptr)
		{
			Vertex *_local_17 = _local_16->vertex.get();
			if (_local_17->transformId != _arg_2)
			{
				double _local_18 = _local_17->cameraX;
				double _local_19 = _local_17->cameraY;
				double _local_20 = _local_17->cameraZ;
				_local_17->cameraX = ((((_arg_3 * _local_18) + (_arg_4 * _local_19)) + (_arg_5 * _local_20)) + _arg_6);
				_local_17->cameraY = ((((_arg_7 * _local_18) + (_arg_8 * _local_19)) + (_arg_9 * _local_20)) + _arg_10);
				_local_17->cameraZ = ((((_arg_11 * _local_18) + (_arg_12 * _local_19)) + (_arg_13 * _local_20)) + _arg_14);

				if (_local_17->cameraX < this->boundMinX)
				{
					this->boundMinX = _local_17->cameraX;
				}
				if (_local_17->cameraX > this->boundMaxX)
				{
					this->boundMaxX = _local_17->cameraX;
				}
				if (_local_17->cameraY < this->boundMinY)
				{
					this->boundMinY = _local_17->cameraY;
				}
				if (_local_17->cameraY > this->boundMaxY)
				{
					this->boundMaxY = _local_17->cameraY;
				}
				if (_local_17->cameraZ < this->boundMinZ)
				{
					this->boundMinZ = _local_17->cameraZ;
				}
				if (_local_17->cameraZ > this->boundMaxZ)
				{
					this->boundMaxZ = _local_17->cameraZ;
				}
				_local_17->transformId = _arg_2;
			}
			_local_16 = _local_16->next;
		}
		_local_15 = _local_15->processNext;
	}

	if (_arg_1->processNegative != nullptr)
	{
		this->calculateAABBStruct(_arg_1->processNegative, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9, _arg_10, _arg_11, _arg_12, _arg_13, _arg_14);
	}

	if (_arg_1->processPositive != nullptr)
	{
		this->calculateAABBStruct(_arg_1->processPositive, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9, _arg_10, _arg_11, _arg_12, _arg_13, _arg_14);
	}
}


void VG::calculateAABBStruct(Shared<Face> _arg_1,
							 int _arg_2,
							 double _arg_3,
							 double _arg_4,
							 double _arg_5,
							 double _arg_6,
							 double _arg_7,
							 double _arg_8,
							 double _arg_9,
							 double _arg_10,
							 double _arg_11,
							 double _arg_12,
							 double _arg_13,
							 double _arg_14)
{
	Weak<Face> _local_15 = _arg_1;

	while (_local_15 != nullptr)
	{
		Weak<Wrapper> _local_16 = _local_15->wrapper;
		while (_local_16 != nullptr)
		{
			Vertex *_local_17 = _local_16->vertex.get();
			if (_local_17->transformId != _arg_2)
			{
				double _local_18 = _local_17->cameraX;
				double _local_19 = _local_17->cameraY;
				double _local_20 = _local_17->cameraZ;
				_local_17->cameraX = ((((_arg_3 * _local_18) + (_arg_4 * _local_19)) + (_arg_5 * _local_20)) + _arg_6);
				_local_17->cameraY = ((((_arg_7 * _local_18) + (_arg_8 * _local_19)) + (_arg_9 * _local_20)) + _arg_10);
				_local_17->cameraZ = ((((_arg_11 * _local_18) + (_arg_12 * _local_19)) + (_arg_13 * _local_20)) + _arg_14);
				if (_local_17->cameraX < this->boundMinX)
				{
					this->boundMinX = _local_17->cameraX;
				}
				if (_local_17->cameraX > this->boundMaxX)
				{
					this->boundMaxX = _local_17->cameraX;
				}
				if (_local_17->cameraY < this->boundMinY)
				{
					this->boundMinY = _local_17->cameraY;
				}
				if (_local_17->cameraY > this->boundMaxY)
				{
					this->boundMaxY = _local_17->cameraY;
				}
				if (_local_17->cameraZ < this->boundMinZ)
				{
					this->boundMinZ = _local_17->cameraZ;
				}
				if (_local_17->cameraZ > this->boundMaxZ)
				{
					this->boundMaxZ = _local_17->cameraZ;
				}
				_local_17->transformId = _arg_2;
			}
			_local_16 = _local_16->next;
		}
		_local_15 = _local_15->processNext;
	}

	if (_arg_1->processNegative != nullptr)
	{
		this->calculateAABBStruct(_arg_1->processNegative, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9, _arg_10, _arg_11, _arg_12, _arg_13, _arg_14);
	}

	if (_arg_1->processPositive != nullptr)
	{
		this->calculateAABBStruct(_arg_1->processPositive, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9, _arg_10, _arg_11, _arg_12, _arg_13, _arg_14);
	}
}


namespace
{
	struct LocalVertex
	{
		double x, y, z;
		double cameraX, cameraY, cameraZ;
		double offset;
	};
}


void VG::calculateOOBB(Object3D *_arg_1)
{
	if (this->space == 1)
	{
		this->transformStruct(this->faceStruct,
							  ++this->object->transformId,
							  _arg_1->matrix4.ma,
							  _arg_1->matrix4.mb,
							  _arg_1->matrix4.mc,
							  _arg_1->matrix4.md,
							  _arg_1->matrix4.me,
							  _arg_1->matrix4.mf,
							  _arg_1->matrix4.mg,
							  _arg_1->matrix4.mh,
							  _arg_1->matrix4.mi,
							  _arg_1->matrix4.mj,
							  _arg_1->matrix4.mk,
							  _arg_1->matrix4.ml);
	}

	if (!this->viewAligned)
	{
		this->boundMinX = 1E22;
		this->boundMinY = 1E22;
		this->boundMinZ = 1E22;
		this->boundMaxX = -1E22;
		this->boundMaxY = -1E22;
		this->boundMaxZ = -1E22;
		this->calculateOOBBStruct(this->faceStruct,
								  ++this->object->transformId,
								  this->object->im.ma,
								  this->object->im.mb,
								  this->object->im.mc,
								  this->object->im.md,
								  this->object->im.me,
								  this->object->im.mf,
								  this->object->im.mg,
								  this->object->im.mh,
								  this->object->im.mi,
								  this->object->im.mj,
								  this->object->im.mk,
								  this->object->im.ml);

		if ((this->boundMaxX - this->boundMinX) < 1)
		{
			this->boundMaxX = (this->boundMinX + 1);
		}

		if ((this->boundMaxY - this->boundMinY) < 1)
		{
			this->boundMaxY = (this->boundMinY + 1);
		}

		if ((this->boundMaxZ - this->boundMinZ) < 1)
		{
			this->boundMaxZ = (this->boundMinZ + 1);
		}


		LocalVertex boundVertexList[8];


		LocalVertex *_local_2 = &boundVertexList[0];
		_local_2->x = this->boundMinX;
		_local_2->y = this->boundMinY;
		_local_2->z = this->boundMinZ;
		LocalVertex *_local_3 = &boundVertexList[1];
		_local_3->x = this->boundMaxX;
		_local_3->y = this->boundMinY;
		_local_3->z = this->boundMinZ;
		LocalVertex *_local_4 = &boundVertexList[2];
		_local_4->x = this->boundMinX;
		_local_4->y = this->boundMaxY;
		_local_4->z = this->boundMinZ;
		LocalVertex *_local_5 = &boundVertexList[3];
		_local_5->x = this->boundMaxX;
		_local_5->y = this->boundMaxY;
		_local_5->z = this->boundMinZ;
		LocalVertex *_local_6 = &boundVertexList[4];
		_local_6->x = this->boundMinX;
		_local_6->y = this->boundMinY;
		_local_6->z = this->boundMaxZ;
		LocalVertex *_local_7 = &boundVertexList[5];
		_local_7->x = this->boundMaxX;
		_local_7->y = this->boundMinY;
		_local_7->z = this->boundMaxZ;
		LocalVertex *_local_8 = &boundVertexList[6];
		_local_8->x = this->boundMinX;
		_local_8->y = this->boundMaxY;
		_local_8->z = this->boundMaxZ;
		LocalVertex *_local_9 = &boundVertexList[7];
		_local_9->x = this->boundMaxX;
		_local_9->y = this->boundMaxY;
		_local_9->z = this->boundMaxZ;




		for (int i = 0; i < 8; i++)
		{
			LocalVertex *_local_10 = &boundVertexList[i];
			_local_10->cameraX = ((((this->object->matrix4.ma * _local_10->x) + (this->object->matrix4.mb * _local_10->y)) + (this->object->matrix4.mc * _local_10->z)) + this->object->matrix4.md);
			_local_10->cameraY = ((((this->object->matrix4.me * _local_10->x) + (this->object->matrix4.mf * _local_10->y)) + (this->object->matrix4.mg * _local_10->z)) + this->object->matrix4.mh);
			_local_10->cameraZ = ((((this->object->matrix4.mi * _local_10->x) + (this->object->matrix4.mj * _local_10->y)) + (this->object->matrix4.mk * _local_10->z)) + this->object->matrix4.ml);
			//_local_10 = _local_10->next;
		}

		LocalVertex boundPlaneList[6];

		LocalVertex *_local_11 = &boundPlaneList[0];

		LocalVertex *_local_12 = &boundPlaneList[1];


		double _local_13 = _local_2->cameraX;
		double _local_14 = _local_2->cameraY;
		double _local_15 = _local_2->cameraZ;
		double _local_16 = (_local_3->cameraX - _local_13);
		double _local_17 = (_local_3->cameraY - _local_14);
		double _local_18 = (_local_3->cameraZ - _local_15);
		double _local_19 = (_local_6->cameraX - _local_13);
		double _local_20 = (_local_6->cameraY - _local_14);
		double _local_21 = (_local_6->cameraZ - _local_15);
		double _local_22 = ((_local_21 * _local_17) - (_local_20 * _local_18));
		double _local_23 = ((_local_19 * _local_18) - (_local_21 * _local_16));
		double _local_24 = ((_local_20 * _local_16) - (_local_19 * _local_17));
		double _local_25 = (1 / MyMath::sqrt((((_local_22 * _local_22) + (_local_23 * _local_23)) + (_local_24 * _local_24))));
		_local_22 = (_local_22 * _local_25);
		_local_23 = (_local_23 * _local_25);
		_local_24 = (_local_24 * _local_25);
		_local_11->cameraX = _local_22;
		_local_11->cameraY = _local_23;
		_local_11->cameraZ = _local_24;
		_local_11->offset = (((_local_13 * _local_22) + (_local_14 * _local_23)) + (_local_15 * _local_24));
		_local_12->cameraX = -(_local_22);
		_local_12->cameraY = -(_local_23);
		_local_12->cameraZ = -(_local_24);
		_local_12->offset = (((-(_local_4->cameraX) * _local_22) - (_local_4->cameraY * _local_23)) - (_local_4->cameraZ * _local_24));

		LocalVertex *_local_26 = &boundPlaneList[2];

		LocalVertex *_local_27 = &boundPlaneList[3];


		_local_13 = _local_2->cameraX;
		_local_14 = _local_2->cameraY;
		_local_15 = _local_2->cameraZ;
		_local_16 = (_local_6->cameraX - _local_13);
		_local_17 = (_local_6->cameraY - _local_14);
		_local_18 = (_local_6->cameraZ - _local_15);
		_local_19 = (_local_4->cameraX - _local_13);
		_local_20 = (_local_4->cameraY - _local_14);
		_local_21 = (_local_4->cameraZ - _local_15);
		_local_22 = ((_local_21 * _local_17) - (_local_20 * _local_18));
		_local_23 = ((_local_19 * _local_18) - (_local_21 * _local_16));
		_local_24 = ((_local_20 * _local_16) - (_local_19 * _local_17));
		_local_25 = (1 / MyMath::sqrt((((_local_22 * _local_22) + (_local_23 * _local_23)) + (_local_24 * _local_24))));
		_local_22 = (_local_22 * _local_25);
		_local_23 = (_local_23 * _local_25);
		_local_24 = (_local_24 * _local_25);
		_local_26->cameraX = _local_22;
		_local_26->cameraY = _local_23;
		_local_26->cameraZ = _local_24;
		_local_26->offset = (((_local_13 * _local_22) + (_local_14 * _local_23)) + (_local_15 * _local_24));
		_local_27->cameraX = -(_local_22);
		_local_27->cameraY = -(_local_23);
		_local_27->cameraZ = -(_local_24);
		_local_27->offset = (((-(_local_3->cameraX) * _local_22) - (_local_3->cameraY * _local_23)) - (_local_3->cameraZ * _local_24));

		LocalVertex *_local_28 = &boundPlaneList[4];
		LocalVertex *_local_29 = &boundPlaneList[5];;

		_local_13 = _local_6->cameraX;
		_local_14 = _local_6->cameraY;
		_local_15 = _local_6->cameraZ;
		_local_16 = (_local_7->cameraX - _local_13);
		_local_17 = (_local_7->cameraY - _local_14);
		_local_18 = (_local_7->cameraZ - _local_15);
		_local_19 = (_local_8->cameraX - _local_13);
		_local_20 = (_local_8->cameraY - _local_14);
		_local_21 = (_local_8->cameraZ - _local_15);
		_local_22 = ((_local_21 * _local_17) - (_local_20 * _local_18));
		_local_23 = ((_local_19 * _local_18) - (_local_21 * _local_16));
		_local_24 = ((_local_20 * _local_16) - (_local_19 * _local_17));
		_local_25 = (1 / MyMath::sqrt((((_local_22 * _local_22) + (_local_23 * _local_23)) + (_local_24 * _local_24))));
		_local_22 = (_local_22 * _local_25);
		_local_23 = (_local_23 * _local_25);
		_local_24 = (_local_24 * _local_25);
		_local_28->cameraX = _local_22;
		_local_28->cameraY = _local_23;
		_local_28->cameraZ = _local_24;
		_local_28->offset = (((_local_13 * _local_22) + (_local_14 * _local_23)) + (_local_15 * _local_24));
		_local_29->cameraX = -(_local_22);
		_local_29->cameraY = -(_local_23);
		_local_29->cameraZ = -(_local_24);
		_local_29->offset = (((-(_local_2->cameraX) * _local_22) - (_local_2->cameraY * _local_23)) - (_local_2->cameraZ * _local_24));

		if (_local_11->offset < -(_local_12->offset))
		{
			_local_12->cameraX = -(_local_12->cameraX);
			_local_12->cameraY = -(_local_12->cameraY);
			_local_12->cameraZ = -(_local_12->cameraZ);
			_local_12->offset = -(_local_12->offset);
			_local_11->cameraX = -(_local_11->cameraX);
			_local_11->cameraY = -(_local_11->cameraY);
			_local_11->cameraZ = -(_local_11->cameraZ);
			_local_11->offset = -(_local_11->offset);
		}

		if (_local_26->offset < -(_local_27->offset))
		{
			_local_26->cameraX = -(_local_26->cameraX);
			_local_26->cameraY = -(_local_26->cameraY);
			_local_26->cameraZ = -(_local_26->cameraZ);
			_local_26->offset = -(_local_26->offset);
			_local_27->cameraX = -(_local_27->cameraX);
			_local_27->cameraY = -(_local_27->cameraY);
			_local_27->cameraZ = -(_local_27->cameraZ);
			_local_27->offset = -(_local_27->offset);
		}

		if (_local_29->offset < -(_local_28->offset))
		{
			_local_29->cameraX = -(_local_29->cameraX);
			_local_29->cameraY = -(_local_29->cameraY);
			_local_29->cameraZ = -(_local_29->cameraZ);
			_local_29->offset = -(_local_29->offset);
			_local_28->cameraX = -(_local_28->cameraX);
			_local_28->cameraY = -(_local_28->cameraY);
			_local_28->cameraZ = -(_local_28->cameraZ);
			_local_28->offset = -(_local_28->offset);
		}
	}

	this->space = 2;
}


void VG::draw(Camera3D *_arg_1, double _arg_2, Object3D *_arg_3)
{
	Shared<Face> _local_6 = nullptr;
	Shared<Face> _local_4 = nullptr;

	if (this->space == 1)
	{
		this->transformStruct(this->faceStruct,
							  ++this->object->transformId,
							  _arg_3->matrix4.ma,
							  _arg_3->matrix4.mb,
							  _arg_3->matrix4.mc,
							  _arg_3->matrix4.md,
							  _arg_3->matrix4.me,
							  _arg_3->matrix4.mf,
							  _arg_3->matrix4.mg,
							  _arg_3->matrix4.mh,
							  _arg_3->matrix4.mi,
							  _arg_3->matrix4.mj,
							  _arg_3->matrix4.mk,
							  _arg_3->matrix4.ml);
	}

	if (this->viewAligned)
	{
		_local_4 = this->faceStruct;
/*
		if (this->debug > 0)
		{
			if ((this->debug & Debug.EDGES))
			{
				Debug.drawEdges(_arg_1, _local_4, ((this->space != 2) ? 0xFFFFFF : 0xFF9900));
			}
			if ((this->debug & Debug.BOUNDS))
			{
				if (this->space == 1)
				{
					Debug.drawBounds(_arg_1, _arg_3, this->boundMinX, this->boundMinY, this->boundMinZ, this->boundMaxX, this->boundMaxY, this->boundMaxZ, 0x99FF00);
				}
			}
		}
*/
		_arg_1->addTransparent(_local_4, this->object.get());
	}
	else
	{
		switch (this->sorting)
		{
			case 0:
				_local_4 = this->faceStruct;
				break;
			case 1:
				_local_4 = ((this->faceStruct->processNext != nullptr) ? _arg_1->sortByAverageZ(this->faceStruct) : this->faceStruct);
				break;
			case 2:
				_local_4 = ((this->faceStruct->processNext != nullptr) ? _arg_1->sortByDynamicBSP(this->faceStruct, _arg_2) : this->faceStruct);
				break;
			case 3:
				_local_4 = collectNode(this->faceStruct);
				break;
		}
/*
		if (this->debug > 0)
		{
			if ((this->debug & Debug.EDGES))
			{
				Debug.drawEdges(_arg_1, _local_4, 0xFFFFFF);
			}
			if ((this->debug & Debug.BOUNDS))
			{
				if (this->space == 1)
				{
					Debug.drawBounds(_arg_1, _arg_3, this->boundMinX, this->boundMinY, this->boundMinZ, this->boundMaxX, this->boundMaxY, this->boundMaxZ, 0x99FF00);
				}
				else
				{
					if (this->space == 2)
					{
						Debug.drawBounds(_arg_1, this->object, this->boundMinX, this->boundMinY, this->boundMinZ, this->boundMaxX, this->boundMaxY, this->boundMaxZ, 0xFF9900);
					}
				}
			}
		}
*/

		Shared<Face> _local_7 = _local_4;
		while (_local_7 != nullptr)
		{
			Shared<Face> _local_5 = _local_7->processNext;
			if (((_local_5 == nullptr) || (!(_local_5->material == _local_4->material))))
			{
				_local_7->processNext = nullptr;
				if (_local_4->material != nullptr)
				{
					_local_4->processNegative = _local_6;
					_local_6 = _local_4;
				}
				else
				{
					while (_local_4 != nullptr)
					{
						_local_7 = _local_4->processNext;
						_local_4->processNext = nullptr;
						_local_4 = _local_7;
					}
				}
				_local_4 = _local_5;
			}
			_local_7 = _local_5;
		}

		_local_4 = _local_6;
		while (_local_4 != nullptr)
		{
			Shared<Face> _local_5 = _local_4->processNegative;
			_local_4->processNegative = nullptr;
			_arg_1->addTransparent(_local_4, this->object.get());
			_local_4 = _local_5;
		}
	}

	this->faceStruct = nullptr;
}


void VG::transformStruct(Shared<Face> _arg_1,
						 int _arg_2,
						 double _arg_3,
						 double _arg_4,
						 double _arg_5,
						 double _arg_6,
						 double _arg_7,
						 double _arg_8,
						 double _arg_9,
						 double _arg_10,
						 double _arg_11,
						 double _arg_12,
						 double _arg_13,
						 double _arg_14)
{
	Shared<Face> _local_15 = _arg_1;
	while (_local_15 != nullptr)
	{
		Weak<Wrapper> _local_16 = _local_15->wrapper;
		while (_local_16 != nullptr)
		{
			Vertex *_local_17 = _local_16->vertex.get();
			if (_local_17->transformId != _arg_2)
			{
				double _local_18 = _local_17->cameraX;
				double _local_19 = _local_17->cameraY;
				double _local_20 = _local_17->cameraZ;
				_local_17->cameraX = ((((_arg_3 * _local_18) + (_arg_4 * _local_19)) + (_arg_5 * _local_20)) + _arg_6);
				_local_17->cameraY = ((((_arg_7 * _local_18) + (_arg_8 * _local_19)) + (_arg_9 * _local_20)) + _arg_10);
				_local_17->cameraZ = ((((_arg_11 * _local_18) + (_arg_12 * _local_19)) + (_arg_13 * _local_20)) + _arg_14);
				_local_17->transformId = _arg_2;
			}
			_local_16 = _local_16->next;
		}
		_local_15 = _local_15->processNext;
	}
	if (_arg_1->processNegative != nullptr)
	{
		this->transformStruct(_arg_1->processNegative, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9, _arg_10, _arg_11, _arg_12, _arg_13, _arg_14);
	}
	if (_arg_1->processPositive != nullptr)
	{
		this->transformStruct(_arg_1->processPositive, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9, _arg_10, _arg_11, _arg_12, _arg_13, _arg_14);
	}
}


void VG::destroy()
{
	next = nullptr;
	boundPlaneList = nullptr;
	boundVertexList = nullptr;
	object = nullptr;
	faceStruct = nullptr;
}


void VG::split(Camera3D *_arg_1, double _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6)
{
	Shared<Face> _local_7 = this->faceStruct->create(8);


	this->splitFaceStruct(_arg_1, this->faceStruct, _local_7, _arg_2, _arg_3, _arg_4, _arg_5, (_arg_5 - _arg_6), (_arg_5 + _arg_6));

	if (_local_7->processNegative != nullptr)
	{
		Shared<VG> _local_8 = VG::create(1);

		/*
		if (collector != nullptr)
		{
			_local_8 = c.collector;
			collector = c.collector->next;
			_local_8->next = nullptr;
		}
		else
		{
			_local_8 = new VG();
		}
		*/

		this->next = _local_8;
		_local_8->faceStruct = _local_7->processNegative;
		_local_7->processNegative = nullptr;
		_local_8->object = this->object;
		_local_8->sorting = this->sorting;
		//_local_8->debug = this->debug;
		_local_8->space = this->space;
		_local_8->viewAligned = this->viewAligned;
		_local_8->boundMinX = 1E22;
		_local_8->boundMinY = 1E22;
		_local_8->boundMinZ = 1E22;
		_local_8->boundMaxX = -1E22;
		_local_8->boundMaxY = -1E22;
		_local_8->boundMaxZ = -1E22;
		_local_8->updateAABBStruct(_local_8->faceStruct, ++this->object->transformId);
	}
	else
	{
		this->next = nullptr;
	}
	if (_local_7->processPositive != nullptr)
	{
		this->faceStruct = _local_7->processPositive;
		_local_7->processPositive = nullptr;
		this->boundMinX = 1E22;
		this->boundMinY = 1E22;
		this->boundMinZ = 1E22;
		this->boundMaxX = -1E22;
		this->boundMaxY = -1E22;
		this->boundMaxZ = -1E22;
		this->updateAABBStruct(this->faceStruct, ++this->object->transformId);
	}
	else
	{
		this->faceStruct = nullptr;
	}
	//_local_7->next = Face::collector;
	//Face::collector = _local_7;
}


void VG::crop(Camera3D *_arg_1, double _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6)
{
	this->faceStruct = this->cropFaceStruct(_arg_1, this->faceStruct, _arg_2, _arg_3, _arg_4, _arg_5, (_arg_5 - _arg_6), (_arg_5 + _arg_6));
	if (this->faceStruct != nullptr)
	{
		this->boundMinX = 1E22;
		this->boundMinY = 1E22;
		this->boundMinZ = 1E22;
		this->boundMaxX = -1E22;
		this->boundMaxY = -1E22;
		this->boundMaxZ = -1E22;
		this->updateAABBStruct(this->faceStruct, ++this->object->transformId);
	}
}


Shared<Face> VG::cropFaceStruct(Camera3D *_arg_1, Shared<Face> _arg_2, double _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7, double _arg_8)
{
	Shared<Face> _local_13 = nullptr;
	Shared<Face> _local_14 = nullptr;
	Shared<Face> _local_15 = nullptr;
	Shared<Face> _local_16 = nullptr;

	if (_arg_2->processNegative != nullptr)
	{
		_local_13 = this->cropFaceStruct(_arg_1, _arg_2->processNegative, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8);
		_arg_2->processNegative = nullptr;
	}
	if (_arg_2->processPositive != nullptr)
	{
		_local_14 = this->cropFaceStruct(_arg_1, _arg_2->processPositive, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8);
		_arg_2->processPositive = nullptr;
	}
	if (_arg_2->wrapper != nullptr)
	{
		Shared<Face> _local_9 = _arg_2;
		while (_local_9 != nullptr)
		{
			Shared<Face> _local_10 = _local_9->processNext;
			Shared<Wrapper> _local_11 = _local_9->wrapper;
			Shared<Vertex> _local_17 = _local_11->vertex;
			_local_11 = _local_11->next;
			Shared<Vertex> _local_18 = _local_11->vertex;
			_local_11 = _local_11->next;
			Shared<Vertex> _local_19 = _local_11->vertex;
			_local_11 = _local_11->next;
			double _local_20 = (((_local_17->cameraX * _arg_3) + (_local_17->cameraY * _arg_4)) + (_local_17->cameraZ * _arg_5));
			double _local_21 = (((_local_18->cameraX * _arg_3) + (_local_18->cameraY * _arg_4)) + (_local_18->cameraZ * _arg_5));
			double _local_22 = (((_local_19->cameraX * _arg_3) + (_local_19->cameraY * _arg_4)) + (_local_19->cameraZ * _arg_5));
			bool _local_23 = (((_local_20 < _arg_7) || (_local_21 < _arg_7)) || (_local_22 < _arg_7));
			bool _local_24 = (((_local_20 > _arg_8) || (_local_21 > _arg_8)) || (_local_22 > _arg_8));
			while (_local_11 != nullptr)
			{
				Shared<Vertex> _local_12 = _local_11->vertex;
				double _local_25 = (((_local_12->cameraX * _arg_3) + (_local_12->cameraY * _arg_4)) + (_local_12->cameraZ * _arg_5));
				if (_local_25 < _arg_7)
				{
					_local_23 = true;
				}
				else
				{
					if (_local_25 > _arg_8)
					{
						_local_24 = true;
					}
				}
				_local_12->offset = _local_25;
				_local_11 = _local_11->next;
			}
			if ((!(_local_24)))
			{
				_local_9->processNext = nullptr;
			}
			else
			{
				if ((!(_local_23)))
				{
					if (_local_15 != nullptr)
					{
						_local_16->processNext = _local_9;
					}
					else
					{
						_local_15 = _local_9;
					}
					_local_16 = _local_9;
				}
				else
				{
					_local_17->offset = _local_20;
					_local_18->offset = _local_21;
					_local_19->offset = _local_22;
					Shared<Face> _local_26 = _local_9->create(9);
					_local_26->material = _local_9->material;
					_arg_1->lastFace->next = _local_26;
					_arg_1->lastFace = _local_26;
					Shared<Wrapper> _local_27 = nullptr;
					_local_11 = _local_9->wrapper->next->next;
					while (_local_11->next != nullptr)
					{
						_local_11 = _local_11->next;
					}
					_local_17 = _local_11->vertex;
					_local_20 = _local_17->offset;
					bool _local_29 = ((!(_local_9->material == nullptr)) && (_local_9->material->useVerticesNormals));
					_local_11 = _local_9->wrapper;
					while (_local_11 != nullptr)
					{
						_local_18 = _local_11->vertex;
						_local_21 = _local_18->offset;
						if ((((_local_20 < _arg_7) && (_local_21 > _arg_8)) || ((_local_20 > _arg_8) && (_local_21 < _arg_7))))
						{
							double _local_30 = ((_arg_6 - _local_20) / (_local_21 - _local_20));
							Shared<Vertex> _local_12 = _local_18->create();
							_arg_1->lastVertex->next = _local_12;
							_arg_1->lastVertex = _local_12;
							_local_12->x = (_local_17->x + ((_local_18->x - _local_17->x) * _local_30));
							_local_12->y = (_local_17->y + ((_local_18->y - _local_17->y) * _local_30));
							_local_12->z = (_local_17->z + ((_local_18->z - _local_17->z) * _local_30));
							_local_12->u = (_local_17->u + ((_local_18->u - _local_17->u) * _local_30));
							_local_12->v = (_local_17->v + ((_local_18->v - _local_17->v) * _local_30));
							_local_12->cameraX = (_local_17->cameraX + ((_local_18->cameraX - _local_17->cameraX) * _local_30));
							_local_12->cameraY = (_local_17->cameraY + ((_local_18->cameraY - _local_17->cameraY) * _local_30));
							_local_12->cameraZ = (_local_17->cameraZ + ((_local_18->cameraZ - _local_17->cameraZ) * _local_30));
							if (_local_29)
							{
								_local_12->normalX = (_local_17->normalX + ((_local_18->normalX - _local_17->normalX) * _local_30));
								_local_12->normalY = (_local_17->normalY + ((_local_18->normalY - _local_17->normalY) * _local_30));
								_local_12->normalZ = (_local_17->normalZ + ((_local_18->normalZ - _local_17->normalZ) * _local_30));
							}
							Shared<Wrapper> _local_28 = _local_11->create();
							_local_28->vertex = _local_12;
							if (_local_27 != nullptr)
							{
								_local_27->next = _local_28;
							}
							else
							{
								_local_26->wrapper = _local_28;
							}
							_local_27 = _local_28;
						}
						if (_local_21 >= _arg_7)
						{
							Shared<Wrapper> _local_28 = _local_11->create();
							_local_28->vertex = _local_18;
							if (_local_27 != nullptr)
							{
								_local_27->next = _local_28;
							}
							else
							{
								_local_26->wrapper = _local_28;
							}
							_local_27 = _local_28;
						}
						_local_17 = _local_18;
						_local_20 = _local_21;
						_local_11 = _local_11->next;
					}
					if (_local_15 != nullptr)
					{
						_local_16->processNext = _local_26;
					}
					else
					{
						_local_15 = _local_26;
					}
					_local_16 = _local_26;
					_local_9->processNext = nullptr;
				}
			}
			_local_9 = _local_10;
		}
	}
	if (((!(_local_15 == nullptr)) || ((!(_local_13 == nullptr)) && (!(_local_14 == nullptr)))))
	{
		if (_local_15 == nullptr)
		{
			_local_15 = _arg_2->create(10);
			_arg_1->lastFace->next = _local_15;
			_arg_1->lastFace = _local_15;
		}
		else
		{
			_local_16->processNext = nullptr;
		}
		if (this->sorting == 3)
		{
			_local_15->normalX = _arg_2->normalX;
			_local_15->normalY = _arg_2->normalY;
			_local_15->normalZ = _arg_2->normalZ;
			_local_15->offset = _arg_2->offset;
		}
		_local_15->processNegative = _local_13;
		_local_15->processPositive = _local_14;
		return _local_15;
	}
	return ((_local_13 != nullptr) ? _local_13 : _local_14);
}


void VG::updateAABBStruct(Shared<Face> _arg_1, int _arg_2)
{
	Shared<Face> _local_3 = _arg_1;
	while (_local_3 != nullptr)
	{
		Weak<Wrapper> _local_4 = _local_3->wrapper;
		while (_local_4 != nullptr)
		{
			Vertex *_local_5 = _local_4->vertex.get();
			if (_local_5->transformId != _arg_2)
			{
				if (_local_5->cameraX < this->boundMinX)
				{
					this->boundMinX = _local_5->cameraX;
				}
				if (_local_5->cameraX > this->boundMaxX)
				{
					this->boundMaxX = _local_5->cameraX;
				}
				if (_local_5->cameraY < this->boundMinY)
				{
					this->boundMinY = _local_5->cameraY;
				}
				if (_local_5->cameraY > this->boundMaxY)
				{
					this->boundMaxY = _local_5->cameraY;
				}
				if (_local_5->cameraZ < this->boundMinZ)
				{
					this->boundMinZ = _local_5->cameraZ;
				}
				if (_local_5->cameraZ > this->boundMaxZ)
				{
					this->boundMaxZ = _local_5->cameraZ;
				}
				_local_5->transformId = _arg_2;
			}
			_local_4 = _local_4->next;
		}
		_local_3 = _local_3->processNext;
	}
	if (_arg_1->processNegative != nullptr)
	{
		this->updateAABBStruct(_arg_1->processNegative, _arg_2);
	}
	if (_arg_1->processPositive != nullptr)
	{
		this->updateAABBStruct(_arg_1->processPositive, _arg_2);
	}
}


void VG::splitFaceStruct(Camera3D *_arg_1, Shared<Face> _arg_2, Shared<Face> _arg_3, double _arg_4, double _arg_5, double _arg_6, double _arg_7, double _arg_8, double _arg_9)
{
	Shared<Face> _local_15 = nullptr;
	Shared<Face> _local_16 = nullptr;
	Shared<Face> _local_17 = nullptr;
	Shared<Face> _local_18 = nullptr;
	Shared<Face> _local_19 = nullptr;
	Shared<Face> _local_20 = nullptr;
	Shared<Face> _local_21 = nullptr;
	Shared<Face> _local_22 = nullptr;


	if (_arg_2->processNegative != nullptr)
	{
		this->splitFaceStruct(_arg_1, _arg_2->processNegative, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9);
		_arg_2->processNegative = nullptr;
		_local_15 = _arg_3->processNegative;
		_local_16 = _arg_3->processPositive;
	}

	if (_arg_2->processPositive != nullptr)
	{
		this->splitFaceStruct(_arg_1, _arg_2->processPositive, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9);
		_arg_2->processPositive = nullptr;
		_local_17 = _arg_3->processNegative;
		_local_18 = _arg_3->processPositive;
	}
	if (_arg_2->wrapper != nullptr)
	{
		Shared<Face> _local_10 = _arg_2;
		while (_local_10 != nullptr)
		{
			Shared<Face> _local_11 = _local_10->processNext;
			Shared<Wrapper> _local_12 = _local_10->wrapper;
			Shared<Vertex> _local_29 = _local_12->vertex;
			_local_12 = _local_12->next;
			Shared<Vertex> _local_30 = _local_12->vertex;
			_local_12 = _local_12->next;
			Shared<Vertex> _local_31 = _local_12->vertex;
			_local_12 = _local_12->next;
			double _local_32 = (((_local_29->cameraX * _arg_4) + (_local_29->cameraY * _arg_5)) + (_local_29->cameraZ * _arg_6));
			double _local_33 = (((_local_30->cameraX * _arg_4) + (_local_30->cameraY * _arg_5)) + (_local_30->cameraZ * _arg_6));
			double _local_34 = (((_local_31->cameraX * _arg_4) + (_local_31->cameraY * _arg_5)) + (_local_31->cameraZ * _arg_6));
			bool _local_35 = (((_local_32 < _arg_8) || (_local_33 < _arg_8)) || (_local_34 < _arg_8));
			bool _local_36 = (((_local_32 > _arg_9) || (_local_33 > _arg_9)) || (_local_34 > _arg_9));
			bool _local_37 = (((_local_32 < _arg_8) && (_local_33 < _arg_8)) && (_local_34 < _arg_8));
			while (_local_12 != nullptr)
			{
				Shared<Vertex> _local_13 = _local_12->vertex;
				double _local_38 = (((_local_13->cameraX * _arg_4) + (_local_13->cameraY * _arg_5)) + (_local_13->cameraZ * _arg_6));
				if (_local_38 < _arg_8)
				{
					_local_35 = true;
				}
				else
				{
					_local_37 = false;
					if (_local_38 > _arg_9)
					{
						_local_36 = true;
					}
				}
				_local_13->offset = _local_38;
				_local_12 = _local_12->next;
			}
			if ((!(_local_35)))
			{
				if (_local_21 != nullptr)
				{
					_local_22->processNext = _local_10;
				}
				else
				{
					_local_21 = _local_10;
				}
				_local_22 = _local_10;
			}
			else
			{
				if ((!(_local_36)))
				{
					if (_local_37)
					{
						if (_local_19 != nullptr)
						{
							_local_20->processNext = _local_10;
						}
						else
						{
							_local_19 = _local_10;
						}
						_local_20 = _local_10;
					}
					else
					{
						_local_29->offset = _local_32;
						_local_30->offset = _local_33;
						_local_31->offset = _local_34;
						Shared<Face> _local_23 = _local_10->create(11);
						_local_23->material = _local_10->material;
						_arg_1->lastFace->next = _local_23;
						_arg_1->lastFace = _local_23;
						Shared<Wrapper> _local_25 = nullptr;
						bool _local_28 = ((!(_local_10->material == nullptr)) && (_local_10->material->useVerticesNormals));
						_local_12 = _local_10->wrapper;
						while (_local_12 != nullptr)
						{
							_local_30 = _local_12->vertex;
							if (_local_30->offset >= _arg_8)
							{
								Shared<Vertex> _local_14 = _local_30->create();
								_arg_1->lastVertex->next = _local_14;
								_arg_1->lastVertex = _local_14;
								_local_14->x = _local_30->x;
								_local_14->y = _local_30->y;
								_local_14->z = _local_30->z;
								_local_14->u = _local_30->u;
								_local_14->v = _local_30->v;
								_local_14->cameraX = _local_30->cameraX;
								_local_14->cameraY = _local_30->cameraY;
								_local_14->cameraZ = _local_30->cameraZ;
								if (_local_28)
								{
									_local_14->normalX = _local_30->normalX;
									_local_14->normalY = _local_30->normalY;
									_local_14->normalZ = _local_30->normalZ;
								}
								_local_30 = _local_14;
							}
							Shared<Wrapper> _local_27 = _local_12->create();
							_local_27->vertex = _local_30;
							if (_local_25 != nullptr)
							{
								_local_25->next = _local_27;
							}
							else
							{
								_local_23->wrapper = _local_27;
							}
							_local_25 = _local_27;
							_local_12 = _local_12->next;
						}
						if (_local_19 != nullptr)
						{
							_local_20->processNext = _local_23;
						}
						else
						{
							_local_19 = _local_23;
						}
						_local_20 = _local_23;
						_local_10->processNext = nullptr;
					}
				}
				else
				{
					_local_29->offset = _local_32;
					_local_30->offset = _local_33;
					_local_31->offset = _local_34;
					Shared<Face> _local_23 = _local_10->create(12);
					_local_23->material = _local_10->material;
					_arg_1->lastFace->next = _local_23;
					_arg_1->lastFace = _local_23;
					Shared<Face> _local_24 = _local_10->create(13);
					_local_24->material = _local_10->material;
					_arg_1->lastFace->next = _local_24;
					_arg_1->lastFace = _local_24;
					Shared<Wrapper> _local_25 = nullptr;
					Shared<Wrapper> _local_26 = nullptr;
					_local_12 = _local_10->wrapper->next->next;
					while (_local_12->next != nullptr)
					{
						_local_12 = _local_12->next;
					}
					_local_29 = _local_12->vertex;
					_local_32 = _local_29->offset;
					bool _local_28 = ((!(_local_10->material == nullptr)) && (_local_10->material->useVerticesNormals));
					_local_12 = _local_10->wrapper;
					while (_local_12 != nullptr)
					{
						_local_30 = _local_12->vertex;
						_local_33 = _local_30->offset;
						if ((((_local_32 < _arg_8) && (_local_33 > _arg_9)) || ((_local_32 > _arg_9) && (_local_33 < _arg_8))))
						{
							double _local_39 = ((_arg_7 - _local_32) / (_local_33 - _local_32));
							Shared<Vertex> _local_13 = _local_30->create();
							_arg_1->lastVertex->next = _local_13;
							_arg_1->lastVertex = _local_13;
							_local_13->x = (_local_29->x + ((_local_30->x - _local_29->x) * _local_39));
							_local_13->y = (_local_29->y + ((_local_30->y - _local_29->y) * _local_39));
							_local_13->z = (_local_29->z + ((_local_30->z - _local_29->z) * _local_39));
							_local_13->u = (_local_29->u + ((_local_30->u - _local_29->u) * _local_39));
							_local_13->v = (_local_29->v + ((_local_30->v - _local_29->v) * _local_39));
							_local_13->cameraX = (_local_29->cameraX + ((_local_30->cameraX - _local_29->cameraX) * _local_39));
							_local_13->cameraY = (_local_29->cameraY + ((_local_30->cameraY - _local_29->cameraY) * _local_39));
							_local_13->cameraZ = (_local_29->cameraZ + ((_local_30->cameraZ - _local_29->cameraZ) * _local_39));
							if (_local_28)
							{
								_local_13->normalX = (_local_29->normalX + ((_local_30->normalX - _local_29->normalX) * _local_39));
								_local_13->normalY = (_local_29->normalY + ((_local_30->normalY - _local_29->normalY) * _local_39));
								_local_13->normalZ = (_local_29->normalZ + ((_local_30->normalZ - _local_29->normalZ) * _local_39));
							}
							Shared<Wrapper> _local_27 = _local_12->create();
							_local_27->vertex = _local_13;
							if (_local_25 != nullptr)
							{
								_local_25->next = _local_27;
							}
							else
							{
								_local_23->wrapper = _local_27;
							}
							_local_25 = _local_27;
							Shared<Vertex> _local_14 = _local_30->create();
							_arg_1->lastVertex->next = _local_14;
							_arg_1->lastVertex = _local_14;
							_local_14->x = _local_13->x;
							_local_14->y = _local_13->y;
							_local_14->z = _local_13->z;
							_local_14->u = _local_13->u;
							_local_14->v = _local_13->v;
							_local_14->cameraX = _local_13->cameraX;
							_local_14->cameraY = _local_13->cameraY;
							_local_14->cameraZ = _local_13->cameraZ;
							if (_local_28)
							{
								_local_14->normalX = _local_13->normalX;
								_local_14->normalY = _local_13->normalY;
								_local_14->normalZ = _local_13->normalZ;
							}
							_local_27 = _local_12->create();
							_local_27->vertex = _local_14;
							if (_local_26 != nullptr)
							{
								_local_26->next = _local_27;
							}
							else
							{
								_local_24->wrapper = _local_27;
							}
							_local_26 = _local_27;
						}
						if (_local_30->offset < _arg_8)
						{
							Shared<Wrapper> _local_27 = _local_12->create();
							_local_27->vertex = _local_30;
							if (_local_25 != nullptr)
							{
								_local_25->next = _local_27;
							}
							else
							{
								_local_23->wrapper = _local_27;
							}
							_local_25 = _local_27;
						}
						else
						{
							if (_local_30->offset > _arg_9)
							{
								Shared<Wrapper> _local_27 = _local_12->create();
								_local_27->vertex = _local_30;
								if (_local_26 != nullptr)
								{
									_local_26->next = _local_27;
								}
								else
								{
									_local_24->wrapper = _local_27;
								}
								_local_26 = _local_27;
							}
							else
							{
								Shared<Wrapper> _local_27 = _local_12->create();
								_local_27->vertex = _local_30;
								if (_local_26 != nullptr)
								{
									_local_26->next = _local_27;
								}
								else
								{
									_local_24->wrapper = _local_27;
								}
								_local_26 = _local_27;
								Shared<Vertex> _local_14 = _local_30->create();
								_arg_1->lastVertex->next = _local_14;
								_arg_1->lastVertex = _local_14;
								_local_14->x = _local_30->x;
								_local_14->y = _local_30->y;
								_local_14->z = _local_30->z;
								_local_14->u = _local_30->u;
								_local_14->v = _local_30->v;
								_local_14->cameraX = _local_30->cameraX;
								_local_14->cameraY = _local_30->cameraY;
								_local_14->cameraZ = _local_30->cameraZ;
								if (_local_28)
								{
									_local_14->normalX = _local_30->normalX;
									_local_14->normalY = _local_30->normalY;
									_local_14->normalZ = _local_30->normalZ;
								}
								_local_27 = _local_12->create();
								_local_27->vertex = _local_14;
								if (_local_25 != nullptr)
								{
									_local_25->next = _local_27;
								}
								else
								{
									_local_23->wrapper = _local_27;
								}
								_local_25 = _local_27;
							}
						}
						_local_29 = _local_30;
						_local_32 = _local_33;
						_local_12 = _local_12->next;
					}
					if (_local_19 != nullptr)
					{
						_local_20->processNext = _local_23;
					}
					else
					{
						_local_19 = _local_23;
					}
					_local_20 = _local_23;
					if (_local_21 != nullptr)
					{
						_local_22->processNext = _local_24;
					}
					else
					{
						_local_21 = _local_24;
					}
					_local_22 = _local_24;
					_local_10->processNext = nullptr;
				}
			}
			_local_10 = _local_11;
		}
	}
	if (((!(_local_19 == nullptr)) || ((!(_local_15 == nullptr)) && (!(_local_17 == nullptr)))))
	{
		if (_local_19 == nullptr)
		{
			_local_19 = _arg_2->create(14);
			_arg_1->lastFace->next = _local_19;
			_arg_1->lastFace = _local_19;
		}
		else
		{
			_local_20->processNext = nullptr;
		}
		if (this->sorting == 3)
		{
			_local_19->normalX = _arg_2->normalX;
			_local_19->normalY = _arg_2->normalY;
			_local_19->normalZ = _arg_2->normalZ;
			_local_19->offset = _arg_2->offset;
		}
		_local_19->processNegative = _local_15;
		_local_19->processPositive = _local_17;
		_arg_3->processNegative = _local_19;
	}
	else
	{
		_arg_3->processNegative = ((_local_15 != nullptr) ? _local_15 : _local_17);
	}
	if (((!(_local_21 == nullptr)) || ((!(_local_16 == nullptr)) && (!(_local_18 == nullptr)))))
	{
		if (_local_21 == nullptr)
		{
			_local_21 = _arg_2->create(15);
			_arg_1->lastFace->next = _local_21;
			_arg_1->lastFace = _local_21;
		}
		else
		{
			_local_22->processNext = nullptr;
		}
		if (this->sorting == 3)
		{
			_local_21->normalX = _arg_2->normalX;
			_local_21->normalY = _arg_2->normalY;
			_local_21->normalZ = _arg_2->normalZ;
			_local_21->offset = _arg_2->offset;
		}
		_local_21->processNegative = _local_16;
		_local_21->processPositive = _local_18;
		_arg_3->processPositive = _local_21;
	}
	else
	{
		_arg_3->processPositive = ((_local_16 != nullptr) ? _local_16 : _local_18);
	}
}
