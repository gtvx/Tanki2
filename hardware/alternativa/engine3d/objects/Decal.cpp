#include "Decal.h"
#include "../core/Vertex.h"
#include "../core/Face.h"
#include "../core/Wrapper.h"
#include "hardware/alternativa/gfx/core/VertexBufferResource.h"
#include "hardware/alternativa/gfx/core/IndexBufferResource.h"
#include "../core/Camera3D.h"
#include "../core/VG.h"


std::shared_ptr<Decal> new_Decal()
{
	return std::make_shared<Decal>();
}


Decal::Decal()
{
	shadowMapAlphaThreshold = 100;
	attenuation = 1000000;
}


#if 0
void Decal::createGeometry(Mesh *mesh, bool _arg_2)
{
	if (!_arg_2)
	{
		mesh = std::dynamic_pointer_cast<Mesh>(mesh->clone());
	}

	faceList = mesh->faceList;
	vertexList = mesh->vertexList;

	mesh->faceList = nullptr;
	mesh->vertexList = nullptr;

	Vertex *_local_3 = vertexList;
	while (_local_3 != nullptr)
	{
		_local_3->transformId = 0;
		//_local_3->id = nullptr;
		_local_3 = _local_3->next;
	}

	Face *_local_4 = faceList;
	while (_local_4 != nullptr)
	{
		//_local_4->id = nullptr;
		_local_4 = _local_4->next;
	}

	calculateBounds();
}
#endif


std::shared_ptr<Object3D> Decal::clone() const
{
	qDebug("used Decal::clone");
#if 0
	var _local_1:Decal = new Decal();
	_local_1.clonePropertiesFrom(this);
	return (_local_1);
#endif
	return nullptr;
}


void Decal::clonePropertiesFrom(Object3D *src)
{
	qDebug("used Decal::clonePropertiesFrom");
	(void)src;

#if 0
	super.clonePropertiesFrom(_arg_1);
	var _local_2:Decal = (_arg_1 as Decal);
	this.attenuation = _local_2.attenuation;
#endif
}


void Decal::draw(Camera3D *camera)
{
	if (faceList == nullptr)
	{
		return;
	}

	if (clipping == 0)
	{
		if (culling & 0x01)
		{
			return;
		}
		culling = 0;
	}

	this->prepareResources();

	useDepth = true;

	if (faceList->material != nullptr)
	{
		camera->addDecal(this);
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
	}

#if 0
	int _local_2 = ((_arg_1.debug) ? _arg_1.checkInDebug(this) : 0);
	if ((_local_2 & Debug.BOUNDS))
	{
		Debug.drawBounds(_arg_1, this, boundMinX, boundMinY, boundMinZ, boundMaxX, boundMaxY, boundMaxZ);
	};

	if ((_local_2 & Debug.EDGES))
	{
		if (transformId > 0x1DCD6500)
		{
			transformId = 0;
			Vertex *_local_4 = vertexList;
			while (_local_4 != null)
			{
				_local_4.transformId = 0;
				_local_4 = _local_4.next;
			}
		}

		transformId++;
		calculateInverseMatrix();

		Face *_local_3 = prepareFaces(_arg_1, faceList);
		if (_local_3 == null)
		{
			return;
		}

		Debug.drawEdges(_arg_1, _local_3, 0xFFFFFF);
	}
#endif
}


Shared<VG> Decal::getVG(Camera3D *camera)
{
	this->draw(camera);
	return nullptr;
}


void Decal::prepareResources()
{
	if (vertexBuffer == nullptr)
	{
		QVector<float> _local_1;

		int _local_3 = 0;

		Vertex *_local_4 = vertexList.get();

		while (_local_4 != nullptr)
		{
			_local_1.append(_local_4->x);
			_local_1.append(_local_4->y);
			_local_1.append(_local_4->z);
			_local_1.append(_local_4->u);
			_local_1.append(_local_4->v);
			_local_1.append(_local_4->normalX);
			_local_1.append(_local_4->normalY);
			_local_1.append(_local_4->normalZ);
			_local_4->index = _local_3;
			_local_3++;
			_local_4 = _local_4->next.get();
		}

		vertexBuffer = new VertexBufferResource(_local_1, 8);

		QVector<uint> _local_5;

		numTriangles = 0;
		Weak<Face> _local_7 = faceList;

		while (_local_7 != nullptr)
		{
			Weak<Wrapper> _local_8 = _local_7->wrapper;
			uint _local_9 = _local_8->vertex->index;
			_local_8 = _local_8->next;
			uint _local_10 = _local_8->vertex->index;
			_local_8 = _local_8->next;

			while (_local_8 != nullptr)
			{
				uint _local_11 = _local_8->vertex->index;

				_local_5.append(_local_9);
				_local_5.append(_local_10);
				_local_5.append(_local_11);
				_local_10 = _local_11;
				numTriangles++;
				_local_8 = _local_8->next;
			}
			_local_7 = _local_7->next;
		}

		indexBuffer = new IndexBufferResource(_local_5);
	}
}
