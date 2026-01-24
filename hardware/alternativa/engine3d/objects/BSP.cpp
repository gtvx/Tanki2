#include "BSP.h"
#include "Mesh.h"
#include "../core/Face.h"
#include "../core/Vertex.h"
#include "../core/Wrapper.h"
#include "flash/geom/Vector3D.h"
#include "../core/Faces.h"
#include <QMap>
#include <QDebug>


class Node
{
public:
	std::shared_ptr<Node> negative = nullptr;
	std::shared_ptr<Node> positive = nullptr;
	Shared<Face> faceList = nullptr;
	double normalX = 0;
	double normalY = 0;
	double normalZ = 0;
	double offset = 0;
};


std::shared_ptr<BSP> new_BSP()
{
	return std::make_shared<BSP>();
}


BSP::BSP()
{
	root = nullptr;
	vertexList = nullptr;
	splitAnalysis = true;
	threshold = 0.01;
	clipping = 2;
}


static std::shared_ptr<Node> cloneNode(std::shared_ptr<Node> _arg_1, QMap<Shared<Face>, Shared<Face>> &_arg_2)
{
	Shared<Face> _local_4 = nullptr;

	std::shared_ptr<Node> _local_3 = std::make_shared<Node>();
	Shared<Face> _local_5 = _arg_1->faceList;

	while (_local_5 != nullptr)
	{
		Shared<Face> _local_6 = _arg_2[_local_5];
		if (_local_6 == nullptr)
		{
			_local_6 = Face::create(17);
			_local_6->material = _local_5->material;
			_local_6->normalX = _local_5->normalX;
			_local_6->normalY = _local_5->normalY;
			_local_6->normalZ = _local_5->normalZ;
			_local_6->offset = _local_5->offset;
			Shared<Wrapper> _local_7 = nullptr;
			Shared<Wrapper> _local_8 = _local_5->wrapper;
			while (_local_8 != nullptr)
			{
				Shared<Wrapper> _local_9 = Wrapper::create();
				_local_9->vertex = _local_8->vertex->value;
				if (_local_7 != nullptr)
				{
					_local_7->next = _local_9;
				}
				else
				{
					_local_6->wrapper = _local_9;
				}
				_local_7 = _local_9;
				_local_8 = _local_8->next;
			}
		}

		if (_local_3->faceList != nullptr)
		{
			_local_4->next = _local_6;
		}
		else
		{
			_local_3->faceList = _local_6;
		}

		_local_4 = _local_6;
		_local_5 = _local_5->next;
	}

	_local_3->normalX = _arg_1->normalX;
	_local_3->normalY = _arg_1->normalY;
	_local_3->normalZ = _arg_1->normalZ;
	_local_3->offset = _arg_1->offset;

	if (_arg_1->negative != nullptr)
	{
		_local_3->negative = cloneNode(_arg_1->negative, _arg_2);
	}

	if (_arg_1->positive != nullptr)
	{
		_local_3->positive = cloneNode(_arg_1->positive, _arg_2);
	}

	return _local_3;
}


void BSP::clonePropertiesFrom(const Object3D *src)
{
	Shared<Vertex> _local_4 = nullptr;

	Object3D::clonePropertiesFrom(src);


	BSP *_local_2 = (BSP*)(src);

	this->clipping = _local_2->clipping;
	this->threshold = _local_2->threshold;
	this->splitAnalysis = _local_2->splitAnalysis;
	Shared<Vertex> _local_3 = _local_2->vertexList;
	while (_local_3 != nullptr)
	{
		Shared<Vertex> _local_8 = Vertex::create();
		_local_8->x = _local_3->x;
		_local_8->y = _local_3->y;
		_local_8->z = _local_3->z;
		_local_8->u = _local_3->u;
		_local_8->v = _local_3->v;
		_local_8->normalX = _local_3->normalX;
		_local_8->normalY = _local_3->normalY;
		_local_8->normalZ = _local_3->normalZ;
		_local_3->value = _local_8;
		if (_local_4 != nullptr)
		{
			_local_4->next = _local_8;
		}
		else
		{
			this->vertexList = _local_8;
		}
		_local_4 = _local_8;
		_local_3 = _local_3->next;
	}

	QMap<Shared<Face>, Shared<Face>> _local_5;

	int _local_6 = _local_2->faces.length();
	int _local_7 = 0;

	while (_local_7 < _local_6)
	{
		Shared<Face> _local_9 = _local_2->faces[_local_7];
		Shared<Face> _local_10 = Face::create(18);
		_local_10->material = _local_9->material;
		_local_10->smoothingGroups = _local_9->smoothingGroups;
		_local_10->normalX = _local_9->normalX;
		_local_10->normalY = _local_9->normalY;
		_local_10->normalZ = _local_9->normalZ;
		_local_10->offset = _local_9->offset;
		Shared<Wrapper> _local_11 = nullptr;
		Shared<Wrapper> _local_12 = _local_9->wrapper;
		while (_local_12 != nullptr)
		{
			Shared<Wrapper> _local_13 = Wrapper::create();
			_local_13->vertex = _local_12->vertex->value;
			if (_local_11 != nullptr)
			{
				_local_11->next = _local_13;
			}
			else
			{
				_local_10->wrapper = _local_13;
			}
			_local_11 = _local_13;
			_local_12 = _local_12->next;
		}

		if (this->faces.length() <= _local_7)
			this->faces.resize(_local_7 + 1);

		this->faces[_local_7] = _local_10;
		_local_5[_local_9] = _local_10;
		_local_7++;
	}

	if (_local_2->root != nullptr)
	{
		//this->root = _local_2->cloneNode(_local_2->root, _local_5);
		this->root = cloneNode(_local_2->root, _local_5);
	}

	_local_3 = _local_2->vertexList;
	while (_local_3 != nullptr)
	{
		_local_3->value = nullptr;
		_local_3 = _local_3->next;
	}

}


std::shared_ptr<Object3D> BSP::clone() const
{
	auto out = new_BSP();
	out->clonePropertiesFrom(this);
	return out;
}


void BSP::destroyTree()
{

}


void BSP::createTree(std::shared_ptr<Mesh> _arg_1, bool _arg_2)
{
	this->destroyTree();

	if (!_arg_2)
	{
		_arg_1 = std::dynamic_pointer_cast<Mesh>(_arg_1->clone());
	}


	Shared<Face> _local_3 = _arg_1->faceList;

	this->vertexList = _arg_1->vertexList;

	_arg_1->faceList = nullptr;
	_arg_1->vertexList = nullptr;

	Vertex *_local_4 = this->vertexList.get();

	while (_local_4 != nullptr)
	{
		_local_4->transformId = 0;
		//_local_4->id = nullptr;
		_local_4 = _local_4->next.get();
	}

	this->faces.clear();

	int _local_5 = 0;
	Shared<Face> _local_6 = _local_3;
	while (_local_6 != nullptr)
	{
		_local_6->calculateBestSequenceAndNormal();
		//_local_6->id = nullptr;
		this->faces.append(_local_6);
		_local_5++;
		_local_6 = _local_6->next;
	}

	if (_local_3 != nullptr)
	{
		this->root = this->createNode(_local_3);
	}

	calculateBounds();
}


std::shared_ptr<Node> BSP::createNode(Shared<Face> _arg_1)
{
	Shared<Face> _local_21 = nullptr;
	Shared<Face> _local_22 = nullptr;
	Shared<Face> _local_25 = nullptr;
	Shared<Face> _local_26 = nullptr;

	int _local_27 = 0;
	Shared<Face> _local_28 = nullptr;
	int _local_29 = 0;
	Shared<Face> _local_32 = nullptr;
	Shared<Face> _local_33 = nullptr;

	std::shared_ptr<Node> _local_2 = std::make_shared<Node>();
	Shared<Face> _local_20 = _arg_1;


	Shared<Wrapper> _local_34 = nullptr;
	Shared<Wrapper> _local_35 = nullptr;

	if (((this->splitAnalysis) && (!(_arg_1->next == nullptr))))
	{
		_local_27 = 2147483647;
		_local_28 = _arg_1;
		while (_local_28 != nullptr)
		{
			double _local_14 = _local_28->normalX;
			double _local_15 = _local_28->normalY;
			double _local_16 = _local_28->normalZ;
			double _local_17 = _local_28->offset;
			double _local_18 = (_local_17 - this->threshold);
			double _local_19 = (_local_17 + this->threshold);
			_local_29 = 0;
			Shared<Face> _local_30 = _arg_1;
			while (_local_30 != nullptr)
			{
				if (_local_30 != _local_28)
				{
					Shared<Wrapper> _local_3 = _local_30->wrapper;
					Shared<Vertex> _local_4 = _local_3->vertex;
					_local_3 = _local_3->next;
					Shared<Vertex> _local_5 = _local_3->vertex;
					_local_3 = _local_3->next;
					Shared<Vertex> _local_6 = _local_3->vertex;
					_local_3 = _local_3->next;
					bool _local_10 = (((_local_4->x * _local_14) + (_local_4->y * _local_15)) + (_local_4->z * _local_16));
					bool _local_11 = (((_local_5->x * _local_14) + (_local_5->y * _local_15)) + (_local_5->z * _local_16));
					bool _local_12 = (((_local_6->x * _local_14) + (_local_6->y * _local_15)) + (_local_6->z * _local_16));
					bool _local_8 = (((_local_10 < _local_18) || (_local_11 < _local_18)) || (_local_12 < _local_18));
					bool _local_9 = (((_local_10 > _local_19) || (_local_11 > _local_19)) || (_local_12 > _local_19));
					while (_local_3 != nullptr)
					{
						Shared<Vertex> _local_7 = _local_3->vertex;
						double _local_13 = (((_local_7->x * _local_14) + (_local_7->y * _local_15)) + (_local_7->z * _local_16));
						if (_local_13 < _local_18)
						{
							_local_8 = true;
							if (_local_9) break;
						}
						else
						{
							if (_local_13 > _local_19)
							{
								_local_9 = true;
								if (_local_8) break;
							}
						}
						_local_3 = _local_3->next;
					}
					if (((_local_9) && (_local_8)))
					{
						_local_29++;
						if (_local_29 >= _local_27) break;
					}
				}
				_local_30 = _local_30->next;
			}
			if (_local_29 < _local_27)
			{
				_local_20 = _local_28;
				_local_27 = _local_29;
				if (_local_27 == 0) break;
			}
			_local_28 = _local_28->next;
		}
	}

	Shared<Face> _local_23 = _local_20;
	Shared<Face> _local_24 = _local_20->next;
	double _local_14 = _local_20->normalX;
	double _local_15 = _local_20->normalY;
	double _local_16 = _local_20->normalZ;
	double _local_17 = _local_20->offset;
	double _local_18 = (_local_17 - this->threshold);
	double _local_19 = (_local_17 + this->threshold);

	while (_arg_1 != nullptr)
	{
		if (_arg_1 != _local_20)
		{
			Shared<Face> _local_31 = _arg_1->next;
			Shared<Wrapper> _local_3 = _arg_1->wrapper;
			Shared<Vertex> _local_4 = _local_3->vertex;
			_local_3 = _local_3->next;
			Shared<Vertex> _local_5 = _local_3->vertex;
			_local_3 = _local_3->next;
			Shared<Vertex> _local_6 = _local_3->vertex;
			_local_3 = _local_3->next;
			double _local_10 = (((_local_4->x * _local_14) + (_local_4->y * _local_15)) + (_local_4->z * _local_16));
			double _local_11 = (((_local_5->x * _local_14) + (_local_5->y * _local_15)) + (_local_5->z * _local_16));
			double _local_12 = (((_local_6->x * _local_14) + (_local_6->y * _local_15)) + (_local_6->z * _local_16));
			bool _local_8 = (((_local_10 < _local_18) || (_local_11 < _local_18)) || (_local_12 < _local_18));
			bool _local_9 = (((_local_10 > _local_19) || (_local_11 > _local_19)) || (_local_12 > _local_19));
			while (_local_3 != nullptr)
			{
				Shared<Vertex> _local_7 = _local_3->vertex;
				double _local_13 = (((_local_7->x * _local_14) + (_local_7->y * _local_15)) + (_local_7->z * _local_16));
				if (_local_13 < _local_18)
				{
					_local_8 = true;
				}
				else
				{
					if (_local_13 > _local_19)
					{
						_local_9 = true;
					}
				}
				_local_7->offset = _local_13;
				_local_3 = _local_3->next;
			}
			if ((!(_local_8)))
			{
				if ((!(_local_9)))
				{
					if ((((_arg_1->normalX * _local_14) + (_arg_1->normalY * _local_15)) + (_arg_1->normalZ * _local_16)) > 0)
					{
						_local_23->next = _arg_1;
						_local_23 = _arg_1;
					}
					else
					{
						if (_local_21 != nullptr)
						{
							_local_22->next = _arg_1;
						}
						else
						{
							_local_21 = _arg_1;
						}
						_local_22 = _arg_1;
					}
				}
				else
				{
					if (_local_25 != nullptr)
					{
						_local_26->next = _arg_1;
					}
					else
					{
						_local_25 = _arg_1;
					}
					_local_26 = _arg_1;
				}
			}
			else
			{
				if ((!(_local_9)))
				{
					if (_local_21 != nullptr)
					{
						_local_22->next = _arg_1;
					}
					else
					{
						_local_21 = _arg_1;
					}
					_local_22 = _arg_1;
				}
				else
				{
					_local_4->offset = _local_10;
					_local_5->offset = _local_11;
					_local_6->offset = _local_12;
					_local_32 = Face::create(19);
					_local_33 = Face::create(20);
					_local_34 = nullptr;
					_local_35 = nullptr;
					_local_3 = _arg_1->wrapper->next->next;
					while (_local_3->next != nullptr)
					{
						_local_3 = _local_3->next;
					}
					Shared<Vertex> _local_4 = _local_3->vertex;
					_local_10 = _local_4->offset;
					_local_3 = _arg_1->wrapper;
					while (_local_3 != nullptr)
					{
						_local_5 = _local_3->vertex;
						_local_11 = _local_5->offset;
						if ((((_local_10 < _local_18) && (_local_11 > _local_19)) || ((_local_10 > _local_19) && (_local_11 < _local_18))))
						{
							double _local_37 = ((_local_17 - _local_10) / (_local_11 - _local_10));
							Shared<Vertex> _local_7 = Vertex::create();
							_local_7->next = this->vertexList;
							this->vertexList = _local_7;
							_local_7->x = (_local_4->x + ((_local_5->x - _local_4->x) * _local_37));
							_local_7->y = (_local_4->y + ((_local_5->y - _local_4->y) * _local_37));
							_local_7->z = (_local_4->z + ((_local_5->z - _local_4->z) * _local_37));
							_local_7->u = (_local_4->u + ((_local_5->u - _local_4->u) * _local_37));
							_local_7->v = (_local_4->v + ((_local_5->v - _local_4->v) * _local_37));
							_local_7->normalX = (_local_4->normalX + ((_local_5->normalX - _local_4->normalX) * _local_37));
							_local_7->normalY = (_local_4->normalY + ((_local_5->normalY - _local_4->normalY) * _local_37));
							_local_7->normalZ = (_local_4->normalZ + ((_local_5->normalZ - _local_4->normalZ) * _local_37));
							Shared<Wrapper> _local_36 = Wrapper::create();
							_local_36->vertex = _local_7;
							if (_local_34 != nullptr)
							{
								_local_34->next = _local_36;
							}
							else
							{
								_local_32->wrapper = _local_36;
							}
							_local_34 = _local_36;
							_local_36 = Wrapper::create();
							_local_36->vertex = _local_7;
							if (_local_35 != nullptr)
							{
								_local_35->next = _local_36;
							}
							else
							{
								_local_33->wrapper = _local_36;
							}
							_local_35 = _local_36;
						}
						if (_local_11 <= _local_19)
						{
							Shared<Wrapper> _local_36 = Wrapper::create();
							_local_36->vertex = _local_5;
							if (_local_34 != nullptr)
							{
								_local_34->next = _local_36;
							}
							else
							{
								_local_32->wrapper = _local_36;
							}
							_local_34 = _local_36;
						}
						if (_local_11 >= _local_18)
						{
							Shared<Wrapper> _local_36 = Wrapper::create();
							_local_36->vertex = _local_5;
							if (_local_35 != nullptr)
							{
								_local_35->next = _local_36;
							}
							else
							{
								_local_33->wrapper = _local_36;
							}
							_local_35 = _local_36;
						}
						_local_4 = _local_5;
						_local_10 = _local_11;
						_local_3 = _local_3->next;
					}
					_local_32->material = _arg_1->material;
					_local_32->smoothingGroups = _arg_1->smoothingGroups;
					_local_32->calculateBestSequenceAndNormal();
					if (_local_21 != nullptr)
					{
						_local_22->next = _local_32;
					}
					else
					{
						_local_21 = _local_32;
					}
					_local_22 = _local_32;
					_local_33->material = _arg_1->material;
					_local_33->smoothingGroups = _arg_1->smoothingGroups;
					_local_33->calculateBestSequenceAndNormal();
					if (_local_25 != nullptr)
					{
						_local_26->next = _local_33;
					}
					else
					{
						_local_25 = _local_33;
					}
					_local_26 = _local_33;
				}
			}
			_arg_1 = _local_31;
		}
		else
		{
			_arg_1 = _local_24;
		}
	}

	if (_local_21 != nullptr)
	{
		_local_22->next = nullptr;
		_local_2->negative = this->createNode(_local_21);
	}

	_local_23->next = nullptr;
	_local_2->faceList = _local_20;
	_local_2->normalX = _local_14;
	_local_2->normalY = _local_15;
	_local_2->normalZ = _local_16;
	_local_2->offset = _local_17;

	if (_local_25 != nullptr)
	{
		_local_26->next = nullptr;
		_local_2->positive = this->createNode(_local_25);
	}

	return _local_2;
}


static void setMaterialToNode(std::shared_ptr<Node> node, std::shared_ptr<Material> material)
{
	Weak<Face> face = node->faceList;
	while (face != nullptr)
	{
		face->material = material;
		face = face->next;
	}

	if (node->negative != nullptr)
	{
		setMaterialToNode(node->negative, material);
	}

	if (node->positive != nullptr)
	{
		setMaterialToNode(node->positive, material);
	}
}


void BSP::setMaterialToAllFaces(std::shared_ptr<Material> material)
{
	this->deleteResources();

	int count = this->faces.length();

	int i = 0;

	while (i < count)
	{
		this->faces[i]->material = material;
		i++;
	}

	if (this->root != nullptr)
	{
		setMaterialToNode(this->root, material);
	}
}


void BSP::deleteResources()
{

}


void BSP::updateBounds(Object3D *_arg_1, Object3D *_arg_2)
{
	Vertex *_local_3 = this->vertexList.get();

	while (_local_3 != nullptr)
	{
		if (_arg_2 != nullptr)
		{
			_local_3->cameraX = ((((_arg_2->matrix4.ma * _local_3->x) + (_arg_2->matrix4.mb * _local_3->y)) + (_arg_2->matrix4.mc * _local_3->z)) + _arg_2->matrix4.md);
			_local_3->cameraY = ((((_arg_2->matrix4.me * _local_3->x) + (_arg_2->matrix4.mf * _local_3->y)) + (_arg_2->matrix4.mg * _local_3->z)) + _arg_2->matrix4.mh);
			_local_3->cameraZ = ((((_arg_2->matrix4.mi * _local_3->x) + (_arg_2->matrix4.mj * _local_3->y)) + (_arg_2->matrix4.mk * _local_3->z)) + _arg_2->matrix4.ml);
		}
		else
		{
			_local_3->cameraX = _local_3->x;
			_local_3->cameraY = _local_3->y;
			_local_3->cameraZ = _local_3->z;
		}

		if (_local_3->cameraX < _arg_1->bound.boundMinX)
		{
			_arg_1->bound.boundMinX = _local_3->cameraX;
		}
		if (_local_3->cameraX > _arg_1->bound.boundMaxX)
		{
			_arg_1->bound.boundMaxX = _local_3->cameraX;
		}
		if (_local_3->cameraY < _arg_1->bound.boundMinY)
		{
			_arg_1->bound.boundMinY = _local_3->cameraY;
		}
		if (_local_3->cameraY > _arg_1->bound.boundMaxY)
		{
			_arg_1->bound.boundMaxY = _local_3->cameraY;
		}
		if (_local_3->cameraZ < _arg_1->bound.boundMinZ)
		{
			_arg_1->bound.boundMinZ = _local_3->cameraZ;
		}
		if (_local_3->cameraZ > _arg_1->bound.boundMaxZ)
		{
			_arg_1->bound.boundMaxZ = _local_3->cameraZ;
		}
		_local_3 = _local_3->next.get();
	}
}


void BSP::collectPlanesNode(std::shared_ptr<Node> _arg_1, const Vector3D *_arg_2, Faces *_arg_3)
{

	double _local_4 = ((((_arg_1->normalX * _arg_2->x) + (_arg_1->normalY * _arg_2->y)) + (_arg_1->normalZ * _arg_2->z)) - _arg_1->offset);

	if (_local_4 >= _arg_2->w)
	{
		if (_arg_1->positive != nullptr)
		{
			collectPlanesNode(_arg_1->positive, _arg_2, _arg_3);
		}
	}
	else
	{
		if (_local_4 <= -(_arg_2->w))
		{
			if (_arg_1->negative != nullptr)
			{
				collectPlanesNode(_arg_1->negative, _arg_2, _arg_3);
			}
		}
		else
		{
			Shared<Face> _local_5 = _arg_1->faceList;
			while (_local_5 != nullptr)
			{
				Weak<Wrapper> _local_6 = _local_5->wrapper;
				while (_local_6 != nullptr)
				{
					Vertex *_local_7 = _local_6->vertex.get();
					_local_7->cameraX = ((((this->matrix4.ma * _local_7->x) + (this->matrix4.mb * _local_7->y)) + (this->matrix4.mc * _local_7->z)) + this->matrix4.md);
					_local_7->cameraY = ((((this->matrix4.me * _local_7->x) + (this->matrix4.mf * _local_7->y)) + (this->matrix4.mg * _local_7->z)) + this->matrix4.mh);
					_local_7->cameraZ = ((((this->matrix4.mi * _local_7->x) + (this->matrix4.mj * _local_7->y)) + (this->matrix4.mk * _local_7->z)) + this->matrix4.ml);
					_local_6 = _local_6->next;
				}
				_arg_3->faces->append(_local_5);
				_local_5 = _local_5->next;
			}
			if (_arg_1->positive != nullptr)
			{
				collectPlanesNode(_arg_1->positive, _arg_2, _arg_3);
			}
			if (_arg_1->negative != nullptr)
			{
				collectPlanesNode(_arg_1->negative, _arg_2, _arg_3);
			}
		}
	}
}


void BSP::collectPlanes(const Vector3D *_arg_1, const Vector3D *_arg_2, const Vector3D *_arg_3,
						 const Vector3D *_arg_4, const Vector3D *_arg_5, Faces *_arg_6, void *_arg_7)
{

	(void)_arg_7;

#if 0
	if ((((!(_arg_7 == nullptr)) && (_arg_7[this])) || (this->root == nullptr)))
	{
		return;
	}
#endif

	if (this->root == nullptr)
		return;


	Vector3D _local_8;

	calculateSphere(_arg_1, _arg_2, _arg_3, _arg_4, _arg_5, &_local_8);

	if (!boundIntersectSphere(&_local_8, bound.boundMinX, bound.boundMinY, bound.boundMinZ, bound.boundMaxX, bound.boundMaxY, bound.boundMaxZ))
	{
		return;
	}

	collectPlanesNode(this->root, &_local_8, _arg_6);
}


void BSP::prepareResources()
{
	qDebug("used BSP::prepareResources");

#if 0
	var _local_1:Vector.<Number>;
	var _local_2:int;
	var _local_3:int;
	var _local_4:Vertex;
	var _local_5:Vector.<uint>;
	var _local_6:int;
	var _local_7:Face;
	var _local_8:Wrapper;
	var _local_9:uint;
	var _local_10:uint;
	var _local_11:uint;

	if (this.vertexBuffer == null)
	{
		_local_1 = new Vector.<Number>();
		_local_2 = 0;
		_local_3 = 0;
		_local_4 = this.vertexList;
		while (_local_4 != null)
		{
			_local_1[_local_2] = _local_4.x;
			_local_2++;
			_local_1[_local_2] = _local_4.y;
			_local_2++;
			_local_1[_local_2] = _local_4.z;
			_local_2++;
			_local_1[_local_2] = _local_4.u;
			_local_2++;
			_local_1[_local_2] = _local_4.v;
			_local_2++;
			_local_1[_local_2] = _local_4.normalX;
			_local_2++;
			_local_1[_local_2] = _local_4.normalY;
			_local_2++;
			_local_1[_local_2] = _local_4.normalZ;
			_local_2++;
			_local_4.index = _local_3;
			_local_3++;
			_local_4 = _local_4.next;
		};
		this.vertexBuffer = new VertexBufferResource(_local_1, 8);
		_local_5 = new Vector.<uint>();
		_local_6 = 0;
		this.numTriangles = 0;
		for each (_local_7 in this.faces)
		{
			_local_8 = _local_7.wrapper;
			_local_9 = _local_8.vertex.index;
			_local_8 = _local_8.next;
			_local_10 = _local_8.vertex.index;
			_local_8 = _local_8.next;
			while (_local_8 != null)
			{
				_local_11 = _local_8.vertex.index;
				_local_5[_local_6] = _local_9;
				_local_6++;
				_local_5[_local_6] = _local_10;
				_local_6++;
				_local_5[_local_6] = _local_11;
				_local_6++;
				_local_10 = _local_11;
				this.numTriangles++;
				_local_8 = _local_8.next;
			}
		}
		this.indexBuffer = new IndexBufferResource(_local_5);
	}
#endif
}
