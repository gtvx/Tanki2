#include "Occluder.h"
#include "Error.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include "hardware/alternativa/engine3d/core/Wrapper.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include <QString>
#include <QDebug>


std::shared_ptr<Occluder> new_Occluder()
{
	return std::make_shared<Occluder>();
}


Occluder::Occluder()
{

}


std::shared_ptr<Object3D> Occluder::clone() const
{
	auto out = new_Occluder();
	out->clonePropertiesFrom(this);
	return out;
}


void Occluder::clonePropertiesFrom(const Object3D *s)
{
	Object3D::clonePropertiesFrom(s);

	const Occluder *src = (const Occluder*)s;

	this->faceList = src->faceList;
	this->edgeList = src->edgeList;
	this->vertexList = src->vertexList;
}


void Occluder::createForm(std::shared_ptr<Mesh> mesh, bool c)
{
	this->destroyForm();

	if (!c)
	{
		mesh = std::dynamic_pointer_cast<Mesh>(mesh->clone());
	}

	this->faceList = mesh->faceList;
	this->vertexList = mesh->vertexList;

	mesh->faceList = nullptr;
	mesh->vertexList = nullptr;

	Vertex *vertex = this->vertexList.get();
	while (vertex != nullptr)
	{
		vertex->transformId = 0;
		//vertex->id = nullptr;
		vertex = vertex->next.get();
	}

	Weak<Face> face = this->faceList;

	while (face != nullptr)
	{
		//face->id = nullptr;
		face = face->next;
	}


	QString error = this->calculateEdges();

	if (!error.isNull())
	{
		qDebug() << "Occluder::createForm error" << error;
		this->destroyForm();
		throw ArgumentError(error);
	}

	calculateBounds();
}


void Occluder::destroyForm()
{
	this->faceList = nullptr;
	this->edgeList = nullptr;
	this->vertexList = nullptr;
}


QString Occluder::calculateEdges()
{
	Shared<Face> _local_1 = this->faceList;
	while (_local_1 != nullptr)
	{
		_local_1->calculateBestSequenceAndNormal();

		Weak<Wrapper> _local_2 = _local_1->wrapper;
		while (_local_2 != nullptr)
		{
			Shared<Vertex> _local_4 = _local_2->vertex;
			Shared<Vertex> _local_5 = ((_local_2->next != nullptr) ? _local_2->next->vertex : _local_1->wrapper->vertex);
			std::shared_ptr<Edge> _local_3 = this->edgeList;
			while (_local_3 != nullptr)
			{
				if (((_local_3->a == _local_4) && (_local_3->b == _local_5)))
				{
					return ("The supplied geometry is not valid.");
				}
				if (((_local_3->a == _local_5) && (_local_3->b == _local_4))) break;
				_local_3 = _local_3->next;
			}
			if (_local_3 != nullptr)
			{
				_local_3->right = _local_1;
			}
			else
			{
				_local_3 = std::make_shared<Edge>();
				_local_3->a = _local_4;
				_local_3->b = _local_5;
				_local_3->left = _local_1;
				_local_3->next = this->edgeList;
				this->edgeList = _local_3;
			}
			_local_2 = _local_2->next;
			_local_4 = _local_5;
		}

		_local_1 = _local_1->next;
	}

	std::shared_ptr<Edge> _local_3 = this->edgeList;
	while (_local_3 != nullptr)
	{
		if (((_local_3->left == nullptr) || (_local_3->right == nullptr)))
		{
			return ("The supplied geometry is non whole.");
		}
		double _local_6 = (_local_3->b->x - _local_3->a->x);
		double _local_7 = (_local_3->b->y - _local_3->a->y);
		double _local_8 = (_local_3->b->z - _local_3->a->z);
		double _local_9 = ((_local_3->right->normalZ * _local_3->left->normalY) - (_local_3->right->normalY * _local_3->left->normalZ));
		double _local_10 = ((_local_3->right->normalX * _local_3->left->normalZ) - (_local_3->right->normalZ * _local_3->left->normalX));
		double _local_11 = ((_local_3->right->normalY * _local_3->left->normalX) - (_local_3->right->normalX * _local_3->left->normalY));
		if ((((_local_6 * _local_9) + (_local_7 * _local_10)) + (_local_8 * _local_11)) < 0)
		{

		}

		_local_3 = _local_3->next;
	}

	return {};
}


void Occluder::updateBounds(Object3D *_arg_1, Object3D *_arg_2)
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
