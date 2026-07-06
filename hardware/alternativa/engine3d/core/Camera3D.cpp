#include "Camera3D.h"
#include "MyMath.h"
#include "Object3DContainer.h"
#include "Face.h"
#include "Vertex.h"
#include "Wrapper.h"
#include "../materials/Material.h"
#include "Face.h"
#include "View.h"
#include "hardware/alternativa/engine3d/materials/DrawParams.h"
#include "hardware/alternativa/gfx/core/VertexBufferResource.h"
#include "hardware/alternativa/gfx/core/IndexBufferResource.h"
#include "Material_isType.h"
#include "../objects/Sprite3D.h"
#include "Object3D_isType.h"
#include "../objects/Decal.h"
#include "vulkan/VulkanWindow.h"
#include "vulkan/VulkanUniform.h"
#include "utils/float_array.h"


static const double fov = 1.5707963267948966;


#define VERTEX_BUFFER_SIZE 1000


struct VectexNode
{
	 VertexBufferResource *vertexBuffer = nullptr;
	 int vertexBufferSize = 0;
};



Camera3D::Camera3D()
{
	viewSizeX = 0;
	viewSizeY = 0;
	numDraws = 0;
	numPolygons = 0;
	numTriangles = 0;
	//lastWrapper = nullptr;
	lastVertex = nullptr;
	farClipping = 1000000;
	fov = 1.5707963267949;
	transparentCount = 0;

	numOccluders = 0;
	depthCount = 0;
	softTransparency = false;
	softTransparencyStrength = 1;
	ssao = false;
	ssaoStrength = 1;
	deferredLighting = false;
	deferredLightingStrength = 1;
	shadowMapStrength = 1;

	shadowMap = nullptr;

	view = new View;

	occludedAll = false;

	nearClipping = 1;

	decalsCount = 0;

	name_class = "Camera3D";

	//this->_diagram = this->createDiagram();
	this->firstVertex = Vertex::create();
	this->firstFace = Face::create(4);
	//this->firstWrapper = new Wrapper();
	//this->lastWrapper = this->firstWrapper;
	this->lastVertex = this->firstVertex;
	this->lastFace = this->firstFace;

	constantsVertexBuffer = nullptr;
	constantsIndexBuffer = nullptr;

	vertexBufferList = new VectexNode*[VERTEX_BUFFER_SIZE];

	for (int i = 0; i < VERTEX_BUFFER_SIZE; i++)
		vertexBufferList[i] = nullptr;

	vertexCurrentNumber = 0;

	float_array_set_8(fragmentConst, 0, 0, 0, 1, 0.5, 0.5, 0, (1 / 0x1000));
	vulkanUniform = new VulkanUniform;

}


void Camera3D::render(int, int)
{

}


void Camera3D::composeCameraMatrix()
{
	double _loc1_ = this->viewSizeX / this->focalLength;
	double _loc2_ = this->viewSizeY / this->focalLength;
	double _loc3_ = MyMath::cos(rotationX);
	double _loc4_ = MyMath::sin(rotationX);
	double _loc5_ = MyMath::cos(rotationY);
	double _loc6_ = MyMath::sin(rotationY);
	double _loc7_ = MyMath::cos(rotationZ);
	double _loc8_ = MyMath::sin(rotationZ);
	double _loc9_ = _loc7_ * _loc6_;
	double _loc10_ = _loc8_ * _loc6_;
	double _loc11_ = _loc5_;
	double _loc12_ = _loc4_;
	double _loc13_ = _loc3_;
	double _loc14_ = _loc3_;
	double _loc15_ = _loc4_;
	matrix4.ma = _loc7_ * _loc11_ * _loc1_;
	matrix4.mb = (_loc9_ * _loc12_ - _loc8_ * _loc13_) * _loc2_;
	matrix4.mc = _loc9_ * _loc14_ + _loc8_ * _loc15_;
	matrix4.md = x;
	matrix4.me = _loc8_ * _loc11_ * _loc1_;
	matrix4.mf = (_loc10_ * _loc12_ + _loc7_ * _loc13_) * _loc2_;
	matrix4.mg = _loc10_ * _loc14_ - _loc7_ * _loc15_;
	matrix4.mh = y;
	matrix4.mi = -_loc6_ * _loc1_;
	matrix4.mj = _loc5_ * _loc12_ * _loc2_;
	matrix4.mk = _loc5_ * _loc14_;
	matrix4.ml = z;
	double _loc16_ = 0 / this->viewSizeX;
	double _loc17_ = 0 / this->viewSizeY;
	matrix4.mc = matrix4.mc - (matrix4.ma * _loc16_ + matrix4.mb * _loc17_);
	matrix4.mg = matrix4.mg - (matrix4.me * _loc16_ + matrix4.mf * _loc17_);
	matrix4.mk = matrix4.mk - (matrix4.mi * _loc16_ + matrix4.mj * _loc17_);
}


void Camera3D::clonePropertiesFrom(const Object3D *in)
{
	Object3D::clonePropertiesFrom(in);

	const Camera3D *src = (const Camera3D*)in;

	this->viewSizeX = src->viewSizeX;
	this->viewSizeY = src->viewSizeY;;
	this->numDraws = src->numDraws;;
	this->numPolygons = src->numPolygons;;
	this->numTriangles = src->numTriangles;;
	this->focalLength = src->focalLength;;
	this->farClipping = src->farClipping;

#if 0
	var _local_2:Camera3D = (_arg_1 as Camera3D);
	this->fov = _local_2->fov;
	this->nearClipping = _local_2->nearClipping;
	this->farClipping = _local_2->farClipping;
	this->debug = _local_2->debug;
	this->fogNear = _local_2->fogNear;
	this->fogFar = _local_2->fogFar;
	this->fogAlpha = _local_2->fogAlpha;
	this->fogColor = _local_2->fogColor;
	this->softTransparency = _local_2->softTransparency;
	this->depthBufferScale = _local_2->depthBufferScale;
	this->ssao = _local_2->ssao;
	this->ssaoRadius = _local_2->ssaoRadius;
	this->ssaoRange = _local_2->ssaoRange;
	this->ssaoColor = _local_2->ssaoColor;
	this->ssaoAlpha = _local_2->ssaoAlpha;
	this->directionalLight = _local_2->directionalLight;
	this->shadowMap = _local_2->shadowMap;
	this->ambientColor = _local_2->ambientColor;
	this->deferredLighting = _local_2->deferredLighting;
	this->fogStrength = _local_2->fogStrength;
	this->softTransparencyStrength = _local_2->softTransparencyStrength;
	this->ssaoStrength = _local_2->ssaoStrength;
	this->directionalLightStrength = _local_2->directionalLightStrength;
	this->shadowMapStrength = _local_2->shadowMapStrength;
	this->shadowsStrength = _local_2->shadowsStrength;
	this->shadowsDistanceMultiplier = _local_2->shadowsDistanceMultiplier;
	this->deferredLightingStrength = _local_2->deferredLightingStrength;
	for (int _local_3 in _local_2->shadows)
	{
		this->shadows[_local_3] = true;
	}
#endif
}


void Camera3D::initVulkan(VulkanWindow *vulkanWindow)
{
	vulkanUniform->init(vulkanWindow, 2);
}


void Camera3D::renderVulkan(VulkanWindow *vulkanWindow)
{
	this->opaqueCount = 0;

	int zBufferPrecision = 16;

	this->viewSizeX = view->width * 0.5;
	this->viewSizeY = view->height * 0.5;

	this->focalLength = MyMath::sqrt(((this->viewSizeX * this->viewSizeX) + (this->viewSizeY * this->viewSizeY))) / MyMath::tan((this->fov * 0.5));

	this->correctionX = this->viewSizeX / this->focalLength;
	this->correctionY = this->viewSizeY / this->focalLength;

	this->projection[0] = 1 << zBufferPrecision;
	this->projection[1] = 1;
	this->projection[2] = this->farClipping / (this->farClipping - this->nearClipping);
	this->projection[3] = (this->nearClipping * this->farClipping) / (this->nearClipping - this->farClipping);

	this->fragmentConst[0] = this->farClipping;
	this->fragmentConst[1] = this->farClipping / 255;

	{
		float array[16];

		//vertex c3
		array[0] = this->projection[0];
		array[1] = this->projection[1];
		array[2] = this->projection[2];
		array[3] = this->projection[3];

		//fragment c17
		array[4] = this->fragmentConst[0];
		array[5] = this->fragmentConst[1];
		array[6] = this->fragmentConst[2];
		array[7] = this->fragmentConst[3];
		array[8] = this->fragmentConst[4];
		array[9] = this->fragmentConst[5];
		array[10] = this->fragmentConst[6];
		array[11] = this->fragmentConst[7];

		//vertex c254
		array[12] = 0.00125;
		array[13] = 0.00166667;
		array[14] = 0;
		array[15] = 0;


		vulkanUniform->upload(array, 0, 16 * sizeof(float));
	}


	this->composeCameraMatrix();

	Object3D *object = this;
	while (object->_parent != nullptr)
	{
		object = object->_parent.get();
		object->composeMatrix();
		appendMatrix(object);
	}

	this->matrix_g.copy(&this->matrix4);


	invertMatrix();
	this->transform[0] = this->matrix4.ma;
	this->transform[1] = this->matrix4.mb;
	this->transform[2] = this->matrix4.mc;
	this->transform[3] = this->matrix4.md;
	this->transform[4] = this->matrix4.me;
	this->transform[5] = this->matrix4.mf;
	this->transform[6] = this->matrix4.mg;
	this->transform[7] = this->matrix4.mh;
	this->transform[8] = this->matrix4.mi;
	this->transform[9] = this->matrix4.mj;
	this->transform[10] = this->matrix4.mk;
	this->transform[11] = this->matrix4.ml;
	this->numOccluders = 0;
	this->occludedAll = false;

	if (object != this && object->visible)
	{
		object->appendMatrix(this);

		object->cullingInCamera(this, 63);

		object->concatenatedAlpha = object->alpha;
		object->concatenatedBlendMode = object->blendMode;
		object->concatenatedColorTransform = object->colorTransform;

		object->draw(this);
	}


	for (int i = 0; i < this->opaqueCount; i++)
	{
		Material *material = this->opaqueMaterials[i];

		DrawParams p;
		p.vulkanWindow = vulkanWindow;
		p.camera = this;
		p.vertexBuffer = this->opaqueVertexBuffers[i];
		p.indexBuffer = this->opaqueIndexBuffers[i];
		p.firstIndex =  this->opaqueFirstIndexes[i];
		p.numTriangles = this->opaqueNumsTriangles[i];
		p.object =  this->opaqueObjects[i];
		p.vulkanUniform = vulkanUniform;

		material->drawOpaqueVulkan(&p);
	}



	int i = this->transparentCount - 1;

	while (i >= 0)
	{
		//if ((_local_1 + 1) == this->transparentOpaqueCount)
		//{
			//this->device.setDepthTest(false, Context3DCompareMode.LESS);
		//}

		Shared<Face> face = this->transparentFaceLists[i];
		Object3D *object = this->transparentObjects[i];

		if (object->concatenatedBlendMode != "normal")
		{
			//this->device.setBlendFactors(Context3DBlendFactor.SOURCE_ALPHA, Context3DBlendFactor.ONE);
		}
		else
		{
			//this->device.setBlendFactors(Context3DBlendFactor.SOURCE_ALPHA, Context3DBlendFactor.ONE_MINUS_SOURCE_ALPHA);
		}


		bool _local_18 = Object3D_isTypeFind<Sprite3D>(object);

		if (_local_18)
		{
			int distance = 0;

			Sprite3D *sprite = (Sprite3D*)object;

			Shared<Face> face_temp = face;

			while (face_temp->processNext != nullptr)
			{
				face_temp->distance = distance;
				face_temp = face_temp->processNext;
			}

			face_temp->distance = distance;

			if (this->transparentBatchObjects.length() <= distance)
				this->transparentBatchObjects.resize(distance + 10);

			this->transparentBatchObjects[distance] = object;

			distance++;

			int j = (i - 1);

			while (j >= 0)
			{
				Shared<Face> face_2 = this->transparentFaceLists[j];

				if (face->material != face_2->material)
					break;

				Object3D *batchObject = this->transparentObjects[j];
#if 1
				if (Object3D_isTypeFind<Sprite3D>(batchObject))
				{

					Sprite3D *_local_25 = (Sprite3D*)(batchObject);

					if ((((((((((!(sprite->useLight == _local_25->useLight)) ||
								(!(sprite->useShadowMap == _local_25->useShadowMap))) ||
							   (sprite->lighted)) ||
							  (_local_25->lighted)) ||
							 (!(sprite->softAttenuation == _local_25->softAttenuation))) ||
							(!(sprite->concatenatedAlpha == _local_25->concatenatedAlpha))) ||
						   (!(sprite->concatenatedColorTransform == nullptr))) ||
						  (!(_local_25->concatenatedColorTransform == nullptr))) ||
						 (!(sprite->concatenatedBlendMode == _local_25->concatenatedBlendMode))))
					{

						break;
					}
					break;
				}
				else
				{

					break;
				}
#endif
				face_temp->processNext = face_2;
				face_temp = face_2;

				while (face_temp->processNext != nullptr)
				{
					face_temp->distance = distance;
					face_temp = face_temp->processNext;
				}

				face_temp->distance = distance;

				if (this->transparentBatchObjects.length() <= distance)
					this->transparentBatchObjects.resize(distance + 10);

				this->transparentBatchObjects[distance] = batchObject;

				distance++;
				i--;
				j--;
			}
		}



		bool _local_19 = _local_18 && !((Sprite3D*)object)->depthTest;

		if (_local_19)
		{
			//this->device.setDepthTest(false, Context3DCompareMode.ALWAYS);
		}

		this->drawTransparentList(vulkanWindow, face, object, _local_18);

		if (_local_19)
		{
			//this->device.setDepthTest(false, Context3DCompareMode.LESS);
		}

		i--;
	}


	{
		int i = this->decalsCount - 1;
		while (i >= 0)
		{
			Decal *decal = this->decals[i];

#if 0
			if (_local_11.concatenatedBlendMode != "normal")
			{
				this.device.setBlendFactors(Context3DBlendFactor.SOURCE_ALPHA, Context3DBlendFactor.ONE);
			}
			else
			{
				this.device.setBlendFactors(Context3DBlendFactor.SOURCE_ALPHA, Context3DBlendFactor.ONE_MINUS_SOURCE_ALPHA);
			}
#endif

			DrawParams p;
			p.vulkanWindow = vulkanWindow;
			p.camera = this;
			p.vertexBuffer = decal->vertexBuffer;
			p.indexBuffer = decal->indexBuffer;
			p.firstIndex =  0;
			p.numTriangles = decal->numTriangles;
			p.object =  decal;
			p.decal = true;
			p.vulkanUniform = vulkanUniform;

			decal->faceList->material->drawOpaqueVulkan(&p);

			i--;
		}
	}


	this->decalsCount = 0;
	this->transparentCount = 0;

	deferredDestroy();
}


Shared<Face> Camera3D::cull(Shared<Face> _arg_1, int _arg_2)
{
	Shared<Face> _local_3 = nullptr;
	Shared<Face> _local_4 = nullptr;
	Shared<Face> _local_5;
	Shared<Vertex> _local_6;
	Shared<Vertex> _local_7;
	Shared<Vertex> _local_8;
	Shared<Wrapper> _local_9;
	Shared<Vertex> _local_10;
	Shared<Wrapper> _local_11;
	double _local_12 = MyMath::nan();
	double _local_13 = MyMath::nan();;
	double _local_14;
	double _local_15 = MyMath::nan();
	double _local_16 = MyMath::nan();
	double _local_17 = MyMath::nan();
	double _local_18 = MyMath::nan();
	double _local_19 = MyMath::nan();
	double _local_20;
	bool _local_21 = ((_arg_2 & 0x01) > 0);
	bool _local_22 = ((_arg_2 & 0x02) > 0);
	bool _local_23 = ((_arg_2 & 0x04) > 0);
	bool _local_24 = ((_arg_2 & 0x08) > 0);
	bool _local_25 = ((_arg_2 & 0x10) > 0);
	bool _local_26 = ((_arg_2 & 0x20) > 0);
	double _local_27 = this->nearClipping;
	double _local_28 = this->farClipping;
	bool _local_29 = ((_local_23) || (_local_24));
	bool _local_30 = ((_local_25) || (_local_26));
	Shared<Face> _local_31 = _arg_1;

	for (;_local_31 != nullptr;(_local_31 = _local_5))
	{
		_local_5 = _local_31->processNext;
		_local_9 = _local_31->wrapper;
		_local_6 = _local_9->vertex;
		_local_9 = _local_9->next;
		_local_7 = _local_9->vertex;
		_local_9 = _local_9->next;
		_local_8 = _local_9->vertex;
		_local_9 = _local_9->next;
		if (_local_29)
		{
			_local_12 = _local_6->cameraX;
			_local_15 = _local_7->cameraX;
			_local_18 = _local_8->cameraX;
		}
		if (_local_30)
		{
			_local_13 = _local_6->cameraY;
			_local_16 = _local_7->cameraY;
			_local_19 = _local_8->cameraY;
		}
		_local_14 = _local_6->cameraZ;
		_local_17 = _local_7->cameraZ;
		_local_20 = _local_8->cameraZ;
		if (_local_21)
		{
			if ((((_local_14 <= _local_27) || (_local_17 <= _local_27)) || (_local_20 <= _local_27)))
			{
				_local_31->processNext = nullptr;
				continue;
			}
			_local_11 = _local_9;
			while (_local_11 != nullptr)
			{
				if (_local_11->vertex->cameraZ <= _local_27) break;
				_local_11 = _local_11->next;
			}
			if (_local_11 != nullptr)
			{
				_local_31->processNext = nullptr;
				continue;
			}
		}
		if (((((_local_22) && (_local_14 >= _local_28)) && (_local_17 >= _local_28)) && (_local_20 >= _local_28)))
		{
			_local_11 = _local_9;
			while (_local_11 != nullptr)
			{
				if (_local_11->vertex->cameraZ < _local_28) break;
				_local_11 = _local_11->next;
			}
			if (_local_11 == nullptr)
			{
				_local_31->processNext = nullptr;
				continue;
			}
		}
		if (((((_local_23) && (_local_14 <= -(_local_12))) && (_local_17 <= -(_local_15))) && (_local_20 <= -(_local_18))))
		{
			_local_11 = _local_9;
			while (_local_11 != nullptr)
			{
				_local_10 = _local_11->vertex;
				if (-(_local_10->cameraX) < _local_10->cameraZ) break;
				_local_11 = _local_11->next;
			}
			if (_local_11 == nullptr)
			{
				_local_31->processNext = nullptr;
				continue;
			}
		}
		if (((((_local_24) && (_local_14 <= _local_12)) && (_local_17 <= _local_15)) && (_local_20 <= _local_18)))
		{
			_local_11 = _local_9;
			while (_local_11 != nullptr)
			{
				_local_10 = _local_11->vertex;
				if (_local_10->cameraX < _local_10->cameraZ) break;
				_local_11 = _local_11->next;
			}
			if (_local_11 == nullptr)
			{
				_local_31->processNext = nullptr;
				continue;
			}
		}
		if (((((_local_25) && (_local_14 <= -(_local_13))) && (_local_17 <= -(_local_16))) && (_local_20 <= -(_local_19))))
		{
			_local_11 = _local_9;
			while (_local_11 != nullptr)
			{
				_local_10 = _local_11->vertex;
				if (-(_local_10->cameraY) < _local_10->cameraZ) break;
				_local_11 = _local_11->next;
			}
			if (_local_11 == nullptr)
			{
				_local_31->processNext = nullptr;
				continue;
			}
		}
		if (((((_local_26) && (_local_14 <= _local_13)) && (_local_17 <= _local_16)) && (_local_20 <= _local_19)))
		{
			_local_11 = _local_9;
			while (_local_11 != nullptr)
			{
				_local_10 = _local_11->vertex;
				if (_local_10->cameraY < _local_10->cameraZ) break;
				_local_11 = _local_11->next;
			}
			if (_local_11 == nullptr)
			{
				_local_31->processNext = nullptr;
				continue;
			}
		}
		if (_local_3 != nullptr)
		{
			_local_4->processNext = _local_31;
		}
		else
		{
			_local_3 = _local_31;
		}
		_local_4 = _local_31;
	}

	if (_local_4 != nullptr)
	{
		_local_4->processNext = nullptr;
	}

	return _local_3;
}


Shared<Face> Camera3D::clip(Shared<Face> _arg_1, int _arg_2)
{
	Shared<Face> _local_3 = nullptr;
	Shared<Face> _local_4 = nullptr;
	Shared<Face> _local_5;
	Shared<Vertex> _local_6;
	Shared<Vertex> _local_7;
	Shared<Vertex> _local_8;
	Shared<Wrapper>  _local_9;
	Shared<Vertex> _local_10;
	Shared<Wrapper> _local_11;
	Shared<Wrapper> _local_12;
	Shared<Wrapper> _local_13;
	Shared<Wrapper> _local_14;
	Shared<Wrapper> _local_15;
	double _local_16 = MyMath::nan();
	double _local_17 = MyMath::nan();
	double _local_18;
	double _local_19 = MyMath::nan();
	double _local_20 = MyMath::nan();
	double _local_21;
	double _local_22 = MyMath::nan();
	double _local_23 = MyMath::nan();
	double _local_24 = MyMath::nan();
	bool _local_25;
	bool _local_26 = false;
	bool _local_27;
	bool _local_28;
	bool _local_29;
	bool _local_30;
	double _local_31;
	double _local_32;
	bool _local_33;
	bool _local_34;
	int _local_35;
	double _local_36;
	Shared<Face> _local_37;
	bool _local_38;
	Shared<Face> _local_39;

	_local_25 = ((_arg_2 & 0x01) > 0);
	_local_26 = ((_arg_2 & 0x02) > 0);
	_local_27 = ((_arg_2 & 0x04) > 0);
	_local_28 = ((_arg_2 & 0x08) > 0);
	_local_29 = ((_arg_2 & 0x10) > 0);
	_local_30 = ((_arg_2 & 0x20) > 0);
	_local_31 = this->nearClipping;
	_local_32 = this->farClipping;
	_local_33 = ((_local_27) || (_local_28));
	_local_34 = ((_local_29) || (_local_30));
	_local_37 = _arg_1;
	for (;_local_37 != nullptr;(_local_37 = _local_5))
	{
		_local_5 = _local_37->processNext;
		_local_9 = _local_37->wrapper;
		_local_6 = _local_9->vertex;
		_local_9 = _local_9->next;
		_local_7 = _local_9->vertex;
		_local_9 = _local_9->next;
		_local_8 = _local_9->vertex;
		_local_9 = _local_9->next;
		if (_local_33)
		{
			_local_16 = _local_6->cameraX;
			_local_19 = _local_7->cameraX;
			_local_22 = _local_8->cameraX;
		}
		if (_local_34)
		{
			_local_17 = _local_6->cameraY;
			_local_20 = _local_7->cameraY;
			_local_23 = _local_8->cameraY;
		}
		_local_18 = _local_6->cameraZ;
		_local_21 = _local_7->cameraZ;
		_local_24 = _local_8->cameraZ;
		_local_35 = 0;
		if (_local_25)
		{
			if ((((_local_18 <= _local_31) && (_local_21 <= _local_31)) && (_local_24 <= _local_31)))
			{
				_local_11 = _local_9;
				while (_local_11 != nullptr)
				{
					if (_local_11->vertex->cameraZ > _local_31)
					{
						_local_35 = (_local_35 | 0x01);
						break;
					}
					_local_11 = _local_11->next;
				}
				if (_local_11 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			else
			{
				if ((((_local_18 > _local_31) && (_local_21 > _local_31)) && (_local_24 > _local_31)))
				{
					_local_11 = _local_9;
					while (_local_11 != nullptr)
					{
						if (_local_11->vertex->cameraZ <= _local_31)
						{
							_local_35 = (_local_35 | 0x01);
							break;
						}
						_local_11 = _local_11->next;
					}
				}
				else
				{
					_local_35 = (_local_35 | 0x01);
				}
			}
		}
		if (_local_26)
		{
			if ((((_local_18 >= _local_32) && (_local_21 >= _local_32)) && (_local_24 >= _local_32)))
			{
				_local_11 = _local_9;
				while (_local_11 != nullptr)
				{
					if (_local_11->vertex->cameraZ < _local_32)
					{
						_local_35 = (_local_35 | 0x02);
						break;
					}
					_local_11 = _local_11->next;
				}
				if (_local_11 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			else
			{
				if ((((_local_18 < _local_32) && (_local_21 < _local_32)) && (_local_24 < _local_32)))
				{
					_local_11 = _local_9;
					while (_local_11 != nullptr)
					{
						if (_local_11->vertex->cameraZ >= _local_32)
						{
							_local_35 = (_local_35 | 0x02);
							break;
						}
						_local_11 = _local_11->next;
					}
				}
				else
				{
					_local_35 = (_local_35 | 0x02);
				}
			}
		}
		if (_local_27)
		{
			if ((((_local_18 <= -(_local_16)) && (_local_21 <= -(_local_19))) && (_local_24 <= -(_local_22))))
			{
				_local_11 = _local_9;
				while (_local_11 != nullptr)
				{
					_local_10 = _local_11->vertex;
					if (-(_local_10->cameraX) < _local_10->cameraZ)
					{
						_local_35 = (_local_35 | 0x04);
						break;
					}
					_local_11 = _local_11->next;
				}
				if (_local_11 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			else
			{
				if ((((_local_18 > -(_local_16)) && (_local_21 > -(_local_19))) && (_local_24 > -(_local_22))))
				{
					_local_11 = _local_9;
					while (_local_11 != nullptr)
					{
						_local_10 = _local_11->vertex;
						if (-(_local_10->cameraX) >= _local_10->cameraZ)
						{
							_local_35 = (_local_35 | 0x04);
							break;
						}
						_local_11 = _local_11->next;
					}
				}
				else
				{
					_local_35 = (_local_35 | 0x04);
				}
			}
		}
		if (_local_28)
		{
			if ((((_local_18 <= _local_16) && (_local_21 <= _local_19)) && (_local_24 <= _local_22)))
			{
				_local_11 = _local_9;
				while (_local_11 != nullptr)
				{
					_local_10 = _local_11->vertex;
					if (_local_10->cameraX < _local_10->cameraZ)
					{
						_local_35 = (_local_35 | 0x08);
						break;
					}
					_local_11 = _local_11->next;
				}
				if (_local_11 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			else
			{
				if ((((_local_18 > _local_16) && (_local_21 > _local_19)) && (_local_24 > _local_22)))
				{
					_local_11 = _local_9;
					while (_local_11 != nullptr)
					{
						_local_10 = _local_11->vertex;
						if (_local_10->cameraX >= _local_10->cameraZ)
						{
							_local_35 = (_local_35 | 0x08);
							break;
						}
						_local_11 = _local_11->next;
					}
				}
				else
				{
					_local_35 = (_local_35 | 0x08);
				}
			}
		}
		if (_local_29)
		{
			if ((((_local_18 <= -(_local_17)) && (_local_21 <= -(_local_20))) && (_local_24 <= -(_local_23))))
			{
				_local_11 = _local_9;
				while (_local_11 != nullptr)
				{
					_local_10 = _local_11->vertex;
					if (-(_local_10->cameraY) < _local_10->cameraZ)
					{
						_local_35 = (_local_35 | 0x10);
						break;
					}
					_local_11 = _local_11->next;
				}
				if (_local_11 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			else
			{
				if ((((_local_18 > -(_local_17)) && (_local_21 > -(_local_20))) && (_local_24 > -(_local_23))))
				{
					_local_11 = _local_9;
					while (_local_11 != nullptr)
					{
						_local_10 = _local_11->vertex;
						if (-(_local_10->cameraY) >= _local_10->cameraZ)
						{
							_local_35 = (_local_35 | 0x10);
							break;
						}
						_local_11 = _local_11->next;
					}
				}
				else
				{
					_local_35 = (_local_35 | 0x10);
				}
			}
		}
		if (_local_30)
		{
			if ((((_local_18 <= _local_17) && (_local_21 <= _local_20)) && (_local_24 <= _local_23)))
			{
				_local_11 = _local_9;
				while (_local_11 != nullptr)
				{
					_local_10 = _local_11->vertex;
					if (_local_10->cameraY < _local_10->cameraZ)
					{
						_local_35 = (_local_35 | 0x20);
						break;
					}
					_local_11 = _local_11->next;
				}
				if (_local_11 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			else
			{
				if ((((_local_18 > _local_17) && (_local_21 > _local_20)) && (_local_24 > _local_23)))
				{
					_local_11 = _local_9;
					while (_local_11 != nullptr)
					{
						_local_10 = _local_11->vertex;
						if (_local_10->cameraY >= _local_10->cameraZ)
						{
							_local_35 = (_local_35 | 0x20);
							break;
						}
						_local_11 = _local_11->next;
					}
				}
				else
				{
					_local_35 = (_local_35 | 0x20);
				}
			}
		}
		if (_local_35 > 0)
		{
			_local_38 = ((!(_local_37->material == nullptr)) && (_local_37->material->useVerticesNormals));
			_local_12 = nullptr;
			_local_13 = nullptr;
			_local_11 = _local_37->wrapper;
			while (_local_11 != nullptr)
			{
				_local_15 = _local_11->create();
				_local_15->vertex = _local_11->vertex;
				if (_local_12 != nullptr)
				{
					_local_13->next = _local_15;
				}
				else
				{
					_local_12 = _local_15;
				}
				_local_13 = _local_15;
				_local_11 = _local_11->next;
			}
			if ((_local_35 & 0x01))
			{
				_local_6 = _local_13->vertex;
				_local_18 = _local_6->cameraZ;
				_local_11 = _local_12;
				_local_12 = nullptr;
				_local_13 = nullptr;
				while (_local_11 != nullptr)
				{
					_local_14 = _local_11->next;
					_local_7 = _local_11->vertex;
					_local_21 = _local_7->cameraZ;
					if ((((_local_21 > _local_31) && (_local_18 <= _local_31)) || ((_local_21 <= _local_31) && (_local_18 > _local_31))))
					{
						_local_36 = ((_local_31 - _local_18) / (_local_21 - _local_18));
						_local_10 = _local_7->create();
						this->lastVertex->next = _local_10;
						this->lastVertex = _local_10;
						_local_10->cameraX = (_local_6->cameraX + ((_local_7->cameraX - _local_6->cameraX) * _local_36));
						_local_10->cameraY = (_local_6->cameraY + ((_local_7->cameraY - _local_6->cameraY) * _local_36));
						_local_10->cameraZ = (_local_18 + ((_local_21 - _local_18) * _local_36));
						_local_10->x = (_local_6->x + ((_local_7->x - _local_6->x) * _local_36));
						_local_10->y = (_local_6->y + ((_local_7->y - _local_6->y) * _local_36));
						_local_10->z = (_local_6->z + ((_local_7->z - _local_6->z) * _local_36));
						_local_10->u = (_local_6->u + ((_local_7->u - _local_6->u) * _local_36));
						_local_10->v = (_local_6->v + ((_local_7->v - _local_6->v) * _local_36));
						if (_local_38)
						{
							_local_10->normalX = (_local_6->normalX + ((_local_7->normalX - _local_6->normalX) * _local_36));
							_local_10->normalY = (_local_6->normalY + ((_local_7->normalY - _local_6->normalY) * _local_36));
							_local_10->normalZ = (_local_6->normalZ + ((_local_7->normalZ - _local_6->normalZ) * _local_36));
						}
						_local_15 = _local_11->create();
						_local_15->vertex = _local_10;
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_15;
						}
						else
						{
							_local_12 = _local_15;
						}
						_local_13 = _local_15;
					}
					if (_local_21 > _local_31)
					{
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_11;
						}
						else
						{
							_local_12 = _local_11;
						}
						_local_13 = _local_11;
						_local_11->next = nullptr;
					}
					else
					{
						_local_11->vertex = nullptr;
						_local_11 = nullptr;
						//_local_11->next = Wrapper::collector;
						//Wrapper::collector = _local_11;
					}
					_local_6 = _local_7;
					_local_18 = _local_21;
					_local_11 = _local_14;
				}
				if (_local_12 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			if ((_local_35 & 0x02))
			{
				_local_6 = _local_13->vertex;
				_local_18 = _local_6->cameraZ;
				_local_11 = _local_12;
				_local_12 = nullptr;
				_local_13 = nullptr;
				while (_local_11 != nullptr)
				{
					_local_14 = _local_11->next;
					_local_7 = _local_11->vertex;
					_local_21 = _local_7->cameraZ;
					if ((((_local_21 < _local_32) && (_local_18 >= _local_32)) || ((_local_21 >= _local_32) && (_local_18 < _local_32))))
					{
						_local_36 = ((_local_32 - _local_18) / (_local_21 - _local_18));
						_local_10 = _local_7->create();
						this->lastVertex->next = _local_10;
						this->lastVertex = _local_10;
						_local_10->cameraX = (_local_6->cameraX + ((_local_7->cameraX - _local_6->cameraX) * _local_36));
						_local_10->cameraY = (_local_6->cameraY + ((_local_7->cameraY - _local_6->cameraY) * _local_36));
						_local_10->cameraZ = (_local_18 + ((_local_21 - _local_18) * _local_36));
						_local_10->x = (_local_6->x + ((_local_7->x - _local_6->x) * _local_36));
						_local_10->y = (_local_6->y + ((_local_7->y - _local_6->y) * _local_36));
						_local_10->z = (_local_6->z + ((_local_7->z - _local_6->z) * _local_36));
						_local_10->u = (_local_6->u + ((_local_7->u - _local_6->u) * _local_36));
						_local_10->v = (_local_6->v + ((_local_7->v - _local_6->v) * _local_36));
						if (_local_38)
						{
							_local_10->normalX = (_local_6->normalX + ((_local_7->normalX - _local_6->normalX) * _local_36));
							_local_10->normalY = (_local_6->normalY + ((_local_7->normalY - _local_6->normalY) * _local_36));
							_local_10->normalZ = (_local_6->normalZ + ((_local_7->normalZ - _local_6->normalZ) * _local_36));
						}
						_local_15 = _local_11->create();
						_local_15->vertex = _local_10;
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_15;
						}
						else
						{
							_local_12 = _local_15;
						}
						_local_13 = _local_15;
					}
					if (_local_21 < _local_32)
					{
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_11;
						}
						else
						{
							_local_12 = _local_11;
						}
						_local_13 = _local_11;
						_local_11->next = nullptr;
					}
					else
					{
						_local_11->vertex = nullptr;
						_local_11 = nullptr;
						//_local_11->next = Wrapper::collector;
						//Wrapper::collector = _local_11;
					}
					_local_6 = _local_7;
					_local_18 = _local_21;
					_local_11 = _local_14;
				}
				if (_local_12 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			if ((_local_35 & 0x04))
			{
				_local_6 = _local_13->vertex;
				_local_16 = _local_6->cameraX;
				_local_18 = _local_6->cameraZ;
				_local_11 = _local_12;
				_local_12 = nullptr;
				_local_13 = nullptr;
				while (_local_11 != nullptr)
				{
					_local_14 = _local_11->next;
					_local_7 = _local_11->vertex;
					_local_19 = _local_7->cameraX;
					_local_21 = _local_7->cameraZ;
					if ((((_local_21 > -(_local_19)) && (_local_18 <= -(_local_16))) || ((_local_21 <= -(_local_19)) && (_local_18 > -(_local_16)))))
					{
						_local_36 = ((_local_16 + _local_18) / (((_local_16 + _local_18) - _local_19) - _local_21));
						_local_10 = _local_7->create();
						this->lastVertex->next = _local_10;
						this->lastVertex = _local_10;
						_local_10->cameraX = (_local_16 + ((_local_19 - _local_16) * _local_36));
						_local_10->cameraY = (_local_6->cameraY + ((_local_7->cameraY - _local_6->cameraY) * _local_36));
						_local_10->cameraZ = (_local_18 + ((_local_21 - _local_18) * _local_36));
						_local_10->x = (_local_6->x + ((_local_7->x - _local_6->x) * _local_36));
						_local_10->y = (_local_6->y + ((_local_7->y - _local_6->y) * _local_36));
						_local_10->z = (_local_6->z + ((_local_7->z - _local_6->z) * _local_36));
						_local_10->u = (_local_6->u + ((_local_7->u - _local_6->u) * _local_36));
						_local_10->v = (_local_6->v + ((_local_7->v - _local_6->v) * _local_36));
						if (_local_38)
						{
							_local_10->normalX = (_local_6->normalX + ((_local_7->normalX - _local_6->normalX) * _local_36));
							_local_10->normalY = (_local_6->normalY + ((_local_7->normalY - _local_6->normalY) * _local_36));
							_local_10->normalZ = (_local_6->normalZ + ((_local_7->normalZ - _local_6->normalZ) * _local_36));
						}
						_local_15 = _local_11->create();
						_local_15->vertex = _local_10;
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_15;
						}
						else
						{
							_local_12 = _local_15;
						}
						_local_13 = _local_15;
					}
					if (_local_21 > -(_local_19))
					{
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_11;
						}
						else
						{
							_local_12 = _local_11;
						}
						_local_13 = _local_11;
						_local_11->next = nullptr;
					}
					else
					{
						_local_11->vertex = nullptr;
						_local_11 = nullptr;
						//_local_11->next = Wrapper::collector;
						//Wrapper::collector = _local_11;
					}
					_local_6 = _local_7;
					_local_16 = _local_19;
					_local_18 = _local_21;
					_local_11 = _local_14;
				}
				if (_local_12 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			if ((_local_35 & 0x08))
			{
				_local_6 = _local_13->vertex;
				_local_16 = _local_6->cameraX;
				_local_18 = _local_6->cameraZ;
				_local_11 = _local_12;
				_local_12 = nullptr;
				_local_13 = nullptr;
				while (_local_11 != nullptr)
				{
					_local_14 = _local_11->next;
					_local_7 = _local_11->vertex;
					_local_19 = _local_7->cameraX;
					_local_21 = _local_7->cameraZ;
					if ((((_local_21 > _local_19) && (_local_18 <= _local_16)) || ((_local_21 <= _local_19) && (_local_18 > _local_16))))
					{
						_local_36 = ((_local_18 - _local_16) / (((_local_18 - _local_16) + _local_19) - _local_21));
						_local_10 = _local_7->create();
						this->lastVertex->next = _local_10;
						this->lastVertex = _local_10;
						_local_10->cameraX = (_local_16 + ((_local_19 - _local_16) * _local_36));
						_local_10->cameraY = (_local_6->cameraY + ((_local_7->cameraY - _local_6->cameraY) * _local_36));
						_local_10->cameraZ = (_local_18 + ((_local_21 - _local_18) * _local_36));
						_local_10->x = (_local_6->x + ((_local_7->x - _local_6->x) * _local_36));
						_local_10->y = (_local_6->y + ((_local_7->y - _local_6->y) * _local_36));
						_local_10->z = (_local_6->z + ((_local_7->z - _local_6->z) * _local_36));
						_local_10->u = (_local_6->u + ((_local_7->u - _local_6->u) * _local_36));
						_local_10->v = (_local_6->v + ((_local_7->v - _local_6->v) * _local_36));
						if (_local_38)
						{
							_local_10->normalX = (_local_6->normalX + ((_local_7->normalX - _local_6->normalX) * _local_36));
							_local_10->normalY = (_local_6->normalY + ((_local_7->normalY - _local_6->normalY) * _local_36));
							_local_10->normalZ = (_local_6->normalZ + ((_local_7->normalZ - _local_6->normalZ) * _local_36));
						}
						_local_15 = _local_11->create();
						_local_15->vertex = _local_10;
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_15;
						}
						else
						{
							_local_12 = _local_15;
						}
						_local_13 = _local_15;
					}
					if (_local_21 > _local_19)
					{
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_11;
						}
						else
						{
							_local_12 = _local_11;
						}
						_local_13 = _local_11;
						_local_11->next = nullptr;
					}
					else
					{
						_local_11->vertex = nullptr;
						_local_11 = nullptr;
						//_local_11->next = Wrapper::collector;
						//Wrapper::collector = _local_11;
					}
					_local_6 = _local_7;
					_local_16 = _local_19;
					_local_18 = _local_21;
					_local_11 = _local_14;
				}
				if (_local_12 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			if ((_local_35 & 0x10))
			{
				_local_6 = _local_13->vertex;
				_local_17 = _local_6->cameraY;
				_local_18 = _local_6->cameraZ;
				_local_11 = _local_12;
				_local_12 = nullptr;
				_local_13 = nullptr;
				while (_local_11 != nullptr)
				{
					_local_14 = _local_11->next;
					_local_7 = _local_11->vertex;
					_local_20 = _local_7->cameraY;
					_local_21 = _local_7->cameraZ;
					if ((((_local_21 > -(_local_20)) && (_local_18 <= -(_local_17))) || ((_local_21 <= -(_local_20)) && (_local_18 > -(_local_17)))))
					{
						_local_36 = ((_local_17 + _local_18) / (((_local_17 + _local_18) - _local_20) - _local_21));
						_local_10 = _local_7->create();
						this->lastVertex->next = _local_10;
						this->lastVertex = _local_10;
						_local_10->cameraX = (_local_6->cameraX + ((_local_7->cameraX - _local_6->cameraX) * _local_36));
						_local_10->cameraY = (_local_17 + ((_local_20 - _local_17) * _local_36));
						_local_10->cameraZ = (_local_18 + ((_local_21 - _local_18) * _local_36));
						_local_10->x = (_local_6->x + ((_local_7->x - _local_6->x) * _local_36));
						_local_10->y = (_local_6->y + ((_local_7->y - _local_6->y) * _local_36));
						_local_10->z = (_local_6->z + ((_local_7->z - _local_6->z) * _local_36));
						_local_10->u = (_local_6->u + ((_local_7->u - _local_6->u) * _local_36));
						_local_10->v = (_local_6->v + ((_local_7->v - _local_6->v) * _local_36));
						if (_local_38)
						{
							_local_10->normalX = (_local_6->normalX + ((_local_7->normalX - _local_6->normalX) * _local_36));
							_local_10->normalY = (_local_6->normalY + ((_local_7->normalY - _local_6->normalY) * _local_36));
							_local_10->normalZ = (_local_6->normalZ + ((_local_7->normalZ - _local_6->normalZ) * _local_36));
						}
						_local_15 = _local_11->create();
						_local_15->vertex = _local_10;
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_15;
						}
						else
						{
							_local_12 = _local_15;
						}
						_local_13 = _local_15;
					}
					if (_local_21 > -(_local_20))
					{
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_11;
						}
						else
						{
							_local_12 = _local_11;
						}
						_local_13 = _local_11;
						_local_11->next = nullptr;
					}
					else
					{
						_local_11->vertex = nullptr;
						_local_11 = nullptr;
						//_local_11->next = Wrapper::collector;
						//Wrapper::collector = _local_11;
					}
					_local_6 = _local_7;
					_local_17 = _local_20;
					_local_18 = _local_21;
					_local_11 = _local_14;
				}
				if (_local_12 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			if ((_local_35 & 0x20))
			{
				_local_6 = _local_13->vertex;
				_local_17 = _local_6->cameraY;
				_local_18 = _local_6->cameraZ;
				_local_11 = _local_12;
				_local_12 = nullptr;
				_local_13 = nullptr;
				while (_local_11 != nullptr)
				{
					_local_14 = _local_11->next;
					_local_7 = _local_11->vertex;
					_local_20 = _local_7->cameraY;
					_local_21 = _local_7->cameraZ;
					if ((((_local_21 > _local_20) && (_local_18 <= _local_17)) || ((_local_21 <= _local_20) && (_local_18 > _local_17))))
					{
						_local_36 = ((_local_18 - _local_17) / (((_local_18 - _local_17) + _local_20) - _local_21));
						_local_10 = _local_7->create();
						this->lastVertex->next = _local_10;
						this->lastVertex = _local_10;
						_local_10->cameraX = (_local_6->cameraX + ((_local_7->cameraX - _local_6->cameraX) * _local_36));
						_local_10->cameraY = (_local_17 + ((_local_20 - _local_17) * _local_36));
						_local_10->cameraZ = (_local_18 + ((_local_21 - _local_18) * _local_36));
						_local_10->x = (_local_6->x + ((_local_7->x - _local_6->x) * _local_36));
						_local_10->y = (_local_6->y + ((_local_7->y - _local_6->y) * _local_36));
						_local_10->z = (_local_6->z + ((_local_7->z - _local_6->z) * _local_36));
						_local_10->u = (_local_6->u + ((_local_7->u - _local_6->u) * _local_36));
						_local_10->v = (_local_6->v + ((_local_7->v - _local_6->v) * _local_36));
						if (_local_38)
						{
							_local_10->normalX = (_local_6->normalX + ((_local_7->normalX - _local_6->normalX) * _local_36));
							_local_10->normalY = (_local_6->normalY + ((_local_7->normalY - _local_6->normalY) * _local_36));
							_local_10->normalZ = (_local_6->normalZ + ((_local_7->normalZ - _local_6->normalZ) * _local_36));
						}
						_local_15 = _local_11->create();
						_local_15->vertex = _local_10;
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_15;
						}
						else
						{
							_local_12 = _local_15;
						}
						_local_13 = _local_15;
					}
					if (_local_21 > _local_20)
					{
						if (_local_12 != nullptr)
						{
							_local_13->next = _local_11;
						}
						else
						{
							_local_12 = _local_11;
						}
						_local_13 = _local_11;
						_local_11->next = nullptr;
					}
					else
					{
						_local_11->vertex = nullptr;
						_local_11 = nullptr;
						//_local_11->next = Wrapper::collector;
						//Wrapper::collector = _local_11;
					}
					_local_6 = _local_7;
					_local_17 = _local_20;
					_local_18 = _local_21;
					_local_11 = _local_14;
				}
				if (_local_12 == nullptr)
				{
					_local_37->processNext = nullptr;
					continue;
				}
			}
			_local_37->processNext = nullptr;
			_local_39 = _local_37->create(5);
			_local_39->material = _local_37->material;
			this->lastFace->next = _local_39;
			this->lastFace = _local_39;
			_local_39->wrapper = _local_12;
			_local_37 = _local_39;
		}
		if (_local_3 != nullptr)
		{
			_local_4->processNext = _local_37;
		}
		else
		{
			_local_3 = _local_37;
		}
		_local_4 = _local_37;
	}
	if (_local_4 != nullptr)
	{
		_local_4->processNext = nullptr;
	}
	return _local_3;
}


Shared<Face> Camera3D::sortByAverageZ(Shared<Face> _arg_1)
{
	Shared<Face> _local_5 = _arg_1;
	Shared<Face> _local_6 = _arg_1->processNext;

	while (((!(_local_6 == nullptr)) && (!(_local_6->processNext == nullptr))))
	{
		_arg_1 = _arg_1->processNext;
		_local_6 = _local_6->processNext->processNext;
	}
	_local_6 = _arg_1->processNext;
	_arg_1->processNext = nullptr;

	if (_local_5->processNext != nullptr)
	{
		_local_5 = this->sortByAverageZ(_local_5);
	}
	else
	{
		int _local_2 = 0;
		double _local_3 = 0;
		Weak<Wrapper> _local_4 = _local_5->wrapper;
		while (_local_4 != nullptr)
		{
			_local_2++;
			_local_3 = (_local_3 + _local_4->vertex->cameraZ);
			_local_4 = _local_4->next;
		}
		_local_5->distance = (_local_3 / _local_2);
	}

	if (_local_6->processNext != nullptr)
	{
		_local_6 = this->sortByAverageZ(_local_6);
	}
	else
	{
		int _local_2 = 0;
		double _local_3 = 0;
		Weak<Wrapper> _local_4 = _local_6->wrapper;
		while (_local_4 != nullptr)
		{
			_local_2++;
			_local_3 = (_local_3 + _local_4->vertex->cameraZ);
			_local_4 = _local_4->next;
		}
		_local_6->distance = (_local_3 / _local_2);
	}

	bool _local_7 = (_local_5->distance > _local_6->distance);
	if (_local_7)
	{
		_arg_1 = _local_5;
		_local_5 = _local_5->processNext;
	}
	else
	{
		_arg_1 = _local_6;
		_local_6 = _local_6->processNext;
	}

	Shared<Face> _local_8 = _arg_1;
	while (true)
	{
		if (_local_5 == nullptr)
		{
			_local_8->processNext = _local_6;
			return (_arg_1);
		}
		if (_local_6 == nullptr)
		{
			_local_8->processNext = _local_5;
			return (_arg_1);
		}
		if (_local_7)
		{
			if (_local_5->distance > _local_6->distance)
			{
				_local_8 = _local_5;
				_local_5 = _local_5->processNext;
			}
			else
			{
				_local_8->processNext = _local_6;
				_local_8 = _local_6;
				_local_6 = _local_6->processNext;
				_local_7 = false;
			}
		}
		else
		{
			if (_local_6->distance > _local_5->distance)
			{
				_local_8 = _local_6;
				_local_6 = _local_6->processNext;
			}
			else
			{
				_local_8->processNext = _local_5;
				_local_8 = _local_5;
				_local_5 = _local_5->processNext;
				_local_7 = true;
			}
		}
	}

	return nullptr;
}


Shared<Face> Camera3D::sortByDynamicBSP(Shared<Face> _arg_1, double _arg_2, Shared<Face> _arg_3)
{
	Shared<Vertex> _local_8;
	Shared<Face> _local_23 = nullptr;
	Shared<Face> _local_24 = nullptr;
	Shared<Face> _local_26 = nullptr;
	Shared<Face> _local_27 = nullptr;
	Shared<Face> _local_28;
	double _local_30;
	double _local_31;
	double _local_32;
	double _local_33;
	double _local_34;
	double _local_35;
	double _local_36;
	double _local_37;
	double _local_38;
	double _local_39;
	bool _local_40;
	bool _local_41;
	double _local_42;
	Shared<Face> _local_43;
	Shared<Face> _local_44;
	Shared<Wrapper> _local_45;
	Shared<Wrapper> _local_46;
	Shared<Wrapper> _local_47;
	bool _local_48;
	double _local_49;

	Shared<Face> _local_9 = _arg_1;
	_arg_1 = _local_9->processNext;

	Shared<Wrapper> _local_4 = _local_9->wrapper;
	Shared<Vertex> _local_5 = _local_4->vertex;
	_local_4 = _local_4->next;
	Shared<Vertex> _local_6 = _local_4->vertex;
	double _local_10 = _local_5->cameraX;
	double _local_11 = _local_5->cameraY;
	double _local_12 = _local_5->cameraZ;
	double _local_13 = (_local_6->cameraX - _local_10);
	double _local_14 = (_local_6->cameraY - _local_11);
	double _local_15 = (_local_6->cameraZ - _local_12);
	double _local_16 = 0;
	double _local_17 = 0;
	double _local_18 = 1;
	double _local_19 = _local_12;
	double _local_20 = 0;
	_local_4 = _local_4->next;

	while (_local_4 != nullptr)
	{
		_local_8 = _local_4->vertex;
		_local_30 = (_local_8->cameraX - _local_10);
		_local_31 = (_local_8->cameraY - _local_11);
		_local_32 = (_local_8->cameraZ - _local_12);
		_local_33 = ((_local_32 * _local_14) - (_local_31 * _local_15));
		_local_34 = ((_local_30 * _local_15) - (_local_32 * _local_13));
		_local_35 = ((_local_31 * _local_13) - (_local_30 * _local_14));
		_local_36 = (((_local_33 * _local_33) + (_local_34 * _local_34)) + (_local_35 * _local_35));
		if (_local_36 > _arg_2)
		{
			_local_36 = (1 / MyMath::sqrt(_local_36));
			_local_16 = (_local_33 * _local_36);
			_local_17 = (_local_34 * _local_36);
			_local_18 = (_local_35 * _local_36);
			_local_19 = (((_local_10 * _local_16) + (_local_11 * _local_17)) + (_local_12 * _local_18));
			break;
		}
		if (_local_36 > _local_20)
		{
			_local_36 = (1 / MyMath::sqrt(_local_36));
			_local_16 = (_local_33 * _local_36);
			_local_17 = (_local_34 * _local_36);
			_local_18 = (_local_35 * _local_36);
			_local_19 = (((_local_10 * _local_16) + (_local_11 * _local_17)) + (_local_12 * _local_18));
			_local_20 = _local_36;
		}
		_local_4 = _local_4->next;
	}

	double _local_21 = (_local_19 - _arg_2);
	double _local_22 = (_local_19 + _arg_2);
	Shared<Face> _local_25 = _local_9;
	Shared<Face> _local_29 = _arg_1;

	while (_local_29 != nullptr)
	{
		_local_28 = _local_29->processNext;
		_local_4 = _local_29->wrapper;
		_local_5 = _local_4->vertex;
		_local_4 = _local_4->next;
		_local_6 = _local_4->vertex;
		_local_4 = _local_4->next;
		Vertex *_local_7 = _local_4->vertex.get();
		_local_4 = _local_4->next;
		_local_37 = (((_local_5->cameraX * _local_16) + (_local_5->cameraY * _local_17)) + (_local_5->cameraZ * _local_18));
		_local_38 = (((_local_6->cameraX * _local_16) + (_local_6->cameraY * _local_17)) + (_local_6->cameraZ * _local_18));
		_local_39 = (((_local_7->cameraX * _local_16) + (_local_7->cameraY * _local_17)) + (_local_7->cameraZ * _local_18));
		_local_40 = (((_local_37 < _local_21) || (_local_38 < _local_21)) || (_local_39 < _local_21));
		_local_41 = (((_local_37 > _local_22) || (_local_38 > _local_22)) || (_local_39 > _local_22));
		while (_local_4 != nullptr)
		{
			_local_8 = _local_4->vertex;
			_local_42 = (((_local_8->cameraX * _local_16) + (_local_8->cameraY * _local_17)) + (_local_8->cameraZ * _local_18));
			if (_local_42 < _local_21)
			{
				_local_40 = true;
			}
			else
			{
				if (_local_42 > _local_22)
				{
					_local_41 = true;
				}
			}
			_local_8->offset = _local_42;
			_local_4 = _local_4->next;
		}
		if ((!(_local_40)))
		{
			if ((!(_local_41)))
			{
				_local_25->processNext = _local_29;
				_local_25 = _local_29;
			}
			else
			{
				if (_local_26 != nullptr)
				{
					_local_27->processNext = _local_29;
				}
				else
				{
					_local_26 = _local_29;
				}
				_local_27 = _local_29;
			}
		}
		else
		{
			if ((!(_local_41)))
			{
				if (_local_23 != nullptr)
				{
					_local_24->processNext = _local_29;
				}
				else
				{
					_local_23 = _local_29;
				}
				_local_24 = _local_29;
			}
			else
			{
				_local_5->offset = _local_37;
				_local_6->offset = _local_38;
				_local_7->offset = _local_39;
				_local_43 = _local_29->create(6);
				_local_43->material = _local_29->material;
				this->lastFace->next = _local_43;
				this->lastFace = _local_43;
				_local_44 = _local_29->create(7);
				_local_44->material = _local_29->material;
				this->lastFace->next = _local_44;
				this->lastFace = _local_44;
				_local_45 = nullptr;
				_local_46 = nullptr;
				_local_4 = _local_29->wrapper->next->next;
				while (_local_4->next != nullptr)
				{
					_local_4 = _local_4->next;
				}
				_local_5 = _local_4->vertex;
				_local_37 = _local_5->offset;
				_local_48 = ((!(_local_29->material == nullptr)) && (_local_29->material->useVerticesNormals));
				_local_4 = _local_29->wrapper;
				while (_local_4 != nullptr)
				{
					_local_6 = _local_4->vertex;
					_local_38 = _local_6->offset;
					if ((((_local_37 < _local_21) && (_local_38 > _local_22)) || ((_local_37 > _local_22) && (_local_38 < _local_21))))
					{
						_local_49 = ((_local_19 - _local_37) / (_local_38 - _local_37));
						_local_8 = _local_6->create();
						this->lastVertex->next = _local_8;
						this->lastVertex = _local_8;
						_local_8->cameraX = (_local_5->cameraX + ((_local_6->cameraX - _local_5->cameraX) * _local_49));
						_local_8->cameraY = (_local_5->cameraY + ((_local_6->cameraY - _local_5->cameraY) * _local_49));
						_local_8->cameraZ = (_local_5->cameraZ + ((_local_6->cameraZ - _local_5->cameraZ) * _local_49));
						_local_8->u = (_local_5->u + ((_local_6->u - _local_5->u) * _local_49));
						_local_8->v = (_local_5->v + ((_local_6->v - _local_5->v) * _local_49));
						if (_local_48)
						{
							_local_8->x = (_local_5->x + ((_local_6->x - _local_5->x) * _local_49));
							_local_8->y = (_local_5->y + ((_local_6->y - _local_5->y) * _local_49));
							_local_8->z = (_local_5->z + ((_local_6->z - _local_5->z) * _local_49));
							_local_8->normalX = (_local_5->normalX + ((_local_6->normalX - _local_5->normalX) * _local_49));
							_local_8->normalY = (_local_5->normalY + ((_local_6->normalY - _local_5->normalY) * _local_49));
							_local_8->normalZ = (_local_5->normalZ + ((_local_6->normalZ - _local_5->normalZ) * _local_49));
						}
						_local_47 = _local_4->create();
						_local_47->vertex = _local_8;
						if (_local_45 != nullptr)
						{
							_local_45->next = _local_47;
						}
						else
						{
							_local_43->wrapper = _local_47;
						}
						_local_45 = _local_47;
						_local_47 = _local_4->create();
						_local_47->vertex = _local_8;
						if (_local_46 != nullptr)
						{
							_local_46->next = _local_47;
						}
						else
						{
							_local_44->wrapper = _local_47;
						}
						_local_46 = _local_47;
					}
					if (_local_38 <= _local_22)
					{
						_local_47 = _local_4->create();
						_local_47->vertex = _local_6;
						if (_local_45 != nullptr)
						{
							_local_45->next = _local_47;
						}
						else
						{
							_local_43->wrapper = _local_47;
						}
						_local_45 = _local_47;
					}
					if (_local_38 >= _local_21)
					{
						_local_47 = _local_4->create();
						_local_47->vertex = _local_6;
						if (_local_46 != nullptr)
						{
							_local_46->next = _local_47;
						}
						else
						{
							_local_44->wrapper = _local_47;
						}
						_local_46 = _local_47;
					}
					_local_5 = _local_6;
					_local_37 = _local_38;
					_local_4 = _local_4->next;
				}
				if (_local_23 != nullptr)
				{
					_local_24->processNext = _local_43;
				}
				else
				{
					_local_23 = _local_43;
				}
				_local_24 = _local_43;
				if (_local_26 != nullptr)
				{
					_local_27->processNext = _local_44;
				}
				else
				{
					_local_26 = _local_44;
				}
				_local_27 = _local_44;
				_local_29->processNext = nullptr;
			}
		}
		_local_29 = _local_28;
	}

	if (_local_26 != nullptr)
	{
		_local_27->processNext = nullptr;
		if (_local_26->processNext != nullptr)
		{
			_arg_3 = this->sortByDynamicBSP(_local_26, _arg_2, _arg_3);
		}
		else
		{
			_local_26->processNext = _arg_3;
			_arg_3 = _local_26;
		}
	}

	_local_25->processNext = _arg_3;

	_arg_3 = _local_9;

	if (_local_23 != nullptr)
	{
		_local_24->processNext = nullptr;
		if (_local_23->processNext != nullptr)
		{
			_arg_3 = this->sortByDynamicBSP(_local_23, _arg_2, _arg_3);
		}
		else
		{
			_local_23->processNext = _arg_3;
			_arg_3 = _local_23;
		}
	}

	return _arg_3;
}


void Camera3D::addTransparent(Shared<Face> face, Object3D *object)
{
	if (this->transparentFaceLists.length() <= this->transparentCount)
		this->transparentFaceLists.resize(this->transparentCount + 10);

	if (this->transparentObjects.length() <= this->transparentCount)
		this->transparentObjects.resize(this->transparentCount + 10);

	this->transparentFaceLists[this->transparentCount] = face;
	this->transparentObjects[this->transparentCount] = object;
	this->transparentCount++;
}


void Camera3D::addOpaque(Material *material,
						 VertexBufferResource *vertex,
						 IndexBufferResource *index,
						 int firstIndex,
						 int numsTriangles,
						 Object3D *object)
{
	if (this->opaqueMaterials.length() <= this->opaqueCount)
		this->opaqueMaterials.resize(this->opaqueCount + 10);

	if (this->opaqueVertexBuffers.length() <= this->opaqueCount)
		this->opaqueVertexBuffers.resize(this->opaqueCount + 10);

	if (this->opaqueIndexBuffers.length() <= this->opaqueCount)
		this->opaqueIndexBuffers.resize(this->opaqueCount + 10);

	if (this->opaqueFirstIndexes.length() <= this->opaqueCount)
		this->opaqueFirstIndexes.resize(this->opaqueCount + 10);

	if (this->opaqueNumsTriangles.length() <= this->opaqueCount)
		this->opaqueNumsTriangles.resize(this->opaqueCount + 10);

	if (this->opaqueObjects.length() <= this->opaqueCount)
		this->opaqueObjects.resize(this->opaqueCount + 10);

	this->opaqueMaterials[this->opaqueCount] = material;
	this->opaqueVertexBuffers[this->opaqueCount] = vertex;
	this->opaqueIndexBuffers[this->opaqueCount] = index;
	this->opaqueFirstIndexes[this->opaqueCount] = firstIndex;
	this->opaqueNumsTriangles[this->opaqueCount] = numsTriangles;
	this->opaqueObjects[this->opaqueCount] = object;
	this->opaqueCount++;
}


void Camera3D::drawTransparentList(VulkanWindow *vulkanWindow, Shared<Face> face, Object3D *object, bool batch)
{
	VectexNode *node = vertexBufferList[this->vertexCurrentNumber];

	if (node == nullptr)
	{
		node = new VectexNode;
		this->vertexBufferList[this->vertexCurrentNumber] = node;
	}

	if (++this->vertexCurrentNumber == VERTEX_BUFFER_SIZE)
	{
		this->vertexCurrentNumber = 0;
	}


	int constants_count = 0;
	int triangles_count = 0;

	Material *material = face->material.get();

	if (material == nullptr)
		return;


	while (face != nullptr)
	{

		Shared<Face> face_next = face->processNext;

		face->processNext = nullptr;

		Weak<Wrapper> wrapper = face->wrapper;

		Vertex *vertex_1 = wrapper->vertex.get();

		wrapper = wrapper->next;

		Vertex *vertex_2 = wrapper->vertex.get();

		if (batch)
		{
			//Object3D *batchObject = this->transparentBatchObjects[face->distance];

			wrapper = wrapper->next;


			while (wrapper != nullptr)
			{
				if (constants.length() <= (constants_count + 24))
				{
					constants.resize(constants_count + 24);
				}


				Vertex *vertex_3 = wrapper->vertex.get();

				//
				constants[constants_count] = vertex_1->cameraX;
				constants_count++;
				constants[constants_count] = vertex_1->cameraY;
				constants_count++;
				constants[constants_count] = vertex_1->cameraZ;
				constants_count++;
				//constants[constants_count] = -batchObject->matrix4.md;
				//constants_count++;

				constants[constants_count] = vertex_1->u;
				constants_count++;
				constants[constants_count] = vertex_1->v;
				constants_count++;
				constants[constants_count] = 0; //-batchObject->matrix4.mh;
				constants_count++;
				constants[constants_count] = 0; //-batchObject->matrix4.ml;
				constants_count++;

				constants[constants_count] = 0; //-batchObject->matrix4.ml;
				constants_count++;
				//


				//
				constants[constants_count] = vertex_2->cameraX;
				constants_count++;
				constants[constants_count] = vertex_2->cameraY;
				constants_count++;
				constants[constants_count] = vertex_2->cameraZ;
				constants_count++;
				//constants[constants_count] = -batchObject->matrix4.md;
				//constants_count++;

				constants[constants_count] = vertex_2->u;
				constants_count++;
				constants[constants_count] = vertex_2->v;
				constants_count++;
				constants[constants_count] = 0; //-batchObject->matrix4.mh;
				constants_count++;
				constants[constants_count] = 0; //-batchObject->matrix4.ml;
				constants_count++;

				constants[constants_count] = 0; //-batchObject->matrix4.ml;
				constants_count++;
				//



				//
				constants[constants_count] = vertex_3->cameraX;
				constants_count++;
				constants[constants_count] = vertex_3->cameraY;
				constants_count++;
				constants[constants_count] = vertex_3->cameraZ;
				constants_count++;
				//constants[constants_count] = -batchObject->matrix4.md;
				//constants_count++;

				constants[constants_count] = vertex_3->u;
				constants_count++;
				constants[constants_count] = vertex_3->v;
				constants_count++;
				constants[constants_count] = 0; //-batchObject->matrix4.mh;
				constants_count++;
				constants[constants_count] = 0; //-batchObject->matrix4.ml;
				constants_count++;

				constants[constants_count] = 0; //-batchObject->matrix4.ml;
				constants_count++;
				//


				triangles_count++;
				vertex_2 = vertex_3;
				wrapper = wrapper->next;
			}
		}
		else
		{
			wrapper = wrapper->next;

			while (wrapper != nullptr)
			{
				if (constants.length() <= (constants_count + 24))
				{
					constants.resize(constants_count + 24);
				}


				Vertex *vertex_3 = wrapper->vertex.get();

				//
				constants[constants_count] = vertex_1->cameraX;
				constants_count++;
				constants[constants_count] = vertex_1->cameraY;
				constants_count++;
				constants[constants_count] = vertex_1->cameraZ;
				constants_count++;
				//constants[constants_count] = vertex_1->normalX;
				//constants_count++;

				constants[constants_count] = vertex_1->u;
				constants_count++;
				constants[constants_count] = vertex_1->v;
				constants_count++;
				constants[constants_count] = 0; //vertex_1->normalY;
				constants_count++;
				constants[constants_count] = 0; //vertex_1->normalZ;
				constants_count++;

				constants[constants_count] = 0; //vertex_1->normalZ;
				constants_count++;
				//

				//
				constants[constants_count] = vertex_2->cameraX;
				constants_count++;
				constants[constants_count] = vertex_2->cameraY;
				constants_count++;
				constants[constants_count] = vertex_2->cameraZ;
				constants_count++;
				//constants[constants_count] = vertex_2->normalX;
				//constants_count++;

				constants[constants_count] = vertex_2->u;
				constants_count++;
				constants[constants_count] = vertex_2->v;
				constants_count++;
				constants[constants_count] = 0; //vertex_2->normalY;
				constants_count++;
				constants[constants_count] = 0; //vertex_2->normalZ;
				constants_count++;

				constants[constants_count] = 0; //vertex_2->normalZ;
				constants_count++;
				//

				//
				constants[constants_count] = vertex_3->cameraX;
				constants_count++;
				constants[constants_count] = vertex_3->cameraY;
				constants_count++;
				constants[constants_count] = vertex_3->cameraZ;
				constants_count++;
				//constants[constants_count] = vertex_3->normalX;
				//constants_count++;

				constants[constants_count] = vertex_3->u;
				constants_count++;
				constants[constants_count] = vertex_3->v;
				constants_count++;
				constants[constants_count] = 0; //vertex_3->normalY;
				constants_count++;
				constants[constants_count] = 0; //vertex_3->normalZ;
				constants_count++;

				constants[constants_count] = 0; //vertex_3->normalZ;
				constants_count++;
				//

				triangles_count++;
				vertex_2 = vertex_3;
				wrapper = wrapper->next;
			}
		}

		face = face_next;
	}


	if (triangles_count > 0)
	{
		if (node->vertexBufferSize < constants_count)
		{
			VkDevice dev = vulkanWindow->device();

			if (dev == nullptr)
				return;

			if (node->vertexBuffer != nullptr)
			{
				delete node->vertexBuffer;
				node->vertexBuffer = nullptr;
			}

			VulkanFunctions *m_devFuncs = vulkanWindow->getFunctions();

			node->vertexBuffer = new VertexBufferResource(constants_count, 8);

			node->vertexBuffer->vk_create(m_devFuncs, dev, vulkanWindow->hostVisibleMemoryIndex());

			node->vertexBufferSize = constants_count;
		}

		node->vertexBuffer->vk_upload(constants.constData(), constants_count / 8);


		DrawParams p;
		p.vulkanWindow = vulkanWindow;
		p.camera = this;
		p.vertexBuffer = node->vertexBuffer;
		p.indexBuffer = nullptr;
		p.firstIndex =  0;
		p.numTriangles = triangles_count;
		p.object =  object;
		p.vulkanUniform = vulkanUniform;

		material->drawTransparentVulkan(&p);
	}
}


void Camera3D::addDecal(Decal *decal)
{
	if (this->decals.count() <= this->decalsCount)
		this->decals.resize(this->decalsCount + 10);

	this->decals[this->decalsCount] = decal;
	this->decalsCount++;
}


void Camera3D::deferredDestroy()
{
	Weak<Face> face = this->firstFace->next;

	while (face != nullptr)
	{
		Weak<Wrapper> wrapper = face->wrapper;
		if (wrapper != nullptr)
		{
			//Wrapper *wrapper_2 = nullptr;
			while (wrapper != nullptr)
			{
				wrapper->vertex = nullptr;
				//wrapper_2 = wrapper;
				wrapper = wrapper->next;
			}
			//this->lastWrapper->next = face->wrapper;
			//this->lastWrapper = wrapper_2;
		}

		face->material = nullptr;
		face->wrapper = nullptr;
		face = face->next;
	}

	if (this->firstFace != this->lastFace)
	{
		this->firstFace->next = nullptr;
		this->lastFace = this->firstFace;

		//this->lastFace->next = Face::collector;
		//Face::collector = this->firstFace.next;
		//this->firstFace.next = nullptr;
		//this->lastFace = this->firstFace;
	}


	//if (this->firstWrapper != this.lastWrapper)
	//{
	//	this->lastWrapper->next = Wrapper::collector;
	//	Wrapper::collector = this.firstWrapper.next;
	//	this->firstWrapper.next = nullptr;
	//	this->lastWrapper = this.firstWrapper;
	//}


	if (this->firstVertex != this->lastVertex)
	{
		this->firstVertex->next = nullptr;
		this->lastVertex = this->firstVertex;

		//this->lastVertex->next = Vertex::collector;
		//Vertex::collector = this->firstVertex->next;
		//this->firstVertex->next = nullptr;
		//this->lastVertex = this->firstVertex;
	}
}
