#include "Sprite3D.h"
#include "MyMath.h"
#include "Material_isType.h"
#include "flash/display/BitmapData.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "../core/Camera3D.h"
#include "../core/View.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "hardware/alternativa/engine3d/core/Wrapper.h"
#include "Material_isType.h"
#include "../core/VG.h"
#include <QDebug>


std::shared_ptr<Sprite3D> new_Sprite3D(double width, double height, std::shared_ptr<Material> material)
{
	return std::make_shared<Sprite3D>(width, height, material);
}


Sprite3D::Sprite3D(double width, double height, std::shared_ptr<Material> material)
{
	this->width = width;
	this->height = height;
	this->material = material;
	shadowMapAlphaThreshold = 100;
	this->originX = 0.5;
	this->originY = 0.5;
	this->autoSize = false;
	this->topLeftU = 0;
	this->topLeftV = 0;
	this->bottomRightU = 1;
	this->bottomRightV = 1;
	this->lighted = false;
	this->rotation = 0;
	this->perspectiveScale = true;
	this->clipping = 2;
	this->depthTest = true;
	this->sorting = 0;
	this->name_class = "Sprite3D";
}


void Sprite3D::clonePropertiesFrom(const Object3D *src)
{
	Object3D::clonePropertiesFrom(src);
	Sprite3D *t = (Sprite3D*)src;

	this->width = t->width;
	this->height = t->height;
	this->autoSize = t->autoSize;
	this->material = t->material;
	this->clipping = t->clipping;
	this->sorting = t->sorting;
	this->originX = t->originX;
	this->originY = t->originY;
	this->topLeftU = t->topLeftU;
	this->topLeftV = t->topLeftV;
	this->bottomRightU = t->bottomRightU;
	this->bottomRightV = t->bottomRightV;
	this->rotation = t->rotation;
	this->perspectiveScale = t->perspectiveScale;
	this->depthTest = t->depthTest;
}


std::shared_ptr<Object3D> Sprite3D::clone() const
{
	auto out = new_Sprite3D(0, 0, nullptr);
	out->clonePropertiesFrom(this);
	return out;
}


void Sprite3D::updateBounds(Object3D *_arg_1, Object3D *_arg_2)
{
	double _local_3 = this->width;
	double _local_4 = this->height;


	if (((this->autoSize) && Material_isType<TextureMaterial>(this->material.get())))
	{
		TextureMaterial *material = (TextureMaterial*)this->material.get();
		BitmapData *_local_11 = material->texture();
		if (_local_11 != nullptr)
		{
			_local_3 = (_local_11->width() * (this->bottomRightU - this->topLeftU));
			_local_4 = (_local_11->height() * (this->bottomRightV - this->topLeftV));
		}
	}

	double _local_5 = (((this->originX >= 0.5) ? this->originX : (1 - this->originX)) * _local_3);
	double _local_6 = (((this->originY >= 0.5) ? this->originY : (1 - this->originY)) * _local_4);
	double _local_7 = MyMath::sqrt(((_local_5 * _local_5) + (_local_6 * _local_6)));
	double _local_8 = 0;
	double _local_9 = 0;
	double _local_10 = 0;

	if (_arg_2 != nullptr)
	{
		double _local_12 = _arg_2->matrix4.ma;
		double _local_13 = _arg_2->matrix4.me;
		double _local_14 = _arg_2->matrix4.mi;
		double _local_15 = MyMath::sqrt((((_local_12 * _local_12) + (_local_13 * _local_13)) + (_local_14 * _local_14)));
		_local_12 = _arg_2->matrix4.mb;
		_local_13 = _arg_2->matrix4.mf;
		_local_14 = _arg_2->matrix4.mj;
		_local_15 = (_local_15 + MyMath::sqrt((((_local_12 * _local_12) + (_local_13 * _local_13)) + (_local_14 * _local_14))));
		_local_12 = _arg_2->matrix4.mc;
		_local_13 = _arg_2->matrix4.mg;
		_local_14 = _arg_2->matrix4.mk;
		_local_15 = (_local_15 + MyMath::sqrt((((_local_12 * _local_12) + (_local_13 * _local_13)) + (_local_14 * _local_14))));
		_local_7 = (_local_7 * (_local_15 / 3));
		_local_8 = _arg_2->matrix4.md;
		_local_9 = _arg_2->matrix4.mh;
		_local_10 = _arg_2->matrix4.ml;
	}
	if ((_local_8 - _local_7) < _arg_1->bound.boundMinX)
	{
		_arg_1->bound.boundMinX = (_local_8 - _local_7);
	}
	if ((_local_8 + _local_7) > _arg_1->bound.boundMaxX)
	{
		_arg_1->bound.boundMaxX = (_local_8 + _local_7);
	}
	if ((_local_9 - _local_7) < _arg_1->bound.boundMinY)
	{
		_arg_1->bound.boundMinY = (_local_9 - _local_7);
	}
	if ((_local_9 + _local_7) > _arg_1->bound.boundMaxY)
	{
		_arg_1->bound.boundMaxY = (_local_9 + _local_7);
	}
	if ((_local_10 - _local_7) < _arg_1->bound.boundMinZ)
	{
		_arg_1->bound.boundMinZ = (_local_10 - _local_7);
	}
	if ((_local_10 + _local_7) > _arg_1->bound.boundMaxZ)
	{
		_arg_1->bound.boundMaxZ = (_local_10 + _local_7);
	}
}


void Sprite3D::draw(Camera3D *camera)
{
	//qDebug("Sprite3D::draw");
	//qDebug() << typeid(*this).name();

	if (this->material == nullptr)
	{
		return;
	}

	Shared<Face> _local_3 = this->calculateFace(camera);
	if (_local_3 != nullptr)
	{
		this->lighted = false;
		if (((((useLight) && (!(camera->view->constrained))) && (camera->deferredLighting)) && (camera->deferredLightingStrength > 0)))
		{
			this->calculateLight(camera);
		}
#if 0
		if (((camera.debug) && ((_local_2 = camera.checkInDebug(this)) > 0)))
		{
			if ((_local_2 & Debug.EDGES))
			{
				Debug.drawEdges(camera, _local_3, 0xFFFFFF);
			}
			if ((_local_2 & Debug.BOUNDS))
			{
				Debug.drawBounds(camera, this, boundMinX, boundMinY, boundMinZ, boundMaxX, boundMaxY, boundMaxZ);
			}
		}
#endif
		camera->addTransparent(_local_3, this);
	}
}


void Sprite3D::calculateLight(Camera3D *camera)
{
	qDebug("used Sprite3D::calculateLight");
	(void)camera;

#if 0
	double _local_2 = (camera->viewSizeX / camera->focalLength);
	double _local_3 = (camera->viewSizeY / camera->focalLength);

	if (((!(camera->view->constrained)) && (((!(camera->directionalLight == nullptr)) &&
											(camera->directionalLightStrength > 0)) || ((!(_arg_1.shadowMap == null)) && (_arg_1.shadowMapStrength > 0)))))
	{
		this->lightConst[0] = 0;
		this->lightConst[1] = 0;
		this->lightConst[2] = 0;
	}
	else
	{
		this->lightConst[0] = 1;
		this->lightConst[1] = 1;
		this->lightConst[2] = 1;
	}

	double _local_13 = (this->matrix4.md * _local_2);
	double _local_14 = (this->matrix4.mh * _local_3);
	double _local_15 = this->matrix4.ml;
	double _local_16 = MyMath::sqrt((((_local_13 * _local_13) + (_local_14 * _local_14)) + (_local_15 * _local_15)));
	double _local_17 = (-(_local_13) / _local_16);
	double _local_18 = (-(_local_14) / _local_16);
	double _local_19 = (-(_local_15) / _local_16);

	int _local_4 = 0;
	while (_local_4 < camera->omniesCount)
	{
		OmniLight *_local_20 = _arg_1.omnies[_local_4];
		double _local_5 = (_local_20.cmd * _local_2);
		double _local_6 = (_local_20.cmh * _local_3);
		double _local_7 = _local_20.cml;
		double _local_8 = _local_20.attenuationEnd;
		if ((((((((_local_5 - _local_8) < _local_13) && ((_local_5 + _local_8) > _local_13)) && ((_local_6 - _local_8) < _local_14)) && ((_local_6 + _local_8) > _local_14)) && ((_local_7 - _local_8) < _local_15)) && ((_local_7 + _local_8) > _local_15)))
		{
			_local_5 = (_local_5 - _local_13);
			_local_6 = (_local_6 - _local_14);
			_local_7 = (_local_7 - _local_15);
			_local_16 = MyMath::.sqrt((((_local_5 * _local_5) + (_local_6 * _local_6)) + (_local_7 * _local_7)));
			if (((_local_16 > 0) && (_local_16 < _local_8)))
			{
				_local_5 = (_local_5 / _local_16);
				_local_6 = (_local_6 / _local_16);
				_local_7 = (_local_7 / _local_16);
				double _local_9 = ((_local_8 - _local_16) / (_local_20.attenuationEnd - _local_20.attenuationBegin));
				if (_local_9 > 1)
				{
					_local_9 = 1;
				}
				if (_local_9 < 0)
				{
					_local_9 = 0;
				}
				_local_9 = (_local_9 * _local_9);
				double _local_11 = (((_local_5 * _local_17) + (_local_6 * _local_18)) + (_local_7 * _local_19));
				_local_11 = (_local_11 * 0.5);
				_local_11 = (_local_11 + 0.5);
				double _local_12 = ((((_local_9 * _local_11) * _local_20.intensity) * 2) * _arg_1.deferredLightingStrength);
				this->lightConst[0] = (this->lightConst[0] + ((_local_12 * ((_local_20.color >> 16) & 0xFF)) / 0xFF));
				this->lightConst[1] = (this->lightConst[1] + ((_local_12 * ((_local_20.color >> 8) & 0xFF)) / 0xFF));
				this->lightConst[2] = (this->lightConst[2] + ((_local_12 * (_local_20.color & 0xFF)) / 0xFF));
				this->lighted = true;
			}
		}
		_local_4++;
	}
	_local_4 = 0;
	while (_local_4 < camera->spotsCount)
	{
		SpotLight *_local_21 = camera->spots[_local_4];
		double _local_5 = (_local_21.cmd * _local_2);
		double _local_6 = (_local_21.cmh * _local_3);
		double _local_7 = _local_21.cml;
		double _local_8 = _local_21.attenuationEnd;
		if ((((((((_local_5 - _local_8) < _local_13) && ((_local_5 + _local_8) > _local_13)) && ((_local_6 - _local_8) < _local_14)) && ((_local_6 + _local_8) > _local_14)) && ((_local_7 - _local_8) < _local_15)) && ((_local_7 + _local_8) > _local_15)))
		{
			_local_5 = (_local_5 - _local_13);
			_local_6 = (_local_6 - _local_14);
			_local_7 = (_local_7 - _local_15);
			_local_16 = Math.sqrt((((_local_5 * _local_5) + (_local_6 * _local_6)) + (_local_7 * _local_7)));
			if (((_local_16 > 0) && (_local_16 < _local_8)))
			{
				_local_5 = (_local_5 / _local_16);
				_local_6 = (_local_6 / _local_16);
				_local_7 = (_local_7 / _local_16);
				double _local_22 = ((((-(_local_5) * _local_21.cmc) * _local_2) - ((_local_6 * _local_21.cmg) * _local_3)) - (_local_7 * _local_21.cmk));
				double _local_23 = Math.cos((_local_21.falloff * 0.5));
				if (_local_22 > _local_23)
				{
					double _local_11 = (((_local_5 * _local_17) + (_local_6 * _local_18)) + (_local_7 * _local_19));
					_local_11 = (_local_11 * 0.5);
					_local_11 = (_local_11 + 0.5);
					double _local_9 = ((_local_8 - _local_16) / (_local_21.attenuationEnd - _local_21.attenuationBegin));
					if (_local_9 > 1)
					{
						_local_9 = 1;
					}
					if (_local_9 < 0)
					{
						_local_9 = 0;
					}
					_local_9 = (_local_9 * _local_9);
					double _local_10 = ((_local_22 - _local_23) / (Math.cos((_local_21.hotspot * 0.5)) - _local_23));
					if (_local_10 > 1)
					{
						_local_10 = 1;
					}
					if (_local_10 < 0)
					{
						_local_10 = 0;
					}
					_local_10 = (_local_10 * _local_10);
					double _local_12 = (((((_local_9 * _local_10) * _local_11) * _local_21.intensity) * 2) * _arg_1.deferredLightingStrength);
					this->lightConst[0] = (this->lightConst[0] + ((_local_12 * ((_local_21.color >> 16) & 0xFF)) / 0xFF));
					this->lightConst[1] = (this->lightConst[1] + ((_local_12 * ((_local_21.color >> 8) & 0xFF)) / 0xFF));
					this->lightConst[2] = (this->lightConst[2] + ((_local_12 * (_local_21.color & 0xFF)) / 0xFF));
					this->lighted = true;
				}
			}
		}
		_local_4++;
	}
	_local_4 = 0;
	while (_local_4 < _arg_1.tubesCount)
	{
		TubeLight *_local_24 = _arg_1.tubes[_local_4];
		double _local_25 = (_local_24.length * 0.5);
		double _local_26 = (_local_25 + _local_24.falloff);
		double _local_27 = (_local_24.cmc * _local_2);
		double _local_28 = (_local_24.cmg * _local_2);
		double _local_29 = _local_24.cmk;
		double _local_5 = ((_local_24.cmd * _local_2) + (_local_27 * _local_25));
		double _local_6 = ((_local_24.cmh * _local_3) + (_local_28 * _local_25));
		double _local_7 = (_local_24.cml + (_local_29 * _local_25));
		double _local_30 = (((_local_27 * (_local_13 - _local_5)) + (_local_28 * (_local_14 - _local_6))) + (_local_29 * (_local_15 - _local_7)));
		if (((_local_30 > -(_local_26)) && (_local_30 < _local_26)))
		{
			_local_5 = (_local_5 + ((_local_27 * _local_30) - _local_13));
			_local_6 = (_local_6 + ((_local_28 * _local_30) - _local_14));
			_local_7 = (_local_7 + ((_local_29 * _local_30) - _local_15));
			_local_16 = Math.sqrt((((_local_5 * _local_5) + (_local_6 * _local_6)) + (_local_7 * _local_7)));
			if (((_local_16 > 0) && (_local_16 < _local_24.attenuationEnd)))
			{
				_local_5 = (_local_5 / _local_16);
				_local_6 = (_local_6 / _local_16);
				_local_7 = (_local_7 / _local_16);
				double _local_11 = (((_local_5 * _local_17) + (_local_6 * _local_18)) + (_local_7 * _local_19));
				_local_11 = (_local_11 * 0.5);
				_local_11 = (_local_11 + 0.5);
				double _local_9 = ((_local_24.attenuationEnd - _local_16) / (_local_24.attenuationEnd - _local_24.attenuationBegin));
				if (_local_9 > 1)
				{
					_local_9 = 1;
				}
				if (_local_9 < 0)
				{
					_local_9 = 0;
				}
				_local_9 = (_local_9 * _local_9);
				if (_local_30 < 0)
				{
					_local_30 = -(_local_30);
				}
				double _local_10 = ((_local_26 - _local_30) / (_local_26 - _local_25));
				if (_local_10 > 1)
				{
					_local_10 = 1;
				}
				if (_local_10 < 0)
				{
					_local_10 = 0;
				}
				_local_10 = (_local_10 * _local_10);
				double _local_12 = (((((_local_9 * _local_10) * _local_11) * _local_24.intensity) * 2) * _arg_1.deferredLightingStrength);
				this->lightConst[0] = (this->lightConst[0] + ((_local_12 * ((_local_24.color >> 16) & 0xFF)) / 0xFF));
				this->lightConst[1] = (this->lightConst[1] + ((_local_12 * ((_local_24.color >> 8) & 0xFF)) / 0xFF));
				this->lightConst[2] = (this->lightConst[2] + ((_local_12 * (_local_24.color & 0xFF)) / 0xFF));
				this->lighted = true;
			}
		}
		_local_4++;
	}
#endif
}


Shared<Face> Sprite3D::calculateFace(Camera3D *camera)
{
	Shared<Vertex> _local_11 = nullptr;
	Shared<Vertex> _local_12 = nullptr;

	culling = (culling & 0x3C);

	double _local_2 = this->matrix4.ml;

	if (((_local_2 <= camera->nearClipping) || (_local_2 >= camera->farClipping)))
	{
		return nullptr;
	}

	double _local_13 = this->width;
	double _local_14 = this->height;
	double _local_15 = this->bottomRightU - this->topLeftU;
	double _local_16 = this->bottomRightV - this->topLeftV;

	if (((this->autoSize) && (Material_isType<TextureMaterial>(this->material.get()))))
	{
		BitmapData *_local_25 = std::dynamic_pointer_cast<TextureMaterial>(this->material)->texture();
		if (_local_25 != nullptr)
		{
			_local_13 = (_local_25->width() * _local_15);
			_local_14 = (_local_25->height() * _local_16);
		}
	}

	//double _local_17 = (camera->viewSizeX / _local_2);
	//double _local_18 = (camera->viewSizeY / _local_2);
	double _local_19 = (camera->focalLength / _local_2);
	double _local_20 = (camera->focalLength / camera->viewSizeX);
	double _local_21 = (camera->focalLength / camera->viewSizeY);


	double _local_3 = (this->matrix4.ma / _local_20);
	double _local_4 = (this->matrix4.me / _local_21);
	double _local_22 = MyMath::sqrt((((_local_3 * _local_3) + (_local_4 * _local_4)) + (this->matrix4.mi * this->matrix4.mi)));
	_local_3 = (this->matrix4.mb / _local_20);
	_local_4 = (this->matrix4.mf / _local_21);
	_local_22 = (_local_22 + MyMath::sqrt((((_local_3 * _local_3) + (_local_4 * _local_4)) + (this->matrix4.mj * this->matrix4.mj))));
	_local_3 = (this->matrix4.mc / _local_20);
	_local_4 = (this->matrix4.mg / _local_21);
	_local_22 = (_local_22 + MyMath::sqrt((((_local_3 * _local_3) + (_local_4 * _local_4)) + (this->matrix4.mk * this->matrix4.mk))));
	_local_22 = (_local_22 / 3);

	if (!this->perspectiveScale)
	{
		_local_22 = (_local_22 / _local_19);
	}

	if (this->rotation == 0)
	{
		double _local_26 = ((_local_22 * _local_13) * _local_20);
		double _local_27 = ((_local_22 * _local_14) * _local_21);
		_local_3 = (this->matrix4.md - (this->originX * _local_26));
		_local_4 = (this->matrix4.mh - (this->originY * _local_27));
		double _local_7 = (_local_3 + _local_26);
		double _local_8 = (_local_4 + _local_27);

		if (((culling > 0) && ((((_local_3 > _local_2) || (_local_4 > _local_2)) || (_local_7 < -(_local_2))) || (_local_8 < -(_local_2)))))
		{
			return nullptr;
		}

		_local_11 = Vertex::createList(4);
		_local_12 = _local_11;
		_local_12->cameraX = _local_3;
		_local_12->cameraY = _local_4;
		_local_12->cameraZ = _local_2;
		_local_12->u = this->topLeftU;
		_local_12->v = this->topLeftV;
		_local_12 = _local_12->next;
		_local_12->cameraX = _local_3;
		_local_12->cameraY = _local_8;
		_local_12->cameraZ = _local_2;
		_local_12->u = this->topLeftU;
		_local_12->v = this->bottomRightV;
		_local_12 = _local_12->next;
		_local_12->cameraX = _local_7;
		_local_12->cameraY = _local_8;
		_local_12->cameraZ = _local_2;
		_local_12->u = this->bottomRightU;
		_local_12->v = this->bottomRightV;
		_local_12 = _local_12->next;
		_local_12->cameraX = _local_7;
		_local_12->cameraY = _local_4;
		_local_12->cameraZ = _local_2;
		_local_12->u = this->bottomRightU;
		_local_12->v = this->topLeftV;
	}
	else
	{
		double _local_28 = (-(MyMath::sin(this->rotation)) * _local_22);
		double _local_29 = (MyMath::cos(this->rotation) * _local_22);
		double _local_30 = ((_local_29 * _local_13) * _local_20);
		double _local_31 = ((-(_local_28) * _local_13) * _local_21);
		double _local_32 = ((_local_28 * _local_14) * _local_20);
		double _local_33 = ((_local_29 * _local_14) * _local_21);
		_local_3 = ((this->matrix4.md - (this->originX * _local_30)) - (this->originY * _local_32));
		_local_4 = ((this->matrix4.mh - (this->originX * _local_31)) - (this->originY * _local_33));
		double _local_5 = (_local_3 + _local_32);
		double _local_6 = (_local_4 + _local_33);
		double _local_7 = ((_local_3 + _local_30) + _local_32);
		double _local_8 = ((_local_4 + _local_31) + _local_33);
		double _local_9 = (_local_3 + _local_30);
		double _local_10 = (_local_4 + _local_31);
		if (culling > 0)
		{
			if (this->clipping == 1)
			{
				if ((((((culling & 0x04) && (_local_2 <= -(_local_3))) && (_local_2 <= -(_local_5))) && (_local_2 <= -(_local_7))) && (_local_2 <= -(_local_9))))
				{
					return nullptr;
				}
				if ((((((culling & 0x08) && (_local_2 <= _local_3)) && (_local_2 <= _local_5)) && (_local_2 <= _local_7)) && (_local_2 <= _local_9)))
				{
					return nullptr;
				}
				if ((((((culling & 0x10) && (_local_2 <= -(_local_4))) && (_local_2 <= -(_local_6))) && (_local_2 <= -(_local_8))) && (_local_2 <= -(_local_10))))
				{
					return nullptr;
				}
				if ((((((culling & 0x20) && (_local_2 <= _local_4)) && (_local_2 <= _local_6)) && (_local_2 <= _local_8)) && (_local_2 <= _local_10)))
				{
					return nullptr;
				}
				_local_11 = Vertex::createList(4);
				_local_12 = _local_11;
				_local_12->cameraX = _local_3;
				_local_12->cameraY = _local_4;
				_local_12->cameraZ = _local_2;
				_local_12->u = this->topLeftU;
				_local_12->v = this->topLeftV;
				_local_12 = _local_12->next;
				_local_12->cameraX = (_local_3 + _local_32);
				_local_12->cameraY = (_local_4 + _local_33);
				_local_12->cameraZ = _local_2;
				_local_12->u = this->topLeftU;
				_local_12->v = this->bottomRightV;
				_local_12 = _local_12->next;
				_local_12->cameraX = ((_local_3 + _local_30) + _local_32);
				_local_12->cameraY = ((_local_4 + _local_31) + _local_33);
				_local_12->cameraZ = _local_2;
				_local_12->u = this->bottomRightU;
				_local_12->v = this->bottomRightV;
				_local_12 = _local_12->next;
				_local_12->cameraX = (_local_3 + _local_30);
				_local_12->cameraY = (_local_4 + _local_31);
				_local_12->cameraZ = _local_2;
				_local_12->u = this->bottomRightU;
				_local_12->v = this->topLeftV;
			}
			else
			{
				if ((culling & 0x04))
				{
					if (((((_local_2 <= -(_local_3)) && (_local_2 <= -(_local_5))) && (_local_2 <= -(_local_7))) && (_local_2 <= -(_local_9))))
					{
						return nullptr;
					}
					if (((((_local_2 > -(_local_3)) && (_local_2 > -(_local_5))) && (_local_2 > -(_local_7))) && (_local_2 > -(_local_9))))
					{
						culling = (culling & 0x3B);
					}
				}
				if ((culling & 0x08))
				{
					if (((((_local_2 <= _local_3) && (_local_2 <= _local_5)) && (_local_2 <= _local_7)) && (_local_2 <= _local_9)))
					{
						return nullptr;
					}
					if (((((_local_2 > _local_3) && (_local_2 > _local_5)) && (_local_2 > _local_7)) && (_local_2 > _local_9)))
					{
						culling = (culling & 0x37);
					}
				}
				if ((culling & 0x10))
				{
					if (((((_local_2 <= -(_local_4)) && (_local_2 <= -(_local_6))) && (_local_2 <= -(_local_8))) && (_local_2 <= -(_local_10))))
					{
						return nullptr;
					}
					if (((((_local_2 > -(_local_4)) && (_local_2 > -(_local_6))) && (_local_2 > -(_local_8))) && (_local_2 > -(_local_10))))
					{
						culling = (culling & 0x2F);
					}
				}
				if ((culling & 0x20))
				{
					if (((((_local_2 <= _local_4) && (_local_2 <= _local_6)) && (_local_2 <= _local_8)) && (_local_2 <= _local_10)))
					{
						return nullptr;
					}
					if (((((_local_2 > _local_4) && (_local_2 > _local_6)) && (_local_2 > _local_8)) && (_local_2 > _local_10)))
					{
						culling = (culling & 0x1F);
					}
				}
				_local_11 = Vertex::createList(4);
				_local_12 = _local_11;
				_local_12->cameraX = _local_3;
				_local_12->cameraY = _local_4;
				_local_12->cameraZ = _local_2;
				_local_12->u = this->topLeftU;
				_local_12->v = this->topLeftV;
				_local_12 = _local_12->next;
				_local_12->cameraX = (_local_3 + _local_32);
				_local_12->cameraY = (_local_4 + _local_33);
				_local_12->cameraZ = _local_2;
				_local_12->u = this->topLeftU;
				_local_12->v = this->bottomRightV;
				_local_12 = _local_12->next;
				_local_12->cameraX = ((_local_3 + _local_30) + _local_32);
				_local_12->cameraY = ((_local_4 + _local_31) + _local_33);
				_local_12->cameraZ = _local_2;
				_local_12->u = this->bottomRightU;
				_local_12->v = this->bottomRightV;
				_local_12 = _local_12->next;
				_local_12->cameraX = (_local_3 + _local_30);
				_local_12->cameraY = (_local_4 + _local_31);
				_local_12->cameraZ = _local_2;
				_local_12->u = this->bottomRightU;
				_local_12->v = this->topLeftV;
			}
		}
		else
		{
			_local_11 = Vertex::createList(4);
			_local_12 = _local_11;
			_local_12->cameraX = _local_3;
			_local_12->cameraY = _local_4;
			_local_12->cameraZ = _local_2;
			_local_12->u = this->topLeftU;
			_local_12->v = this->topLeftV;
			_local_12 = _local_12->next;
			_local_12->cameraX = (_local_3 + _local_32);
			_local_12->cameraY = (_local_4 + _local_33);
			_local_12->cameraZ = _local_2;
			_local_12->u = this->topLeftU;
			_local_12->v = this->bottomRightV;
			_local_12 = _local_12->next;
			_local_12->cameraX = ((_local_3 + _local_30) + _local_32);
			_local_12->cameraY = ((_local_4 + _local_31) + _local_33);
			_local_12->cameraZ = _local_2;
			_local_12->u = this->bottomRightU;
			_local_12->v = this->bottomRightV;
			_local_12 = _local_12->next;
			_local_12->cameraX = (_local_3 + _local_30);
			_local_12->cameraY = (_local_4 + _local_31);
			_local_12->cameraZ = _local_2;
			_local_12->u = this->bottomRightU;
			_local_12->v = this->topLeftV;
		}
	}

	camera->lastVertex->next = _local_11;
	camera->lastVertex = _local_12;

	Shared<Face> _local_23 = Face::create(21);
	_local_23->material = this->material;
	camera->lastFace->next = _local_23;
	camera->lastFace = _local_23;
	Shared<Wrapper> _local_24 = Wrapper::create();
	_local_23->wrapper = _local_24;
	_local_24->vertex = _local_11;
	_local_11 = _local_11->next;

	while (_local_11 != nullptr)
	{
		_local_24->next = _local_24->create();
		_local_24 = _local_24->next;
		_local_24->vertex = _local_11;
		_local_11 = _local_11->next;
	}

	return _local_23;
}


Shared<VG> Sprite3D::getVG(Camera3D *camera)
{
	if (this->material == nullptr)
	{
		return nullptr;
	}

	Shared<Face> face = this->calculateFace(camera);

	if (face != nullptr)
	{
		this->lighted = false;

		if (((((useLight) && (!(camera->view->constrained))) && (camera->deferredLighting)) && (camera->deferredLightingStrength > 0)))
		{
			this->calculateLight(camera);
		}

		face->normalX = 0;
		face->normalY = 0;
		face->normalZ = -1;
		face->offset = -this->matrix4.ml;

		return VG::create(this->shared_from_this(), face, this->sorting, false, true, 3);
	}

	return nullptr;
}
