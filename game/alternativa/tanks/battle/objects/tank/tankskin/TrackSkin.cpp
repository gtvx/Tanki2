#include "TrackSkin.h"
#include "hardware/alternativa/tanks/materials/TrackMaterial.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "MyMath.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include "hardware/alternativa/engine3d/core/Wrapper.h"
#include "hardware/alternativa/engine3d/materials/UVMatrixProvider.h"
#include "flash/geom/Matrix.h"
#include "Material_isType.h"
#include "shared_qhash.h"
#include <QHash>


static QVector<Shared<Vertex>> Face_getVertices(Weak<Face> face)
{
	QVector<Shared<Vertex>> result;

	Weak<Wrapper> wrapper = face->wrapper;

	while (wrapper != nullptr)
	{
		result.append(wrapper->vertex);
		wrapper = wrapper->next;
	}

	return result;
}


TrackSkin::TrackSkin()
{
	uvsProvider = nullptr;
	distance = 0;
}


static double getRatioForVertices(Vertex *_arg_1, Vertex *_arg_2)
{
	double _local_3 = (_arg_1->x - _arg_2->x);
	double _local_4 = (_arg_1->y - _arg_2->y);
	double _local_5 = (_arg_1->z - _arg_2->z);
	double _local_6 = MyMath::sqrt((((_local_3 * _local_3) + (_local_4 * _local_4)) + (_local_5 * _local_5)));
	double _local_7 = (_arg_1->u - _arg_2->u);
	double _local_8 = (_arg_1->v - _arg_2->v);
	double _local_9 = MyMath::sqrt(((_local_7 * _local_7) + (_local_8 * _local_8)));
	return _local_9 / _local_6;
}


static double getRatio(Shared<Face> face)
{
	QVector<Shared<Vertex>> list = Face_getVertices(face);
	return getRatioForVertices(list[0].get(), list[1].get());
}


void TrackSkin::addFace(Shared<Face> face)
{
	this->faces.append(face);
}


void TrackSkin::init()
{
	QHash<Shared<Vertex>, bool> _local_2;
	double value = 0;


	for (Shared<Face> _local_3 : this->faces)
	{
		for (Shared<Vertex> _local_5 : Face_getVertices(_local_3))
		{
			_local_2.insert(_local_5, true);
		}

		value = (value + getRatio(_local_3));
	}

	this->ratio = (value / this->faces.length());

	this->vertices.clear();

	QHashIterator<Shared<Vertex>, bool> iter(_local_2);
	while (iter.hasNext())
	{
		iter.next();
		this->vertices.append(iter.key());
	}
}


void TrackSkin::move(double value)
{
	this->distance = this->distance + (value * this->ratio);
	if (this->uvsProvider != nullptr)
	{
		Matrix *matrix = this->uvsProvider->getMatrix();
		matrix->tx = this->distance;
	}
}


void TrackSkin::setMaterial(std::shared_ptr<Material> material)
{
	for (Shared<Face> face : this->faces)
	{
		face->material = material;
	}

	if (Material_isType<TrackMaterial>(material.get()))
	{
		TrackMaterial *trackMaterial = (TrackMaterial*)(material.get());
		this->uvsProvider = trackMaterial->uvMatrixProvider;
	}
}
